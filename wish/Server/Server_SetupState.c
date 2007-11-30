/*===========================================================================*
**
**  Server_SetupState.c
**
**  State all the state values in the structure Info->state to zero and
**    create the TclTk variable names need to update shared varibles.
**
**  Returns :  0 success
**            -1 error
**
**  Written   22mar05   DLM
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "Client.h"
#include "Server.h"

#include "Server_Ports.h"

/*=========================================
 * function: Server_SetupState
 *=========================================*/

int Server_SetupState( Server_Info *Info, char *name, int debug)
{

  char request[STRING_LENGTH];
  int status;

  strcpy( Info->name, name);

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  strcpy( request, name);
  status = Server_StringLowerCase(request);

  /*
    Determine which address and port to use and put them in the structure
  */
  if ( !strncmp( request, "wfsc", strlen("wfsc")) ) {

    strcpy( Info->address, PCR_SERVER_ADDRESS);
    Info->port = PCR_SERVER_WFSC_PORT;
    
  } else if ( !strncmp( request, "spots", strlen("spots")) ||
	      !strncmp( request, "slopes", strlen("slopes")) ||
	      !strncmp( request, "rtr", strlen("rtr")) ||
	      !strncmp( request, "cmd", strlen("cmd")) ||
	      !strncmp( request, "cur", strlen("cur")) ||
	      !strncmp( request, "pos", strlen("pos")) ) {

    strcpy( Info->address, PCR_SERVER_ADDRESS);
    Info->port = PCR_SERVER_DM_PORT;
    
  } else if ( !strncmp( request, "dserv", strlen("dserv")) ) {

    strcpy( Info->address, PCR_SERVER_ADDRESS);
    Info->port = PCR_SERVER_DM_PORT;
    
  } else if ( !strncmp( request, "pcr_server", strlen("pcr_server")) ) {

    strcpy( Info->address, PCR_SERVER_ADDRESS);
    Info->port = PCR_SERVER_CMD_PORT;

  } else if ( !strncmp( request, "pcr_cmd", strlen("pcr_cmd")) ) {

    strcpy( Info->address, PCR_ADDRESS);
    Info->port = PCR_CMD_PORT;
    
  } else {

    printf(" Server_SetupState: Request not found: %s\n", name);
    return(-1);

  }

  if ( debug ) {
    printf("  Server_SetupState: Server information\n");
    printf("                     Name = %s\n", Info->name);
    printf("                  Address = %s\n", Info->address);
    printf("                     Port = %d\n", Info->port);
  }

  return(0);
}
