/*===============================================================================
 * WFSC_GetSingle         
 *================================================================================

  This routine gets a WFSC spot image from the DDS, reorders the frame from its
    interleaved form (from the WFSC/SDSU) into row/column form needed for display.

  **Note: The data to be sent are stored in the DServ memory in "network" format
    so hton*() routines are not needed

  Written  24apr03  DLC

  Altered  12aug03  DLM Changed original server for WFSC counts and made it into
                          nga WFSC spot image server
*/

#include "PCR.h"
#include "DServ.h"

#define DEBUG 0

/*===============================================================================
 * WFSC_GetSingle 
 *===============================================================================*/

int WFSC_GetSingle( DServ_Info *Info, int sockfd, int debug)
{
  long nBytes, temp_nBytes, new_frame;
  char *WFSC_DataPtr;
  long new_frameNumber;
  int status;
  static long old_frameNumber = -1;

#if( DEBUG )
  long *nxtLong;
  short *nxtShort;
  long datacount;
  int i, j;
  char dimension[STRING_LENGTH];
#endif


  /*
    WFSC frames are available if Info->data->last >= 0
      If there are no frames available, send the -2 flag
      to the client so they may take the appropriate action
  */
  if ( Info->data->last < 0 ) {
    nBytes = htonl(-2);
    status = Socket_StringWrite( sockfd, (char *)&nBytes, sizeof(long));
    if ( status !=  sizeof(long)) {
      if ( debug ) {
	printf("  WFSC_Single: Error sending nBytes in Socket_StringWrite\n");
      }
    }
    return(-1);
  }	
    

  /*
    Find pointer to the last filled frame
  */
  WFSC_DataPtr = Info->data->framePtr[Info->data->last];

  /*
    Skip the 'SOF' at the beginning of the frame
  */
  WFSC_DataPtr+=3;

  /*
    Save the frame number
  */
  new_frameNumber = *((long *)WFSC_DataPtr);
  WFSC_DataPtr += sizeof(long);

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
  old_frameNumber = new_frameNumber;

  /*
    Set the number of byte to negative if this is a repeat frame.  The client
      can use this information to either quit or refresh at a slower rate.
  */
  nBytes = Info->data->wfsc_bytes*new_frame;

#if( DEBUG )

  if ( (Info->data->wfsc_bytes == (24*24*2)) ){
    strcpy( dimension, "24x24");
  } else if ( (Info->data->wfsc_bytes == (72*72*2)) ) {
    strcpy( dimension, "72x72");
  } else {
    strcpy( dimension, "unknown");
  }
    
  nxtLong = (long *)WFSC_DataPtr;
  printf(";=============================================;\n");
  printf("|  Debug information from WFSC_GetSingle      |\n");
  printf(";---------------------------------------------;\n");
  printf("|    Frame Number = %ld\n", ntohl(new_frameNumber));
  printf("|       New Frame = %ld (-1 is old frame)\n", new_frame);
  printf("| Number of Bytes = %ld\n", nBytes);
  printf("| Frame Dimension = %s\n", dimension);
  printf(";=============================================;\n\n");

  /*
    Print out the first few values
  */
  nxtShort = (short *)WFSC_DataPtr;
  printf("    WFSC Data\n");
  datacount = 16;
  for ( j=0; j<datacount; j+=4) {
    printf(" %3d = ", j);
    for (i=0; i<4; i++) {
      printf(" %6d", *nxtShort++);
    }
    printf("\n");
  }
  printf(";=============================================;\n\n");
#endif

  /*
    Send the number of bytes to the client.  This long is in "host" format
      so a htonl() must be used.
  */
  temp_nBytes = htonl(nBytes);
  status = Socket_StringWrite( sockfd, (char *)&temp_nBytes, sizeof(long));
  if ( status !=  sizeof(long)) {
    if ( debug ) {
      printf("  WFSC_Single: Error sending nBytes in Socket_StringWrite\n");
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
      printf("  WFSC_Single: Error sending new_frameNumber in Socket_StringWrite\n");
    }
    return(-1);
  }

  /*
    Send the WFSC Frame to the client.  The frame data are stored in the DServ
      memory in "network" format so a conversion is not needed.
  */
  status = Socket_StringWrite( sockfd, (char *)WFSC_DataPtr, Info->data->wfsc_bytes);
  if ( status !=  Info->data->wfsc_bytes) {
    if ( debug ) {
      printf("  WFSC_Single: Error sending WFSC_DataPtr in Socket_StringWrite\n");
    }
    return(-1);
  }	

  return(nBytes);
}
