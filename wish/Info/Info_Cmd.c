/*===========================================================================*
**
**  Info_Cmd.c
**
**  In this section, new TclTk procedures written in C are created.
**  ---------------------------------------------------------------
**
**  All new procedures must be define above in Tcl_AppInit as well as
**    declared in the headed (see List of new Tcl Commands section).
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "Info.h"
#include "Server_Ports.h"

#include "Client_Globals.h"
#include "Client_Prototypes.h"

extern char *reply_data;

/*===========================================================================*
 * Info_Cmd
 *===========================================================================*/

int Info_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
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
  static Client_Info Info_Info;
  static Gui_Variables Info_GuiVariables;
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
				     argc-2, reply_data, debug_Info);
    if ( debug_Info ) {
        printf("%s\n", reply_data);
    }
    return TCL_ERROR;
  }
  strcpy( name, argv[1]);

  strcpy( reply_data, "OK: Info_Cmd");
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

    status = Client_SetupInfo( &Info_Info, &Info_GuiVariables, "Info",
			       &Info_Connected, &Info_Error);

    first_time = 0;

    status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables );

  }

  if ( debug_Info ) {
    printf("  Info_Cmd: request = %s\n", name);
  }

/**************************************************************************
**
**  Send request to Data Server on the VME
**
**************************************************************************/

  if ( !strncmp( name, "Connect", strlen("Connect") ) ) {

    /********************************************************
     *  Open a connection to the Info Server on the VME
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Info);
      return TCL_ERROR;
    }

    status = Client_Open( &Info_Info, debug_Info);
    if ( status ) {
      sprintf( errorMsg, "Error connecting to %s", Info_Info.name);
      status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

  } else if ( !strncmp( name, "Close", strlen("Close") ) ) {

    /********************************************************
     *  Close a connection to the Info Server on the VME
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Info);
      return TCL_ERROR;
    }

    if ( !Info_Info.connected ) {
      return TCL_OK;
    }

    status = Client_Close( &Info_Info, debug_Info);
    if ( status ) {
      sprintf( errorMsg, "Error closing connection to %s", Info_Info.name);
      status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

  } else if ( !strncmp( name, "Debug", strlen("Debug") ) ) {

    /********************************************************
     *  Close a connection to the Info Server on the VME
     ********************************************************/

    required_args = 1;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Info);
      return TCL_ERROR;
    }

    i=2;
    strcpy( variable, argv[i++]);
    sprintf( request, "Debug\n %s\n", variable);

    if ( !Info_Info.connected ) {
      sprintf( errorMsg, "Not connected to Info Server");
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

    status = Client_Write( Info_Info.fd, request, strlen(request));
    if ( status ) {
      sprintf( errorMsg, "Error quiting connection to %s", Info_Info.name);
      status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

  } else if ( !strncmp( name, "Set", strlen("Set") ) ) {

    /********************************************************
     *  Set variable on Info Server on VME
     ********************************************************/

    required_args = 2;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Info);
      return TCL_ERROR;
    }

    i=2;
    strcpy( variable, argv[i++]);
    strcpy( value, argv[i++]);
    sprintf( request, "Set\n%s\n%s\n", variable, value);
    if ( debug_Info ) {
      printf("%s\n", request);
    }

    if ( !Info_Info.connected ) {
      sprintf( errorMsg, "Not connected to Info Server");
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

    status = Client_Write( Info_Info.fd, request, strlen( request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

  } else if ( !strncmp( name, "Get", strlen("Get") ) ) {

    /********************************************************
     *  Get variable from Info Server on VME
     ********************************************************/

    required_args = 1;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Info);
      return TCL_ERROR;
    }

    i=2;
    strcpy( variable, argv[i++]);
    sprintf( request, "Get\n %s\n", variable);
    if ( debug_Info ) {
      printf("%s\n", request);
    }

    if ( !Info_Info.connected ) {
      sprintf( errorMsg, "Not connected to Info Server");
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

    status = Client_Write( Info_Info.fd, request, strlen( request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

    status = Client_ReadLine( Info_Info.fd, Info_Reply, CLIENT_REPLY_LENGTH);
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

    if ( debug_Info ) {
      printf("          %s", Info_Reply);
    }

  } else if ( !strncmp( name, "All", strlen("All") ) ) {

    /********************************************************
     *  Get variable from Info Server on VME
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Info);
      return TCL_ERROR;
    }

    i=2;
    strcpy( request, "all\n");

    if ( !Info_Info.connected ) {
      sprintf( errorMsg, "Not connected to Info Server");
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

    status = Client_Write( Info_Info.fd, request, strlen( request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

    status = Client_ReadToEOF( &Info_Info, Info_Reply, errorMsg, debug_local);
    if ( status ) {
      status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

  } else if ( !strncmp( name, "See_Vals", strlen("See_Vals") ) ) {

    /********************************************************
     *  Get variable from Info Server on VME
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Info);
      return TCL_ERROR;
    }

    i=2;
    strcpy( request, "see_vals\n");

    if ( !Info_Info.connected ) {
      sprintf( errorMsg, "Not connected to Info Server");
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

    status = Client_Write( Info_Info.fd, request, strlen( request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

    status = Client_ReadToEOF( &Info_Info, Info_Reply, errorMsg, debug_local);
    if ( status ) {
      status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

    status = Info_AverageSeeing( Info_Reply, &Seeing_Average, errorMsg, debug_Info);
    if ( status ) {
      sprintf( errorMsg, "Error calculating average seeing");
      status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }
    Tcl_UpdateLinkedVar (interp, "Seeing_Average");

  } else if ( !strncmp( name, "Reset_Seeing", strlen("Reset_Seeing") ) ) {

    /********************************************************
     *  Close a connection to the Info Server on the VME
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Info);
      return TCL_ERROR;
    }

    sprintf( request, "reset_seeing\n");

    if ( !Info_Info.connected ) {
      sprintf( errorMsg, "Not connected to Info Server");
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
      return TCL_ERROR;
    }

    status = Client_Write( Info_Info.fd, request, strlen(request));
    if ( status ) {
      sprintf( errorMsg, "Error quiting connection to %s", Info_Info.name);
      status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Info);
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
				   reply_data, debug_Info);
    return TCL_ERROR;

  }

  Tcl_AppendResult(interp,"OK", NULL);
  strcpy(reply_data, "OK Fine");

  status = Client_GuiUpdate( interp, &Info_Info, &Info_GuiVariables);
  return TCL_OK;
}
