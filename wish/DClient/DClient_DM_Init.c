/*
  DClient_DM_Init.c

  Routine to request open a socket to the DClient server on the VME, start
    a DServ server  and allocate the memory needed to transfer and manipulate
    the requested data.

  An actual DServ frame is not retrieved here

  Written   6apr05  DLM  Started with DClient_WFSC_Init.c

  Altered  16mar06  DLM  Started with DClient_Slopes_Init.c.  DServ_Array was changed to
                           char because DServ data is short and long.

  Altered  21jul06  DLM  Started with DClient_DM_Init.c

  Altered  22aug06  DLM  Added slope to list of DM data served
*/

#include "PCR_Wish.h"
#include "DClient.h"

int DClient_DM_Init( DClient_Info *DM_Info, char *name,
		       char **DM_Array, int debug, char *errorMsg)
{

  int status;
  char msg[STRING_LENGTH];
  char request[SERVER_LENGTH];
  debug = 1;
  /*
    Setup Server information in structure, if not already connected
  */
  if ( !DM_Info->server.connected ) {

    status = Server_SetupState( (Server_Info *)DM_Info, name, debug);
    if ( status ) {
      strcpy( errorMsg, "DClient_DM_Init: Error in Server_SetupState");
      printf("%s\n", errorMsg);
      return(status);
    }

    status = Server_Open( (Server_Info *)DM_Info, debug);
    if ( status == -12 ) {
      sprintf( errorMsg, "DClient_DM_Init: Could not connect to %s server: %s",
	       DM_Info->server.name, DM_Info->server.name);
      printf("%s\n", errorMsg);
      return(status);
    } else if ( status ) {
      sprintf( errorMsg, "DClient_DM_Init: Could not connect to %s server: %s",
	       DM_Info->server.name, DM_Info->server.name);
      printf("%s\n", errorMsg);
      return(status);
    }
  }

  /*
    Send the DServ server the name of data which will be requested.  This can be
      1) Slope
      2) Pos
      3) Cmd
      4) Cur
      5) RTR
      6) All

    When information about the DServ vector has returned from the DServ server,
      local arrays and parameters are set.  These setting do not change while
      the DServ data is being retrieved by this process.

      **NOTE: the call to Size and type should be switched  23jul06 DLM
  */

  status = DClient_WriteString( DM_Info->server.fd, "Size", msg);
  if ( status ) {
    sprintf( errorMsg, "DClient_DM_Init: Could not initialize %s server", DM_Info->server.name);
    printf("%s\n", errorMsg);
    return(status);
  }

  DM_Info->server.nBytes = DClient_ReadLong( DM_Info->server.fd, DM_Info->server.name, msg);

  if ( debug ) {
    printf("  DClient_DM_Init: nBytes = %d\n", DM_Info->server.nBytes);
  }

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  strcpy( request, name);
  status = Server_StringLowerCase(request);

  if ( !strncmp( request, "slope", strlen(request)) ) {
    if ( DM_Info->server.nBytes != 576 ) {
      printf("  DClient_DM_Init: Unknown nBytes = %d\n", DM_Info->server.nBytes);
      printf("                     Expected 576\n");
      return(-1);
    }
    DM_Info->frame.dims = 1;
    DM_Info->frame.length[0] = 288;
  } else if ( !strncmp( request, "rtr", strlen(request)) ) {
    if ( DM_Info->server.nBytes != 672 ) {
      printf("  DClient_DM_Init: Unknown nBytes = %d\n", DM_Info->server.nBytes);
      printf("                     Expected 672\n");
      return(-1);
    }
    DM_Info->frame.dims = 1;
    DM_Info->frame.length[0] = 336;
  } else if ( !strncmp( request, "pos", strlen(request)) ) {
    if ( DM_Info->server.nBytes != 672 ) {
      printf("  DClient_DM_Init: Unknown nBytes = %d\n", DM_Info->server.nBytes);
      printf("                     Expected 672\n");
      return(-1);
    }
    DM_Info->frame.dims = 1;
    DM_Info->frame.length[0] = 336;
  } else if ( !strncmp( request, "cmd", strlen(request)) ) {
    if ( DM_Info->server.nBytes != 672 ) {
      printf("  DClient_DM_Init: Unknown nBytes = %d\n", DM_Info->server.nBytes);
      printf("                     Expected 672\n");
      return(-1);
    }
    DM_Info->frame.dims = 1;
    DM_Info->frame.length[0] = 336;
  } else if ( !strncmp( request, "cur", strlen(request)) ) {
    if ( DM_Info->server.nBytes != 672 ) {
      printf("  DClient_DM_Init: Unknown nBytes = %d\n", DM_Info->server.nBytes);
      printf("                     Expected 672\n");
      return(-1);
    }
    DM_Info->frame.dims = 1;
    DM_Info->frame.length[0] = 336;
  } else {
    printf("  DClient_DM_Init: Unknown request = %s\n", request);
    return(-1);
  }

  /*
    Check to see if memory has already been allocated.  If so, free it
  */
  if ( DM_Info->server.in_line != NULL) {
    if ( debug ) printf("  DClient_DM_Init: Freeing %s input array\n", DM_Info->server.name);
    free(DM_Info->server.in_line);
  }

  if (( DM_Info->server.in_line = (char *) malloc (DM_Info->server.nBytes) ) == NULL) {
    sprintf( errorMsg, "DClient_DM_Init: Error allocating %s input array", DM_Info->server.name);
    printf("%s\n", errorMsg);
    return(-1);
  }
  memset( DM_Info->server.in_line, 0, DM_Info->server.nBytes );

  strcpy( DM_Info->server.in_line, "Hello\n");
 
  /*
    Check to see if memory has already been allocated.  If so, free it
  */
  if ( *DM_Array != NULL) {
    if ( debug ) printf("  DClient_DM_Init: Freeing %s Array\n", DM_Info->server.name);
    free(*DM_Array);
  }

  if (( *DM_Array = (char *) malloc (DM_Info->server.nBytes) ) == NULL) {
    sprintf( errorMsg, "DClient_DM_Init: Error allocating %s Array", DM_Info->server.name);
    printf("%s\n", errorMsg);
    return(-1);
  }
  memset( *DM_Array, 0, DM_Info->server.nBytes );

  return(0);
}
