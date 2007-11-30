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
#include "Hex.h"

#include "Client_Globals.h"
#include "Client_Prototypes.h"

extern char *reply_data;

/*===========================================================================*
 * Hex_Cmd
 *===========================================================================*/

int Hex_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
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
  static Client_Info Hex_Info;
  static Gui_Variables Hex_GuiVariables;
  static int first_time = 1;

  /* Local variables */
  int required_args;
  int status, i;
  int debug_local = 0;
  int delta_z;

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
				     argc-2, reply_data, debug_Hex);
    if ( debug_Hex ) {
        printf("%s\n", reply_data);
    }
    return TCL_ERROR;
  }
  strcpy( name, argv[1]);

  strcpy( reply_data, "OK: Hex_Cmd");
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

    status = Client_SetupInfo( &Hex_Info, &Hex_GuiVariables, "Hex",
			       &Hex_Connected, &Hex_Error);

    first_time = 0;

    status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables );

  }
  
  Hex_Reply[0] = 0;

/**************************************************************************
**
**  Unlike almost all other client connections (pcr_server, topbox, TCS, etc)
**    we will connect and disconnect to the Hexapod server for every request
**    sent.  This is because the Hexapod server closes the connection after
**    a few seconds of inactivity
**
**************************************************************************/

  if ( Hex_Info.connected ) {

    status = Client_Close( &Hex_Info, debug_Hex);
    if ( status ) {
      sprintf( errorMsg, "Error closing connection to %s", Hex_Info.name);
      status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				    reply_data, debug_Hex);
      return TCL_ERROR;
    }
  }

  /********************************************************
   *  Open a connection to the Hex
   ********************************************************/

  status = Client_Open( &Hex_Info, debug_Hex);
  if ( status ) {
    sprintf( errorMsg, "%s%s%s",
	     "Check that ", Hex_Info.name, " is running");
    strcpy( reply_data, errorMsg);
    if ( debug_Hex ) printf("  %s\n", errorMsg);
    status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
    return TCL_ERROR;
  }

  if ( !strncmp( name, "Focus", strlen("Focus") ) ) {

    /********************************************************
     *  Send a focus request to the Hexapod
     ********************************************************/

    required_args = 1;

    if (argc < 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Hex);
      return TCL_ERROR;
    }

    i=2;
    delta_z = atoi(argv[i]);

    if ( abs(delta_z) > 100 ) {
      sprintf( errorMsg, "Requested focus change too large: %d", delta_z);
      status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Hex);
      return TCL_ERROR;
    }

    sprintf( request, "offset_inc wfs z %d\n", delta_z);

    if ( debug_Hex ) {
      printf("  Hex_Cmd: request = %s", request);
    }

    status = Client_Write( Hex_Info.fd, request, strlen( request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Hex);
      return TCL_ERROR;
    }

    status = Client_ReadToEOF( &Hex_Info, Hex_Reply, errorMsg, debug_local);
    if ( status ) {
      status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Hex);
      return TCL_ERROR;
    }

    if ( debug_Hex ) {
      printf("            reply = %s", Hex_Reply);
    }

    sprintf( request, "apply_offsets\n");

    if ( debug_Hex ) {
      printf("  Hex_Cmd: request = %s", request);
    }

    status = Client_Write( Hex_Info.fd, request, strlen( request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Hex);
      return TCL_ERROR;
    }

    status = Client_ReadToEOF( &Hex_Info, Hex_Reply, errorMsg, debug_local);
    if ( status ) {
      status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Hex);
      return TCL_ERROR;
    }

    if ( debug_Hex ) {
      printf("            reply = %s", Hex_Reply);
    }

  } else if ( !strncmp( name, "Get", strlen("Get") ) ) {

    /********************************************************
     *  Get variable from Info Server on VME
     ********************************************************/

    required_args = 1;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Hex);
      return TCL_ERROR;
    }

    i=2;
    sprintf( request, "get %s\n", argv[i]);

    if ( debug_Hex ) {
      printf("  Hex_Cmd: request = %s", request);
    }

    status = Client_Write( Hex_Info.fd, request, strlen( request));
    if ( status ) {
      sprintf( errorMsg, "Error writting %s", request);
      status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Hex);
      return TCL_ERROR;
    }

    status = Client_ReadToEOF( &Hex_Info, Hex_Reply, errorMsg, debug_local);
    if ( status ) {
      status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Hex);
      return TCL_ERROR;
    }

    if ( debug_Hex ) {
      printf("             reply = %s", Hex_Reply);
    }

  } else if ( !strncmp( name, "All", strlen("All") ) ) {

    /********************************************************
     *  Get variable from Hex Server on VME
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Hex);
      return TCL_ERROR;
    }

    i=2;
    strcpy( request, "all\n");

    if ( debug_Hex ) {
      printf("  Hex_Cmd: request = %s", request);
    }

    status = Client_Write( Hex_Info.fd, request, strlen( request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Hex);
      return TCL_ERROR;
    }

    status = Client_ReadToEOF( &Hex_Info, Hex_Reply, errorMsg, debug_local);
    if ( status ) {
      status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Hex);
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
				   reply_data, debug_Hex);
    return TCL_ERROR;

  }

  /********************************************************
   *  Close a connection to Hex
   ********************************************************/

  if ( !Hex_Info.connected ) {
    return TCL_OK;
  }

  status = Client_Close( &Hex_Info, debug_Hex);
  if ( status ) {
    sprintf( errorMsg, "Error closing connection to %s", Hex_Info.name);
    status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
    status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				  reply_data, debug_Hex);
    return TCL_ERROR;
  }

  Tcl_AppendResult(interp,"OK", NULL);
  strcpy(reply_data, "OK Fine");

  status = Client_GuiUpdate( interp, &Hex_Info, &Hex_GuiVariables);
  return TCL_OK;
}
