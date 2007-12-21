/*===============================================================================
 * DServ_GetSingle         
 *================================================================================

  This routine gets a frame from the either the WFSC or DM DServ Memory, and sends
    the requested image (WFSC, Slope, RTR, Cmd, Cur, Pos) to the client

  Written  16jul06  DLM  Started with WFSC_GetSingle.c

*/

#include "PCR.h"
#include "DServ.h"

#define DEBUG 0

/*===============================================================================
 * DServ_GetSingle 
 *===============================================================================*/

int DServ_GetSingle( DServ_Info *WFSC_Info, DServ_Info *DM_Info, int sockfd, int *debug)
{
  long nBytes, nBytes_Send, new_frame;
  DServ_Info *Info;
  char *DServ_DataPtr;
  long new_frameNumber;
  int status;
  char request[STRING_LENGTH];
  static long old_frameNumber = -1;

  int sets, i;
  int offset[6];
  int bytes[6];

#if( DEBUG )
  short *nxtShort;
  long datacount;
  int j, k;
#endif

  /*
    Set the Info pointer to the DM_Info structure for now.  Later need to determine
      if we are going to send data from the WFSC or DM buffer.  16jul06  DLM
  */
  Info = DM_Info;

  /*
    Read request from the client
  */
  if ( Socket_StringRead( sockfd, request) ) {
    if ( *debug ) {
      printf("  DServ_GetSingle: Error reading request in Socket_StringRead\n");
      fflush(stdout);
    }
    return(-1);
  }

  /*
    Check that a request was sent
  */
  if ( strlen(request) <= 1 ) {
    printf("  DServ_GetSingle: Warning: request of zero length recieved\n");
    fflush(stdout);
    return(-1);
  }

  if ( *debug ) {
    printf("  DServ_GetSingle: request = %s\n", request);
    fflush(stdout);
  }

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  status = Socket_StringLowerCase(request);

  /*
    WFSC frames are available if Info->data->last >= 0
      If there are no frames available, send the -2 flag
      to the client so they may take the appropriate action
  */
  if ( Info->data->last < 0 ) {
    return(-2);
  }	

  /*
    Find pointer to the last filled frame
  */
  DServ_DataPtr = Info->data->framePtr[Info->data->last];

  /*
    Skip the 'SOF' at the beginning of the frame
  */
  DServ_DataPtr+=3;

  /*
    Save the frame number
  */
  new_frameNumber = *((long *)DServ_DataPtr);

  /*
    If the frame numbers are the same, then no new data is available
      in the DDS.  Send the number of bytes positive (+) for a new
      frame and negative (-) for repeat frame.  In either case, send
      the data frame.
   */

  if( new_frameNumber == old_frameNumber ) {
    new_frame = -1;
  } else {
    new_frame = 1;
  }
  printf("%3ld %s  %d  %d\n", new_frame, request, ntohl(new_frameNumber), ntohl(old_frameNumber));
  fflush(stdout);
  old_frameNumber = new_frameNumber;

  /*
    Determine which image the client has requested and set the number of bytes
      to be sent and set the offset in the frame to the correct positin.
  */
  nBytes = 0;
  sets = 0;
  if ( strstr( request, "wfsc")!=NULL ) {

    //    printf("  DServ_GetSingle: WFSC found\n");
    //    DServ_DataPtr += Info->data->wfsc_offset;
    offset[sets] = Info->data->wfsc_offset;
    bytes[sets] = Info->data->wfsc_bytes;
    sets++;
    nBytes += Info->data->wfsc_bytes;

  }

  if ( strstr( request, "slope")!=NULL ) {

    //    printf("  DServ_GetSingle: Slope found\n");
    //    DServ_DataPtr += Info->data->slope_offset;
    offset[sets] = Info->data->slope_offset;
    bytes[sets] = Info->data->slope_bytes;
    sets++;
    nBytes += Info->data->slope_bytes;

  }

  if ( strstr( request, "rtr")!=NULL ) {

    //    printf("  DServ_GetSingle: RTR found\n");
    //    DServ_DataPtr += Info->data->rtr_offset;
    offset[sets] = Info->data->rtr_offset;
    bytes[sets] = Info->data->rtr_bytes;
    sets++;
    nBytes += Info->data->rtr_bytes;

  }

  if ( strstr( request, "cmd")!=NULL ) {

    //    printf("  DServ_GetSingle:  Cmd found\n");
    //    DServ_DataPtr += Info->data->cmd_offset;
    offset[sets] = Info->data->cmd_offset;
    bytes[sets] = Info->data->cmd_bytes;
    sets++;
    nBytes += Info->data->cmd_bytes;

  }

  if ( strstr( request, "cur")!=NULL ) {

    //    printf("  DServ_GetSingle:  Cur found\n");
    //    DServ_DataPtr += Info->data->cur_offset;
    offset[sets] = Info->data->cur_offset;
    bytes[sets] = Info->data->cur_bytes;
    sets++;
    nBytes += Info->data->cur_bytes;

  }

  if ( strstr( request, "pos")!=NULL ) {

    //    printf("  DServ_GetSingle:  Pos found\n");
    //    DServ_DataPtr += Info->data->pos_offset;
    offset[sets] = Info->data->pos_offset;
    bytes[sets] = Info->data->pos_bytes;
    sets++;
    nBytes += Info->data->pos_bytes;

  }

  if ( strstr( request, "all")!=NULL ) {

    //    printf("  DServ_GetSingle:  All found\n");
    //    DServ_DataPtr += Info->data->pos_offset;
    sets = 0;
    offset[sets] = Info->data->all_offset;
    bytes[sets] = Info->data->all_bytes;
    sets++;
    nBytes = Info->data->all_bytes;

  }

  if ( sets <=0 ) {
    
    printf("  DServ_GetSingle: Unknown request: %s\n", request);
    fflush(stdout);
    return(-1);

  }

  /*
    Set the number of byte to negative if this is a repeat frame.  The client
      can use this information to either quit or refresh at a slower rate.
  */
  nBytes_Send = nBytes*new_frame;

#if( DEBUG )

  printf(";=============================================;\n");
  printf("|  Debug information from DServ_GetSingle      |\n");
  printf(";---------------------------------------------;\n");
  printf("|    Frame Number = %d\n", ntohl(new_frameNumber));
  printf("|       New Frame = %ld (-1 is old frame)\n", new_frame);
  printf("| Number of Bytes = %ld\n", nBytes);
  printf(";=============================================;\n\n");
  fflush(stdout);

#endif

  /*
    Send the number of bytes to the client.  This long is in "host" format
      so a htonl() must be used.
  */
  nBytes_Send = htonl(nBytes_Send);
  status = Socket_StringWrite( sockfd, (char *)&nBytes_Send, sizeof(long));
  if ( status !=  sizeof(long)) {
    if ( debug ) {
      printf("  DServ_GetSingle: Error sending nBytes_Send in Socket_StringWrite\n");
      fflush(stdout);
    }
    return(-1);
  }	

  /*
    Send the frame number to the client.  The frameNumber is stored in the DServ
      memory in "network" format so a conversion is not needed.
  */
  status = Socket_StringWrite( sockfd, (char *)&new_frameNumber, sizeof(long));
  if ( status !=  sizeof(long)) {
    if ( debug ) {
      printf("  DServ_GetSingle: Error sending new_frameNumber in Socket_StringWrite\n");
      fflush(stdout);
    }
    return(-1);
  }

  for ( i=0; i<sets; i++) {

    /*
      Set the point to the beginning of the frame, and then offset to the data
        set to send
    */
    DServ_DataPtr = Info->data->framePtr[Info->data->last];
    DServ_DataPtr += offset[i];
    
    /*
      Send the WFSC Frame to the client.  The frame data are stored in the DServ
      memory in "network" format so a conversion is not needed.
    */
    status = Socket_StringWrite( sockfd, (char *)DServ_DataPtr, bytes[i]);
    if ( status !=  bytes[i]) {
      if ( debug ) {
	printf("  DServ_GetSingle: Error sending DServ_DataPtr in Socket_StringWrite\n");
	fflush(stdout);
      }
      return(-1);
    }	

#if( DEBUG )
    /*
      Print out the first few values
    */
    nxtShort = (short *)DServ_DataPtr;
    printf("    Data Set %d\n", i);
    datacount = 16;
    for ( j=0; j<datacount; j++) {
      for (k=0; k<4; k++) {
	printf(" %6d", *nxtShort++);
      }
      printf("\n");
    }
    printf(";=============================================;\n\n");
    fflush(stdout);
#endif

  }

  return(0);
}
