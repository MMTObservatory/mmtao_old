/*===========================================================================*
**
**  PCR_Cmd.c
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
#include "PCR.h"

#include "Client_Globals.h"
#include "Client_Prototypes.h"

extern char *reply_data;

/* PCR_Cmd Connection Information */
extern Server_Info *PCR_Info;

/*===========================================================================*
 * PCR_Cmd
 *===========================================================================*/

int PCR_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
{

  /* Variables sent by TclTk and passed on to the subroutines */

  char name[SERVER_LENGTH];
  char type[SERVER_LENGTH];
  char request[SERVER_LENGTH];

  /* Character string used to send information back from called routines.
       This information is passed back to TckTk via the linked variable
       reply_data.
  */

  char errorMsg[IO_LENGTH];

  /* Server variables */
  static Gui_Variables PCR_GuiVariables;
  static int first_time = 1;

  /* Local variables */

  int required_args;
  int status, i, new_debug;

  extern int h_errno;

/*************************************************************************
**
**  Check that we have at least one parameter.  This is the request and
**    is copied into name[].
**
**************************************************************************/

  Tcl_ResetResult(interp);

  required_args = 1;

  if (argc < 1+required_args ) {
    status = PCR_ParameterError( "ERROR", argv[0], argv[1], required_args,
				     argc-2, reply_data);
    return TCL_ERROR;
  }
  strcpy( name, argv[1]);

  strcpy( reply_data, "OK: PCR_Cmd");
  strcpy( errorMsg, "");

/**************************************************************************
**
**  Initialize Power socket information structures
**
**************************************************************************/

  if ( first_time ) {

    status = Client_SetupInfo( (Client_Info *) PCR_Info, &PCR_GuiVariables, "PCR",
			       &PCR_Connected, &PCR_Error);

    /*
      Setup Server information in all structures
    */
    status = Server_ZeroState( PCR_Info);

    first_time = 0;

    status = Server_GuiUpdate( interp, PCR_Info, &PCR_GuiVariables );

  }

/**************************************************************************
**
**  Send request to Data Server on the VME
**
**************************************************************************/

  if ( debug_PCR ) {
    printf("  PCR_Cmd: request = %s\n", name);
  }

  /*
    Check that a request was sent
  */
  if ( strlen(name) <= 1 ) {
    printf("  PCR_Cmd: Warning: request of zero length recieved\n");
    return(-1);
  }

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  strcpy( request, name);
  status = Server_StringLowerCase(request);
  //    printf("  PCR_Cmd: request = %s\n", request);

  /*
    All commands require a connection to the DServ server, so every time
      PCR_Cmd is called, we will check to make sure we are connected, or
      will initiate this connection
  */
  if ( !PCR_Info->connected ) {

    /*
      Open a socket to the DServ server
    */
    status = PCR_Connect( PCR_Info, debug_PCR, errorMsg);
    if ( status ) {
      strcpy( reply_data, errorMsg);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Connected to PCR_Server\n");
    }
  }

  if ( !strncmp( request, "pcr_connect", strlen(request)) ) {

  } else if ( !strncmp( request, "debug", strlen(request)) ) {

    /*
      Send request connect to WFSC
    */
    strcpy( request, "debug");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send debug flag to server
    */
    i = 2;
    strcpy( type, argv[i++]);
    new_debug = atoi(argv[i++]);

    //    status = PCR_Debug( new_debug, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending debug flag", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Set debug to %s\n", new_debug ? "on" : "off");
    }

  } else if ( !strncmp( request, "cmd", strlen(request)) ) {

    /*
      Check to see if the command changes the camera settings.  If so,
        then we must send a stop to the camera and a reset to dserv
    */

    /*
      Read request
    */
    i = 2;
    strcpy( request, argv[i++]);
    status = Server_StringLowerCase(request);

    if ( !strncmp( request, "set_cam", 7 ) ) {

      /*
	Send a "stop_WFSC" to PCR Cmd server
      */
      strcpy( request, "cmd");
      status = PCR_Request( PCR_Info, request, errorMsg);
      if ( status ) {
	status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				       errorMsg, reply_data);
	return TCL_ERROR;
      }

      if ( debug_PCR ) {
	printf("  PCR_Cmd: Sent request %s\n", request);
      }

      /*
	Send request to PCR Cme server
      */
      strcpy( request, "stop_WFSC");
      status = PCR_Request( PCR_Info, request, errorMsg);
      if ( status ) {
	status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				       errorMsg, reply_data);
	return TCL_ERROR;
      }

      if ( debug_PCR ) {
	printf("                        %s\n", request);
      }

      /*
	Send request to reset the PCR client
      */
      strcpy( request, "reset");
      status = PCR_Request( PCR_Info, request, errorMsg);
      if ( status ) {
	status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				       errorMsg, reply_data);
	return TCL_ERROR;
      }

      if ( debug_PCR ) {
	printf("  PCR_Cmd: Sent request %s\n", request);
      }

      /*
	Send request to reset client receiving WFSC data from the PCR
      */
      strcpy( request, "wfsc");
      status = PCR_Request( PCR_Info, request, errorMsg);
      if ( status ) {
	status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				       errorMsg, reply_data);
	return TCL_ERROR;
      }

      if ( debug_PCR ) {
	printf("                        %s\n", request);
      }

    }

    /*
      Send request to PCR Cmd server

    strcpy( request, "cmd");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }
    */

    /*
      Read request
    */
    i = 2;
    strcpy( request, argv[i++]);

    /*
      Send request to PCR Cme server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("                        %s\n", request);
    }

    /*
      Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
    */
    status = Server_StringLowerCase(request);
    
    if ( !strncmp( request, "stop_wfsc", strlen(request)) ) {

      /*
	Send request to reset the PCR client
      */
      strcpy( request, "reset");
      status = PCR_Request( PCR_Info, request, errorMsg);
      if ( status ) {
	status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				       errorMsg, reply_data);
	return TCL_ERROR;
      }

      if ( debug_PCR ) {
	printf("  PCR_Cmd: Sent request %s\n", request);
      }

      /*
	Send request to reset client receiving WFSC data from the PCR
      */
      strcpy( request, "wfsc");
      status = PCR_Request( PCR_Info, request, errorMsg);
      if ( status ) {
	status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				       errorMsg, reply_data);
	return TCL_ERROR;
      }

      if ( debug_PCR ) {
	printf("  PCR_Cmd: Sent request %s\n", request);
      }

    }

  } else if ( !strncmp( request, "dm_cmd", strlen(request)) ) {

    /*
      Send request to PCR DM Data Server
    */
    strcpy( request, "dm_cmd");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Read request
    */
    i = 2;
    sprintf( request, argv[i++]);

    /*
      Send request to PCR DM Data Server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("                        %s\n", request);
    }

  } else if ( !strncmp( request, "user_gain", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    strcpy( request, "cmd");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Read request
    */
    i = 2;
    sprintf( request, "user_gain %s", argv[i++]);

    /*
      Send request to PCR Cme server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "cam_temp_get", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    strcpy( request, "cam_temp_get");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "cam_bg_send", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    strcpy( request, "cam_bg_send");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "slope_offset_send", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    strcpy( request, "slope_offset_send");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "cam_bias_set", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send quad number and bias value to PCR Cmd server
    */
    i = 2;
    sprintf( request, "%s", argv[i++]);
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("                           %s\n", request);
    }

    sprintf( request, "%s", argv[i++]);
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("                           %s\n", request);
    }

  } else if ( !strncmp( request, "cam_rate_set", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send camera rate to PCR Cmd server
    */
    i = 2;
    sprintf( request, "%s", argv[i++]);
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("                           %s\n", request);
    }

  } else if ( !strncmp( request, "recon_set", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send the name of the reconstructor to request the PCR to load
    */
    i = 2;
    sprintf( request, "%s", argv[i++]);
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("                           %s\n", request);
    }

  } else if ( !strncmp( request, "pcr_cmd_connect", strlen(request)) ) {

    /*
      Send request to connect to a PCR server
    */
    strcpy( request, "connect");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      status = Server_Close( PCR_Info, debug_PCR);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send request to open a connection to the PCR for WFSC data
    */
    strcpy( request, "cmd");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      status = Server_Close( PCR_Info, debug_PCR);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "pcr_wfsc_connect", strlen(request)) ) {

    /*
      Send request to connect to a PCR server
    */
    strcpy( request, "connect");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      status = Server_Close( PCR_Info, debug_PCR);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send request to open a connection to the PCR for WFSC data
    */
    strcpy( request, "wfsc");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      status = Server_Close( PCR_Info, debug_PCR);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "pcr_dds_connect", strlen(request)) ) {

    /*
      Send request to connect to a PCR server
    */
    strcpy( request, "connect");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      status = Server_Close( PCR_Info, debug_PCR);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send request to open a connection to the PCR for DM data
    */
    strcpy( request, "dm");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      status = Server_Close( PCR_Info, debug_PCR);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "pcr_error_connect", strlen(request)) ) {

    /*
      Send request to connect to a PCR server
    */
    strcpy( request, "connect");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      status = Server_Close( PCR_Info, debug_PCR);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send request to open a connection to the PCR for Error messages
    */
    strcpy( request, "error");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      status = Server_Close( PCR_Info, debug_PCR);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "camera_start", strlen(request)) ) {

    /*
      Send request to stop client receiving WFSC data from the PCR
    */
    strcpy( request, "Cam_Start");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "camera_stop", strlen(request)) ) {

    /*
      Send request to stop client receiving WFSC data from the PCR
    */
    strcpy( request, "Cam_Stop");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "loop_close", strlen(request)) ) {

    /*
      Send request to close the AO loop
    */
    strcpy( request, "Loop_Close");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "loop_open", strlen(request)) ) {

    /*
      Send request to open the AO loop
    */
    strcpy( request, "Loop_Open");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "loop_gain", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send loop gain value to PCR
    */
    i = 2;
    sprintf( request, "%s", argv[i++]);
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("                           %s\n", request);
    }

  } else if ( !strncmp( request, "loop_pgain", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send loop pgain value to PCR
    */
    i = 2;
    sprintf( request, "%s", argv[i++]);
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("                           %s\n", request);
    }


  } else if ( !strncmp( request, "loop_igain", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send loop igain value to PCR
    */
    i = 2;
    sprintf( request, "%s", argv[i++]);
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("                           %s\n", request);
    }

  } else if ( !strncmp( request, "loop_dgain", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send loop dgain value to PCR
    */
    i = 2;
    sprintf( request, "%s", argv[i++]);
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("                           %s\n", request);
    }

  } else if ( !strncmp( request, "set_pid_off", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "set_pid_on", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "set_delta_gain", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send loop Delta gain value to PCR
    */
    i = 2;
    sprintf( request, "%s", argv[i++]);
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("                           %s\n", request);
    }

  } else if ( !strncmp( request, "set_previous_gain", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send loop Previous_gain value to PCR
    */
    i = 2;
    sprintf( request, "%s", argv[i++]);
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }
    if ( debug_PCR ) {
      printf("                           %s\n", request);
    }

  } else if ( !strncmp( request, "set_tap_gains_off", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "set_tap_gains_on", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "dm_stop", strlen(request)) ) {

    /*
      Send request to stop the PCR client
    */
    strcpy( request, "stop");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send request to stop client receiving DM data from the PCR
    */
    strcpy( request, "dm");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "wfsc_reset", strlen(request)) ) {

    /*
      Send request to reset the PCR client
    */
    strcpy( request, "reset");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send request to reset client receiving WFSC data from the PCR
    */
    strcpy( request, "wfsc");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "dm_reset", strlen(request)) ) {

    /*
      Send request to reset the PCR client
    */
    strcpy( request, "reset");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

    /*
      Send request to reset client receiving DM data from the PCR
    */
    strcpy( request, "dm");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "slope_offset_on", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "slope_offset_off", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "mod_offset_on", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "mod_offset_off", strlen(request)) ) {

    /*
      Send request to PCR Cmd server
    */
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = PCR_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Sent request %s\n", request);
    }

  } else if ( !strncmp( request, "close", strlen(request)) ) {

    if ( PCR_Info->connected ) {
      /*
	Close the local connection
      */
      status = Server_Close( PCR_Info, debug_PCR);
    }

    if ( debug_PCR ) {
      printf("  PCR_Cmd: Connection to DServ closed\n");
    }


/**************************************************************************
**
**  Unknown call and cleanup
**
**************************************************************************/

  } else {

    printf("  PCR_Cmd: Unknown request: %s\n", name);
    sprintf( errorMsg, "Unknown request: %s", argv[1]);
    status = PCR_RoutineError( "ERROR", argv[0], argv[1], errorMsg, reply_data);
    return TCL_ERROR;

  }

  status = Server_GuiUpdate( interp, PCR_Info, &PCR_GuiVariables);
  return TCL_OK;
}
