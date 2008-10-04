/*
  DClient_Slopes_Single.c

  This routine requests one frame form the Slopes Single server
    on the VME.  A connection to this server has already been
    established.

  The server returns the number of bytes it will return (int), the
    frame number (int) and the Slopes frame (char nBytes int).

  The number of bytes is checked and if returned number in the structure
    Slopes_Single_Info is not the same as will be returned, the structure
    values will be updated.

  Written  27aug03  DLM  Started with DClient_WFSC_Single.c

*/

#include "PCR_Wish.h"
#include "DClient.h"

#define DEBUG 0

int DClient_Slopes_Single( DClient_Info *Slopes_Single_Info, short *Slopes_Array, int *frameNumber,
			   int debug, char *errorMsg)
{

  int num, i;
  int status;
  fd_set rfds;
  struct timeval tv;
  int nBytes, retVal, count;
  short *nxtShort, *nxtArray;

  strcpy( Slopes_Single_Info->server.out_line, "Single\n");

  status = Server_Write( (Server_Info *)Slopes_Single_Info);
  if ( status ) {
    strcpy( errorMsg, "DClient_Slopes_Single: Error in Server_Write sending Single request");
    printf("%s\n", errorMsg);
    return(status);
  }

  strcpy( Slopes_Single_Info->server.out_line, "Slopes\n");

  status = Server_Write( (Server_Info *)Slopes_Single_Info);
  if ( status ) {
    strcpy( errorMsg, "DClient_Slopes_Single: Error in Server_Write sending Single request");
    printf("%s\n", errorMsg);
    return(status);
  }

  /*
    Watch to see when there is data to be read on sockfd
  */
  FD_ZERO(&rfds);
  FD_SET(Slopes_Single_Info->server.fd, &rfds);
  /* Wait up to one seconds. */
  tv.tv_sec = 1;
  tv.tv_usec = 0;
 
  status = select( (Slopes_Single_Info->server.fd)+1, &rfds, NULL, NULL, &tv);

  if ( !status ) {
    printf("  DClient_Slopes_Single: Timed out waiting for a single frame\n");
    return(-1);
  }

  status = Server_Read( (Server_Info *)Slopes_Single_Info, sizeof(int));
  if ( status ) {
    strcpy( errorMsg, "DClient_Slopes_Single: Error in Server_Read getting nBytes");
    printf("%s\n", errorMsg);
    return(status);
  }
  nBytes = ntohl( *(int *)Slopes_Single_Info->server.in_line);

#if( DEBUG )
  printf("  DClient_Slopes_Single: Byte Count = %d\n", nBytes);
#endif

  if ( abs(nBytes) == Slopes_Single_Info->server.nBytes ){

    if ( nBytes < 0 ) {
      strcpy( errorMsg, "  DClient_Slopes_Single: No new Slope data, repeat frame");
      if ( debug ) printf("%s\n", errorMsg);
      nBytes = abs(nBytes);
      retVal = 1;
    } else {
      retVal = 0;
    }

    num = Server_Read( (Server_Info *)Slopes_Single_Info, sizeof(int));
    if ( status ) {
      strcpy( errorMsg, "DClient_Slopes_Single: Error in Server_Read getting frameNumber");
      printf("%s\n", errorMsg);
      return(status);
    }
    *frameNumber = ntohl( *(int *)Slopes_Single_Info->server.in_line);

#if( DEBUG )
    printf(" DClient_Slopes_Single: Frame Number = %d\n", *frameNumber);
#endif

    num = Server_Read( (Server_Info *)Slopes_Single_Info, Slopes_Single_Info->server.nBytes);
    if ( status ) {
      strcpy( errorMsg, "DClient_Slopes_Single: Error in Server_Read getting frame");
      printf("%s\n", errorMsg);
      return(status);
    }

    nxtArray = Slopes_Array;
    nxtShort = (short *)Slopes_Single_Info->server.in_line;
    count = 0;
    for (i=0; i<nBytes; i+=2) {
      //      *nxtArray++ = *nxtShort++;
      *nxtArray++ = ntohs(*nxtShort++);
      count++;
    }

#if( DEBUG )
    nxtArray = Slopes_Array;
    for (i=0; i<count; i++) {
      printf(" %d  %d\n", i, *nxtArray++);
    }
#endif

    if ( debug ) {
      printf("  DClient_Slopes_Single: Frame number = %d\n", *frameNumber);
      printf("                         Number of Bytes = %d\n", nBytes);
    }

  } else if ( nBytes < 0 ) {

    /*
      Return (-1) indicates an error
      Return (-2) no data available in DDS
    */
    if ( nBytes == -1 ) {
      strcpy( errorMsg, "DClient_Slopes_Single: Error in server: closing connection");
      if ( debug ) printf("%s\n", errorMsg);
    } else if ( nBytes == -2 ) {
      strcpy( errorMsg, "DClient_Slopes_Single: No data available, try again later");
      if ( debug ) printf("%s\n", errorMsg);
    } else {
      sprintf( errorMsg, "DClient_Slopes_Single: Unkown error code %d", nBytes);
      if ( debug ) printf("%s\n", errorMsg);
    }
    return(nBytes);

  } else {

    strcpy( errorMsg, "DClient_Slopes_Single: Error reading the correct number of bytes");
    printf("%s\n", errorMsg);
    return(-1);
  }

  return(retVal);
}
