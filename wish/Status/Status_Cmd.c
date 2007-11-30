/*===========================================================================*
**
**  Status_Cmd.c
**
**  In this section, new TclTk procedures written in C are created.
**  ---------------------------------------------------------------
**
**  All new procedures must be define above in Tcl_AppInit as well as
**    declared in the headed (see List of new Tcl Commands section).
**
**  Written  30aug07  DLM  Started with Info_Cmd.c
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "Status.h"
#include "Server_Ports.h"

#include "Client_Globals.h"
#include "Client_Prototypes.h"

extern char *reply_data;


/*===========================================================================*
 * Status_Cmd
 *===========================================================================*/

int Status_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
{

  /* Variables sent by TclTk and passed on to the subroutines */
  char name[STRING_LENGTH];
  char variable[STRING_LENGTH];
  char value[STRING_LENGTH];
  char request[STRING_LENGTH];

  /* Character string used to send information back from called routines.
       This information is passed back to TckTk via the linked variable
       reply_data.
  */
  char errorMsg[IO_LENGTH];

  /* Server variables */
  static Client_Info Status_Info;
  static Gui_Variables Status_GuiVariables;
  static int first_time = 1;

  /* Local variables */
  int required_args;
  int status, i;
  int debug_local = 0;

/*************************************************************************
**
**  Check that we have at least one parameter.  This is the request and
**    is copied into name[].
**
**************************************************************************/

  Tcl_ResetResult(interp);

  required_args = 1;

  if (argc < 1+required_args ) {
    status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				     argc-2, reply_data, debug_Status);
    if ( debug_Status ) {
        printf("%s\n", reply_data);
    }
    return TCL_ERROR;
  }
  strcpy( name, argv[1]);

  strcpy( reply_data, "OK: Status_Cmd");
  strcpy( errorMsg, "");

/**************************************************************************
**
**  Initialize socket information structures
**
**************************************************************************/

  if ( first_time ) {

    /*
      Setup Status Client Info structure and Gui variables
    */

    status = Client_SetupInfo( &Status_Info, &Status_GuiVariables, "Status",
			       &Status_Connected, &Status_Error);

    first_time = 0;

    status = Client_GuiUpdate( interp, &Status_Info, &Status_GuiVariables );

  }

/**************************************************************************
**
**  Send request to Data Server on the VME
**
**************************************************************************/

  if ( debug_Status ) {
    printf("  Status_Cmd: request = %s\n", name);
  }

  /*
    Check that a request was sent
  */
  if ( strlen(name) <= 1 ) {
    printf("  Status_Cmd: Warning: request of zero length recieved\n");
    return(-1);
  }

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  strcpy( request, name);
  status = Server_StringLowerCase(request);

  /*
    All commands require a connection to the DServ server, so every time
      Status_Cmd is called, we will check to make sure we are connected, or
      will initiate this connection
  */
  if ( !Status_Info.connected ) {

    status = Client_Open( &Status_Info, debug_Status);
    if ( status ) {
      sprintf( errorMsg, "Error connecting to %s", Status_Info.name);
      status = Client_GuiUpdate( interp, &Status_Info, &Status_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				    reply_data, debug_Status);
      return TCL_ERROR;
    }

    if ( debug_Status ) {
      printf("  Status_Cmd: Connected to Topbox_Server\n");
    }
  }

  if ( !strncmp( request, "connect", strlen("connect") ) ) {

    /********************************************************
     *  This is already done above, but in case the user send the
     *    connect request, the stub is here
     ********************************************************/

  } else if ( !strncmp( request, "close", strlen("close") ) ) {

    /********************************************************
     *  Close a connection to the Status Server
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				      argc-2, reply_data, debug_Status);
      return TCL_ERROR;
    }

    if ( !Status_Info.connected ) {
      return TCL_OK;
    }

    status = Client_Close( &Status_Info, debug_Status);
    if ( status ) {
      sprintf( errorMsg, "Error closing connection to %s", Status_Info.name);
      status = Client_GuiUpdate( interp, &Status_Info, &Status_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				    reply_data, debug_Status);
      return TCL_ERROR;
    }

  } else if ( !strncmp( request, "send", strlen("send") ) ) {
    
    /********************************************************
     *  Send a request to the Status Server
     ********************************************************/

    printf("  argc = %d\n", argc);

    required_args = 2;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Status);
      return TCL_ERROR;
    }

    i=2;
    strcpy( variable, argv[i++]);
    strcpy( value, argv[i++]);
    sprintf( request, "Set\n%s\n%s\n", variable, value);
    if ( debug_Status ) {
      printf("%s\n", request);
    }

    status = Client_Write( Status_Info.fd, request, strlen(request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Status);
      return TCL_ERROR;
    }

  } else if ( !strncmp( request, "all", strlen("all") ) ) {

    /********************************************************
     *  Get variable from Status Server on VME
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Status);
      return TCL_ERROR;
    }

    i=2;
    strcpy( request, "all\n");

    status = Client_Write( Status_Info.fd, request, strlen(request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Status);
      return TCL_ERROR;
    }

    status = Client_ReadToEOF( &Status_Info, Status_Reply, errorMsg, debug_local);
    if ( status ) {
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Status);
      return TCL_ERROR;
    }

/**************************************************************************
**
**  Unknown call and cleanup
**
**************************************************************************/

  } else {

    sprintf( errorMsg, "Unknown request: %s", argv[1]);
    status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				   reply_data, debug_Status);
    return TCL_ERROR;

  }

  Tcl_AppendResult(interp,"OK", NULL);
  strcpy(reply_data, "OK Fine");

  status = Client_GuiUpdate( interp, &Status_Info, &Status_GuiVariables);
  return TCL_OK;
}
