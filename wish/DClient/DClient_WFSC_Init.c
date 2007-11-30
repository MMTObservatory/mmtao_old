/*
  DClient_WFSC_Init.c

  Routine to request open a socket to the DClient server on the VME, start
    the requested type of WFSC server (Single, Save) and allocate the memory
    needed to transfer and manipulate the requested data.

  An actual WFSC frame is not retrieved here

  Written  23mar05  DLM  Combined DClient_WFSC_Single, DClient_WFSC_Init,
                           and added Server_SetupState and Server_Open to make
			   the one routine.

			 This can be called by either the Single or the Save routines

*/

#include "PCR_Wish.h"
#include "DClient.h"

int DClient_WFSC_Init( DClient_Info *WFSC_Info, char *name,
		       short **WFSC_Array, int debug, char *errorMsg)
{

  int status;
  char msg[STRING_LENGTH];
  char request[STRING_LENGTH];

  /*
    Setup Server information in structure, if not already connected
  */
  if ( !WFSC_Info->server.connected ) {

    status = Server_SetupState( (Server_Info *)WFSC_Info, name, debug);
    if ( status ) {
      strcpy( errorMsg, "DClient_WFSC_Init: Error in Server_SetupState");
      if ( debug ) printf("  %s\n", errorMsg);
      return(status);
    }

    status = Server_Open( (Server_Info *)WFSC_Info, debug);
    if ( status == -12 ) {
      sprintf( errorMsg, "DClient_WFSC_Init: Could not connect to %s server", WFSC_Info->server.name);
      if ( debug ) printf("  %s\n", errorMsg);
      return(status);
    } else if ( status ) {
      sprintf( errorMsg, "DClient_WFSC_Init: Error connecting to %s server", WFSC_Info->server.name);
      if ( debug ) printf("  %s\n", errorMsg);
      return(status);
    }

  }

  /*
    When information about the Spot array has returned from the WFC server,
      local arrays and parameters are set.  These setting do not change while
      spot data is being retrieved by this process.  In this way, if the
      WFSC image is changed from 24x24 to 72x72, the next client and server
      are setup with the proper parameters
  */

  strcpy( request, name);
  status = Server_StringLowerCase(request);

  if ( !strncmp( request, "spots", strlen(request)) ) {

    status = DClient_WriteString( WFSC_Info->server.fd, "Size", msg);
    if ( status ) {
      sprintf( errorMsg, "DClient_WFSC_Init: Could not initialize %s server", WFSC_Info->server.name);
      printf("%s\n", errorMsg);
      return(status);
    }

    WFSC_Info->server.nBytes = DClient_ReadLong( WFSC_Info->server.fd, WFSC_Info->server.name, msg);

  } else {

    WFSC_Info->server.nBytes = DClient_ReadLong( WFSC_Info->server.fd, "Size", msg);

  }

  if ( debug ) {
    printf("  DClient_WFSC_Init: nBytes = %d\n", WFSC_Info->server.nBytes);
  }

  if ( WFSC_Info->server.nBytes == 1152 ) {
    WFSC_Info->state.dimension[0] = 24;
    WFSC_Info->state.dimension[1] = 24;
  } else if ( WFSC_Info->server.nBytes == 10368 ) {
    WFSC_Info->state.dimension[0] = 72;
    WFSC_Info->state.dimension[1] = 72;
  } else {
    printf("  DClient_WFSC_Init: Unknown nBytes = %d\n", WFSC_Info->server.nBytes);
    printf("                     Expect 1152 or 10368\n");
    return(-1);
  }

  /*
    Check to see if memory has already been allocated.  If so, free it
  */
  if ( WFSC_Info->server.in_line != NULL) {
    if ( debug ) printf("  DClient_WFSC_Init: Freeing WFSC_Raw\n");
    free(WFSC_Info->server.in_line);
  }

  if (( WFSC_Info->server.in_line = (char *) malloc (WFSC_Info->server.nBytes) ) == NULL) {
    strcpy( errorMsg, "DClient_WFSC_Init: Error allocating WFSC_Raw");
    printf("  %s\n", errorMsg);
    return(-1);
  }
  memset( WFSC_Info->server.in_line, 0, WFSC_Info->server.nBytes );

  strcpy( WFSC_Info->server.in_line, "Hello\n");
 
  /*
    Check to see if memory has already been allocated.  If so, free it
  */
  if ( *WFSC_Array != NULL) {
    if ( debug ) printf("  DClient_WFSC_Init: Freeing WFSC_Array\n");
    free(*WFSC_Array);
  }

  if (( *WFSC_Array = (short *) malloc (WFSC_Info->server.nBytes) ) == NULL) {
    strcpy( errorMsg, "DClient_WFSC_Init: Error allocating WFSC_Array");
    printf("  %s\n", errorMsg);
    return(-1);
  }
  memset( *WFSC_Array, 0, WFSC_Info->server.nBytes );

  return(0);
}
