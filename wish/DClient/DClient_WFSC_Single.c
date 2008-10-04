/*
  DClient_WFSC_Single.c

  This routine requests one frame form the WFSC Single server
    on the VME.  A connection to this server has already been
    established.

  The server returns the number of bytes it will return (int), the
    frame number (int) and the WFSC frame (char nBytes int).

  The number of bytes is checked and if returned number in the structure
    WFSC_Single_Info is not the same as will be returned, the structure
    values will be updated.

  Written  27aug03  DLM

  Altered  23mar05  DLM  Now use a structure to pass information to and from
                           this routine.

*/

#include "PCR_Wish.h"
#include "DClient.h"

#define DEBUG 0

int DClient_WFSC_Single( DClient_Info *WFSC_Single_Info, short *WFSC_Array, int *frameNumber,
			 int debug, char *errorMsg)
{

  int num, i;
  int status;
  fd_set rfds;
  struct timeval tv;
  int nBytes, retVal, count;
  short *nxtShort, *nxtArray;

#if( DEBUG )
  int j;
#endif

  strcpy( WFSC_Single_Info->server.out_line, "Single\n");

  status = Server_Write( (Server_Info *)WFSC_Single_Info);
  if ( status ) {
    strcpy( errorMsg, "  DClient_WFSC_Single: Error in Server_Write sending Single request");
    printf("%s\n", errorMsg);
    return(status);
  }

  /*
    New DServ for PCR
  */
  if ( !strncmp( WFSC_Single_Info->server.type, "Spots", strlen("Spots")) ) {
    strcpy( WFSC_Single_Info->server.out_line, "WFSC\n");

    status = Server_Write( (Server_Info *)WFSC_Single_Info);
    if ( status ) {
      strcpy( errorMsg, "  DClient_WFSC_Single: Error in Server_Write sending WFSC request");
      printf("%s\n", errorMsg);
      return(status);
    }
  }

  /*
    Watch to see when there is data to be read on sockfd
  */
  FD_ZERO(&rfds);
  FD_SET(WFSC_Single_Info->server.fd, &rfds);
  /* Wait up to one seconds. */
  tv.tv_sec = 3;
  tv.tv_usec = 0;
 
  status = select( (WFSC_Single_Info->server.fd)+1, &rfds, NULL, NULL, &tv);

  if ( !status ) {
    printf("  DClient_WFSC_Single: error getting a single frame\n");
    return(-1);
  }

  status = Server_Read( (Server_Info *)WFSC_Single_Info, sizeof(int));
  if ( status ) {
    strcpy( errorMsg, "  DClient_WFSC_Single: Error in Server_Read getting nBytes");
    printf("%s\n", errorMsg);
    return(status);
  }
  nBytes = ntohl( *(int *)WFSC_Single_Info->server.in_line);

#if( DEBUG )
  printf("  DClient_WFSC_Single: Byte Count = %d\n", nBytes);
#endif

  if ( abs(nBytes) == WFSC_Single_Info->server.nBytes ){

    if ( nBytes < 0 ) {
      strcpy( errorMsg, "  DClient_WFSC_Single: No new WFSC data, repeat frame");
      if ( debug ) printf("%s\n", errorMsg);
      nBytes = abs(nBytes);
      retVal = 1;
    } else {
      retVal = 0;
    }

    num = Server_Read( (Server_Info *)WFSC_Single_Info, sizeof(int));
    if ( status ) {
      strcpy( errorMsg, "  DClient_WFSC_Single: Error in Server_Read getting frameNumber");
      printf("%s\n", errorMsg);
      return(status);
    }
    *frameNumber = ntohl( *(int *)WFSC_Single_Info->server.in_line);

#if( DEBUG )
    printf("  DClient_WFSC_Single: Frame Number = %d\n", *frameNumber);
#endif

    num = Server_Read( (Server_Info *)WFSC_Single_Info, WFSC_Single_Info->server.nBytes);
    if ( status ) {
      strcpy( errorMsg, "  DClient_WFSC_Single: Error in Server_Read getting frame");
      if ( debug ) printf("%s\n", errorMsg);
      return(status);
    }

    nxtArray = WFSC_Array;
    nxtShort = (short *)WFSC_Single_Info->server.in_line;
    count = 0;
    for (i=0; i<nBytes; i+=2) {
      *nxtArray++ = ntohs(*nxtShort++);
      count++;
    }

#if( DEBUG )
    nxtArray = WFSC_Array;
    for (i=0; i<24; i++) {
      for (j=0; j<24; j++) {
	printf(" %4d", *nxtArray++);
      }
      printf("\n");
    }
    //    for (i=0; i<10; i++) {
    //      printf(" %d  %d\n", i, *nxtArray++);
    //    }
#endif

    if ( debug ) {
      printf("  DClient_WFSC_Single: Frame number = %d\n", *frameNumber);
      printf("                       Number of Bytes = %d\n", nBytes);
    }

  } else if ( nBytes < 0 ) {
    /*
      Return (-1) indicates an error
      Return (-2) no data available in DDS
      Return (-3) DDS not initialized
      Return (-11) indicates client WFSC binning has changed
    */
    if ( nBytes == -1 ) {
      strcpy( errorMsg, "  DClient_WFSC_Single: Error in server: closing connection");
      if ( debug ) printf("%s\n", errorMsg);
    } else if ( nBytes == -2 ) {
      strcpy( errorMsg, "  DClient_WFSC_Single: No data available, try again later");
      if ( debug ) printf("%s\n", errorMsg);
    } else if ( nBytes == -3 ) {
      strcpy( errorMsg, "  DClient_WFSC_Single: DDS not initialized, try again later");
      if ( debug ) printf("%s\n", errorMsg);
    } else if ( nBytes == -11 ) {
      strcpy( errorMsg, "  DClient_WFSC_Single: WFSC size changed: re-initializing");
      if ( debug ) printf("%s\n", errorMsg);
    } else {
      sprintf( errorMsg, "  DClient_WFSC_Single: Uknown error code %d", nBytes);
      if ( debug ) printf("%s\n", errorMsg);
    }
    return(nBytes);

  } else {

    strcpy( errorMsg, "  DClient_WFSC_Single: Error reading the correct number of bytes");
    printf("%s\n", errorMsg);
    return(-1);
  }

  return(retVal);
}
