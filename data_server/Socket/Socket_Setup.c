/*===========================================================================*
**
**  Socket_Setup.c
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

#include "PCR.h"
#include "Server_Ports.h"
#include "Socket.h"

/*=========================================
 * function: Socket_Setup
 *=========================================*/

int Socket_Setup( Socket_Info *Info, char *name, int debug)
{

  /*
    Determine which address and port to use and put them in the structure
  */
  if ( !strncmp( name, "PCR_CMD", strlen(name)) ) {

    strcpy( Info->name, "PCR_CMD");
    strcpy( Info->address, PCR_ADDRESS);
    Info->port = PCR_CMD_PORT;
    
  } else if ( !strncmp( name, "PCR_WFSC", strlen(name)) ) {

    strcpy( Info->name, "PCR_WFSC");
    strcpy( Info->address, PCR_ADDRESS);
    Info->port = PCR_WFSC_PORT;
    
  } else if ( !strncmp( name, "PCR_DM", strlen(name)) ) {

    strcpy( Info->name, "PCR_DM");
    strcpy( Info->address, PCR_ADDRESS);
    Info->port = PCR_DM_PORT;
    
  } else if ( !strncmp( name, "PCR_Error", strlen(name)) ) {

    strcpy( Info->name, "PCR_Error");
    strcpy( Info->address, PCR_ADDRESS);
    Info->port = PCR_ERROR_PORT;
    
  } else if ( !strncmp( name, "Master", strlen(name)) ) {

    strcpy( Info->name, "DServ_Master");
    strcpy( Info->address, "localhost");
    Info->port = PCR_SERVER_CMD_PORT;
    
  } else if ( !strncmp( name, "WFSC", strlen(name)) ) {

    strcpy( Info->name, "DServ_WFSC");
    strcpy( Info->address, "localhost");
    Info->port = PCR_SERVER_WFSC_PORT;
    
  } else if ( !strncmp( name, "DM", strlen(name)) ) {

    strcpy( Info->name, "DServ_DM");
    strcpy( Info->address, "localhost");
    Info->port = PCR_SERVER_DM_PORT;

  } else if ( !strncmp( name, "Info", strlen(name)) ) {

    strcpy( Info->name, "Info Server");
    strcpy( Info->address, "localhost");
    Info->port = INFO_PORT;
    
  } else {

    printf(" Socket_Setup: name not found: %s\n", name);
    fflush(stdout);
    return(-1);

  }

  if ( debug ) {
    printf("  Socket_Setup: Server information\n");
    printf("                     Name = %s\n", Info->name);
    printf("                  Address = %s\n", Info->address);
    printf("                     Port = %d\n", Info->port);
    fflush(stdout);
  }

  Info->sockfd = -1;
  Info->connected = 0;
  Info->error = 0;

  return(0);
}
