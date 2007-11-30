/*
  DClient_Slopes_Init.c

  Routine to request open a socket to the data server on the PCR_Server, start
    the Slope server and allocate the memory
    needed to transfer and manipulate the requested data.

  An actual Slopes frame is not retrieved here

  Written  23mar05  DLM  Combined DClient_WFSC_Single, DClient_WFSC_Init,
                           and added Server_SetupState and Server_Open to make
			   the one routine.

			 This can be called by either the Single or the Save routines

  Altered  13jun07  DLM  Started with DClient_Spots_Init.c

*/

#include "PCR_Wish.h"
#include "DClient.h"
#include "WFSC.h"

int DClient_Slopes_Init( DClient_Info *Client_Info, PGplot_Info *Slopes_Info, char *name,
			short **Slopes_Vector, int debug, char *errorMsg)
{

  int status;
  char msg[STRING_LENGTH];
  char request[STRING_LENGTH];

  int i;

  /*
    Setup Server information in structure, if not already connected
  */
  if ( !Client_Info->server.connected ) {

    status = Server_SetupState( (Server_Info *)Client_Info, name, debug);
    if ( status ) {
      strcpy( errorMsg, "DClient_Slopes_Init: Error in Server_SetupState");
      if ( debug ) printf("  %s\n", errorMsg);
      return(status);
    }

    status = Server_Open( (Server_Info *)Client_Info, debug);
    if ( status == -12 ) {
      sprintf( errorMsg, "DClient_Slopes_Init: Could not connect to %s server", Client_Info->server.name);
      if ( debug ) printf("  %s\n", errorMsg);
      return(status);
    } else if ( status ) {
      sprintf( errorMsg, "DClient_Slopes_Init: Error connecting to %s server", Client_Info->server.name);
      if ( debug ) printf("  %s\n", errorMsg);
      return(status);
    }

  }

  /*
    When information about the Spot array has returned from the WFC server,
      local arrays and parameters are set.  These setting do not change while
      spot data is being retrieved by this process.  In this way, if the
      Slopes image is changed from 24x24 to 72x72, the next client and server
      are setup with the proper parameters
  */

  strcpy( request, name);
  status = Server_StringLowerCase(request);

  status = DClient_WriteString( Client_Info->server.fd, "Size", msg);
  if ( status ) {
    sprintf( errorMsg, "DClient_Slopes_Init: Could not initialize %s server", Client_Info->server.name);
    printf("%s\n", errorMsg);
    return(status);
  }

  Client_Info->server.nBytes = DClient_ReadLong( Client_Info->server.fd, Client_Info->server.name, msg);

  if ( debug ) {
    printf("  DClient_Slopes_Init: nBytes = %d\n", Client_Info->server.nBytes);
  }

  /*
    Check that the number of bytes is equal to the SUB_APPS*SUB_APPS (number of slopes) * 2 (x and y directions)
    * 2 (number of bytes per short integer)
  */
  if ( Client_Info->server.nBytes == SUB_APPS*SUB_APPS*2*2 ) {
    Slopes_Info->slopes->values = SUB_APPS*SUB_APPS;
  } else {
    printf("  DClient_Slopes_Init: Unknown nBytes = %d\n", Client_Info->server.nBytes);
    printf("                               Expected %d\n", SUB_APPS*SUB_APPS*2*2 );
    return(-1);
  }

  /*
    Check to see if memory has already been allocated.  If so, free it
  */
  if ( Client_Info->server.in_line != NULL) {
    if ( debug ) printf("  DClient_Slopes_Init: Freeing Client_Info->server.in_line\n");
    free(Client_Info->server.in_line);
  }

  if (( Client_Info->server.in_line = (char *) malloc (Client_Info->server.nBytes) ) == NULL) {
    strcpy( errorMsg, "DClient_Slopes_Init: Error allocating Client_Info->server.in_line");
    printf("  %s\n", errorMsg);
    return(-1);
  }
  memset( Client_Info->server.in_line, 0, Client_Info->server.nBytes );

  strcpy( Client_Info->server.in_line, "Hello\n");
 
  /*
    Check to see if memory has already been allocated.  If so, free it
  */
  if ( *Slopes_Vector != NULL) {
    if ( debug ) printf("  DClient_Slopes_Init: Freeing Slopes_Vector\n");
    free(*Slopes_Vector);
  }

  if (( *Slopes_Vector = (short *) malloc (Client_Info->server.nBytes) ) == NULL) {
    strcpy( errorMsg, "DClient_Slopes_Init: Error allocating Slopes_Vector");
    printf("  %s\n", errorMsg);
    return(-1);
  }
  memset( *Slopes_Vector, 0, Client_Info->server.nBytes );

  /*
    Allocate float arrays needed by the PGplot routines for both x and y
  */
  if ( debug ) printf("  DClient_Slopes_Init: Allocating Slopes_Info->slopes->current_x\n");
  if ( Slopes_Info->slopes->current_x != NULL ) {
    free(Slopes_Info->slopes->current_x);
    printf("  DClient_Slopes_Init: Slopes_Info->slopes->current memory freed\n");
  }

  if (( Slopes_Info->slopes->current_x = (float *)malloc(Slopes_Info->slopes->values*2*sizeof(float)) ) == NULL ) {
    printf("  DClient_Slopes_Init: Error allocating Slopes_Info->slopes->current_x\n");
    return(-1);
  }
  memset( Slopes_Info->slopes->current_x, 0, Slopes_Info->slopes->values*2*sizeof(float) );
  /*
    Set pointer for y values to the correct position in x vector.  In this way when the x and y vectors are
      received from the PCR_Server, we do not have to split the vector, just put it in memory starting at the
      start of the x vector
  */
  Slopes_Info->slopes->current_y = Slopes_Info->slopes->current_x + Slopes_Info->slopes->values;

  /*
    Array for the running average
  */
  if ( debug ) printf("  DClient_Slopes_Init: Allocating Slopes_Info->slopes->average_x\n");
  if ( Slopes_Info->slopes->average_x != NULL ) {
    free(Slopes_Info->slopes->average_x);
    printf("  DClient_Slopes_Init: Slopes_Info->slopes->average memory freed\n");
  }

  if (( Slopes_Info->slopes->average_x = (float *)malloc(Slopes_Info->slopes->values*2*sizeof(float)) ) == NULL ) {
    printf("  DClient_Slopes_Init: Error allocating Slopes_Info->slopes->average_x\n");
    return(-1);
  }
  memset( Slopes_Info->slopes->average_x, 0, Slopes_Info->slopes->values*2*sizeof(float) );
  /*
    Set pointer for y values to the correct position in x vector.  In this way when the x and y vectors are
      received from the PCR_Server, we do not have to split the vector, just put it in memory starting at the
      start of the x vector
  */
  Slopes_Info->slopes->average_y = Slopes_Info->slopes->average_x + Slopes_Info->slopes->values;

  /*
    Fractional values of current frame to add to the running average.  Here we
      set it to 1.0 so the first time through the runnning average is the current frame.
    This value is reset to a reasonable value (~0.1) in PGplot_SlopesCalcualte()
  */
  Slopes_Info->slopes->average_fraction = 1.0;

  /*
    Determine the number of sub-apperatures used.  Needed to find the average tip/tilt
  */
  Slopes_Info->slopes->used_subapps = 0.0;
  for ( i=0; i<SUB_APPS*SUB_APPS; i++) {
    Slopes_Info->slopes->used_subapps += (subapp_used[i] ? 1.0:0.0);
  }

  return(0);
}
