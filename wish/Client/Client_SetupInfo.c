/*===========================================================================*
**
**  Client_SetupInfo.c
**
**  State all the state values in the structure Info->state to zero and
**    create the TclTk variable names need to update shared varibles.
**
**  Returns :  0 success
**            -1 error
**
**  Written   12apr04  DLM  
**
**
**  Altered  29dec04  DLM  Changed to use FSM_Info instead of Motor_Info.
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "Server_Ports.h"
#include "Client.h"

/*=================================================
 * Client_SetupInfo: Setup Info and Gui variables
 *=================================================*/
int Client_SetupInfo( Client_Info *Info, Gui_Variables *Gui, char *name,
		      int *connected, int *error)
{

  /*
    Determine which address and port to use and put them in the structure
  */
  if ( !strncmp( name, "Hex", strlen(name)) ) {

    strcpy( Info->address, HEXAPOD_ADDRESS);
    Info->port = HEXAPOD_PORT;

  } else if ( !strncmp( name, "Info", strlen(name)) ) {

    strcpy( Info->address, PCR_SERVER_ADDRESS);
    Info->port = INFO_PORT;

  } else if ( !strncmp( name, "TCS", strlen(name)) ) {

    strcpy( Info->address, TCS_ADDRESS);
    Info->port = TCS_PORT;

  } else if ( !strncmp( name, "PCR", strlen(name)) ) {

    strcpy( Info->address, PCR_SERVER_ADDRESS);
    Info->port = PCR_SERVER_CMD_PORT;

  } else if ( !strncmp( name, "Topbox", strlen(name)) ) {

    strcpy( Info->address, TOPBOX_SERVER_ADDRESS);
    Info->port = TOPBOX_MASTER_PORT;

  } else if ( !strncmp( name, "Status", strlen(name)) ) {

    strcpy( Info->address, TOPBOX_SERVER_ADDRESS);
    Info->port = STATUS_PORT;

  } else if ( !strncmp( name, "Science", strlen(name)) ) {

    strcpy( Info->address, SCIENCE_SERVER_ADDRESS);
    Info->port = SCIENCE_SERVER_PORT;

  } else if ( !strncmp( name, "TSS", strlen(name)) ) {

    strcpy( Info->address, TSS_SERVER_ADDRESS);
    Info->port = TSS_SERVER_PORT;

  } else {

    printf("  Client_SetupInfo: name not found: %s\n", name);
    return(-1);

  }

  strcpy( Info->name, name);
  Info->connected = 0;
  Info->error = 0;
  Info->fd = 0;

  /*
    Setup connection to gui varibles
  */
  sprintf( Gui->connected.name, "%s_Connected", name);
  sprintf( Gui->error.name, "%s_Error", name);

  Gui->connected.ptr = connected;
  Gui->error.ptr = error;

  return(0);
}
