/*===========================================================================*
**
**  TCS_Cmd.c
**
**  In this section, new TclTk procedures written in C are created.
**  ---------------------------------------------------------------
**
**  All new procedures must be define above in Tcl_AppInit as well as
**    declared in the headed (see List of new Tcl Commands section).
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "Client.h"
#include "Server.h"
#include "TCS.h"

#include "Client_Globals.h"
#include "Client_Prototypes.h"

extern char *reply_data;

/*===========================================================================*
 * TCS_Cmd
 *===========================================================================*/

int TCS_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
{

  /* Variables sent by TclTk and passed on to the subroutines */
  char name[STRING_LENGTH];
  char request[STRING_LENGTH];

  /* Character string used to send information back from called routines.
       This information is passed back to TckTk via the linked variable
       reply_data.
  */
  char errorMsg[IO_LENGTH];

  /* Server variables */
  static Client_Info TCS_Info;
  static Gui_Variables TCS_GuiVariables;
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
				     argc-2, reply_data, debug_TCS);
    if ( debug_TCS ) {
        printf("%s\n", reply_data);
    }
    return TCL_ERROR;
  }
  strcpy( name, argv[1]);

  strcpy( reply_data, "OK: TCS_Cmd");
  strcpy( errorMsg, "");

/**************************************************************************
**
**  Initialize socket information structures
**
**************************************************************************/

  if ( first_time ) {

    /*
      Setup Info Client Info structure and Gui variables
    */

    status = Client_SetupInfo( &TCS_Info, &TCS_GuiVariables, "TCS",
			       &TCS_Connected, &TCS_Error);

    first_time = 0;

    status = Client_GuiUpdate( interp, &TCS_Info, &TCS_GuiVariables );

  }
  
  TCS_Reply[0] = 0;

/**************************************************************************
**
**  Send request to Data Server on the VME
**
**************************************************************************/

  if ( !strncmp( name, "Connect", strlen("Connect") ) ) {

    /********************************************************
     *  Open a connection to the TCS
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_TCS);
      return TCL_ERROR;
    }

    status = Client_Open( &TCS_Info, debug_TCS);
    if ( status ) {
      sprintf( errorMsg, "%s%s%s",
	       "Check that ", TCS_Info.name, " is running");
      strcpy( reply_data, errorMsg);
      if ( debug_TCS ) printf("  %s\n", errorMsg);
      status = Client_GuiUpdate( interp, &TCS_Info, &TCS_GuiVariables);
      return TCL_ERROR;
    }

  } else if ( !strncmp( name, "Close", strlen("Close") ) ) {

    /********************************************************
     *  Close a connection to TCS
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_TCS);
      return TCL_ERROR;
    }

    if ( !TCS_Info.connected ) {
      return TCL_OK;
    }

    status = Client_Close( &TCS_Info, debug_TCS);
    if ( status ) {
      sprintf( errorMsg, "Error closing connection to %s", TCS_Info.name);
      status = Client_GuiUpdate( interp, &TCS_Info, &TCS_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TCS);
      return TCL_ERROR;
    }

  } else if ( !strncmp( name, "Set", strlen("Set") ) ) {

    /********************************************************
     *  Set variable on Info Server on VME
     ********************************************************/

    required_args = 1;

    if (argc < 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_TCS);
      return TCL_ERROR;
    }

    i=2;
    sprintf( request, "%s\n", argv[i]);

    if ( debug_TCS ) {
      printf("  TCS_Cmd: request = %s", request);
    }

    if ( !TCS_Info.connected ) {
      sprintf( errorMsg, "Not connected to TCS Server");
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TCS);
      return TCL_ERROR;
    }

    status = Client_Write( TCS_Info.fd, request, strlen( request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_GuiUpdate( interp, &TCS_Info, &TCS_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TCS);
      return TCL_ERROR;
    }

    status = Client_ReadToEOF( &TCS_Info, TCS_Reply, errorMsg, debug_local);
    if ( status ) {
      status = Client_GuiUpdate( interp, &TCS_Info, &TCS_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TCS);
      return TCL_ERROR;
    }

    if ( debug_TCS ) {
      printf("            reply = %s", TCS_Reply);
    }

  } else if ( !strncmp( name, "Get", strlen("Get") ) ) {

    /********************************************************
     *  Get variable from Info Server on VME
     ********************************************************/

    required_args = 1;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_TCS);
      return TCL_ERROR;
    }

    i=2;
    sprintf( request, "get %s\n", argv[i]);

    if ( debug_TCS ) {
      printf("  TCS_Cmd: request = %s", request);
    }

    if ( !TCS_Info.connected ) {
      sprintf( errorMsg, "Not connected to TCS Server");
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TCS);
      return TCL_ERROR;
    }

    status = Client_Write( TCS_Info.fd, request, strlen( request));
    if ( status ) {
      sprintf( errorMsg, "Error writting %s", request);
      status = Client_GuiUpdate( interp, &TCS_Info, &TCS_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TCS);
      return TCL_ERROR;
    }

    status = Client_ReadToEOF( &TCS_Info, TCS_Reply, errorMsg, debug_local);
    if ( status ) {
      status = Client_GuiUpdate( interp, &TCS_Info, &TCS_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TCS);
      return TCL_ERROR;
    }

    if ( debug_TCS ) {
      printf("             reply = %s", TCS_Reply);
    }

  } else if ( !strncmp( name, "All", strlen("All") ) ) {

    /********************************************************
     *  Get variable from TCS Server on VME
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_TCS);
      return TCL_ERROR;
    }

    i=2;
    strcpy( request, "all\n");

    if ( debug_TCS ) {
      printf("  TCS_Cmd: request = %s", request);
    }

    if ( !TCS_Info.connected ) {
      sprintf( errorMsg, "Not connected to TCS Server");
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TCS);
      return TCL_ERROR;
    }

    status = Client_Write( TCS_Info.fd, request, strlen( request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_GuiUpdate( interp, &TCS_Info, &TCS_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TCS);
      return TCL_ERROR;
    }

    status = Client_ReadToEOF( &TCS_Info, TCS_Reply, errorMsg, debug_local);
    if ( status ) {
      status = Client_GuiUpdate( interp, &TCS_Info, &TCS_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TCS);
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
				   reply_data, debug_TCS);
    return TCL_ERROR;

  }

  Tcl_AppendResult(interp,"OK", NULL);
  strcpy(reply_data, "OK Fine in TCS_Cmd");

  status = Client_GuiUpdate( interp, &TCS_Info, &TCS_GuiVariables);
  return TCL_OK;
}
