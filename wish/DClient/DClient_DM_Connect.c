/*
  DClient_DM_Connect.c

  Routine to request open a socket to the DServ Server, which is getting data for
    the PCR.

  An actual DServ frame is not retrieved here

  Written   4sep06  DLM  Started with DClient_DM_Init.c

*/

#include "PCR_Wish.h"
#include "DClient.h"

int DClient_DM_Connect( DClient_Info *DM_Info, char *name,
		       int debug, char *errorMsg)
{

  int status;

  /*
    Setup Server information in structure, if not already connected
  */
  if ( !DM_Info->server.connected ) {

    status = Server_SetupState( (Server_Info *)DM_Info, name, debug);
    if ( status ) {
      strcpy( errorMsg, "DClient_DM_Connect: Error in Server_SetupState");
      printf("  %s\n", errorMsg);
      return(status);
    }

    status = Server_Open( (Server_Info *)DM_Info, debug);
    if ( status == -12 ) {
      sprintf( errorMsg, "DClient_DM_Connect: Could not connect to %s server",
	       DM_Info->server.name);
      printf("  %s\n", errorMsg);
      return(status);
    } else if ( status ) {
      sprintf( errorMsg, "DClient_DM_Connect: Could not connect to %s server",
	       DM_Info->server.name);
      printf("  %s\n", errorMsg);
      return(status);
    }
  }

  /*
    Allocate memory for the string filled when reading from a socket
      ** NOTE: This string should be removed (see Socket_ReadString() in
                pcr/Socket directory  7sep06  DLM
   */
  //  DM_Info->server.nBytes = TOTAL_DM_BYTES;
  DM_Info->server.nBytes = 72*72*sizeof(short);
  if ( debug ) {
    printf("  DClient_DM_Init: Allocating in_line buffer to %d bytes\n", DM_Info->server.nBytes);
  }

  /*
    Check to see if memory has already been allocated.  If so, free it
  */
  if ( DM_Info->server.in_line != NULL) {
    if ( debug ) printf("  DClient_DM_Init: Freeing %s input array\n", DM_Info->server.name );
    free(DM_Info->server.in_line);
  }

  if (( DM_Info->server.in_line = (char *) malloc (DM_Info->server.nBytes) ) == NULL) {
    sprintf( errorMsg, "DClient_DM_Init: Error allocating %s input array", DM_Info->server.name);
    printf("%s\n", errorMsg);
    return(-1);
  }
  memset( DM_Info->server.in_line, 0, DM_Info->server.nBytes );

  strcpy( DM_Info->server.in_line, "Hello\n");
 
  return(0);
}
