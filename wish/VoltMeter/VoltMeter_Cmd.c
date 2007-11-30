/*===========================================================================*
**
**  In this section, new TclTk procedures written in C are created.
**  ---------------------------------------------------------------
**
**  All new procedures must be define above in Tcl_AppInit as well as
**    declared in the headed (see List of new Tcl Commands section).
**
**  Written  July03  Dylan Curley
**
**  Altered  8jan04  DLM  Started wih OAP_SendCmd.c
**
**  Altered  27mar04  DLM  Change format of FSM_SendCmd to FSM_Cmd
**  Altered  30nov07  DLM  
**                         NOTE: VoltMeter_Cmd is currently not included in
**			         the compilation of PCR_Wish.  The VoltMeter
**				 GUI is about 90% done.
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "VoltMeter_Globals.h"
#include "VoltMeter_Prototypes.h"

#include "VoltMeter.h"

extern char *reply_data;

/*===========================================================================*/
int VoltMeter_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
{

  /* Variables sent by TclTk and passed on to the subroutines */


  /* Character string used to send information back from called routines.
       This information is passed back to TckTk via the linked variable
       reply_data.
  */

  char errorMsg[IO_LENGTH];
  char request[IO_LENGTH];

  /* Local variables */

  int required_args;
  int status, i;
  static Motor_Info VoltMeter_Info;
  static Gui_Variables VoltMeter_Variables;
  static int first_time = 1;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

  Tcl_ResetResult(interp);

  required_args = 1;

  if (argc < 1+required_args ) {
    sprintf( errorMsg, "ERROR in %s: requires %d arguments, found %d",
	     argv[0], required_args, argc-1);
    printf("%s\n", errorMsg);
    Tcl_AppendResult(interp, errorMsg, (char *) NULL);
    strcpy(reply_data, errorMsg);
    return TCL_ERROR;
  }

  strcpy(reply_data, "OK Fine");

/**************************************************************************
**
**  Initialize VoltMeter socket information structures
**
**************************************************************************/

  if ( first_time ) {

    /* Tip information */
    strcpy ( VoltMeter_Info.motor, "HP");
    strcpy ( VoltMeter_Info.name, "VoltMeter");
    strcpy ( VoltMeter_Info.address, VoltMeter_Address);
    VoltMeter_Info.port = VoltMeter_Port;
    VoltMeter_Info.driveid = 0;
    VoltMeter_Info.sockfd = 0;
    VoltMeter_Info.gain = 0.0;
    VoltMeter_Info.accel = 0;
    VoltMeter_Info.speed = 0;

    status = VoltMeter_ZeroState( &VoltMeter_Info, &VoltMeter_Variables);

    first_time = 0;

    status = VoltMeter_GuiUpdate( interp, &VoltMeter_Info, &VoltMeter_Variables );

  }

/**************************************************************************
**
**  Operate HP VoltMeter
**
**************************************************************************/

  if ( strncmp( argv[1], "Connect", strlen("Connect")) == 0) {

    required_args = 0;

    if (argc != 2+required_args ) {
      status = VoltMeter_ParameterError( interp, &VoltMeter_Info, "ERROR", argv[0], argv[1], 
				   required_args, argc-2, reply_data);
      return TCL_ERROR;
    }

    status = VoltMeter_Connect( &VoltMeter_Info, debug_VoltMeter);
    if ( status ) {
      /*
	A non-zero status is due to a timeout in reading the voltmeter.

	This could be an error ocurred in communication or we
	  have lost connection to the voltmeter.  VoltMeter_Get(Read) will take
	  the appropriate action.
      */
      strcpy( request, VOLTMETER_RETRIEVE_ERROR );
      strcat( request, "\012");
      status = VoltMeter_Get( &VoltMeter_Info, request);
      status = VoltMeter_GuiUpdate( interp, &VoltMeter_Info, &VoltMeter_Variables );
      status = VoltMeter_RoutineError( interp, &VoltMeter_Info, "ERROR", argv[0], argv[1], reply_data);

      return TCL_ERROR;
    }

  } else if (strncmp( argv[1], "Close", strlen("Close")) == 0) {

    required_args = 0;

    if (argc != 2+required_args ) {
      status = VoltMeter_ParameterError( interp, &VoltMeter_Info, "ERROR", argv[0], argv[1], 
				   required_args, argc-2, reply_data);
      return TCL_ERROR;
    }

    status = VoltMeter_Close( &VoltMeter_Info, debug_VoltMeter);
    if ( status ) {
      status = VoltMeter_GuiUpdate( interp, &VoltMeter_Info, &VoltMeter_Variables );
      status = VoltMeter_RoutineError( interp, &VoltMeter_Info, "ERROR", argv[0], argv[1], reply_data);
      return TCL_ERROR;
    }

  } else if ( strncmp( argv[1], "Get", strlen("Get")) == 0) {

    required_args = 1;

    if (argc != 2+required_args ) {
      status = VoltMeter_ParameterError( interp, &VoltMeter_Info, "ERROR", argv[0], argv[1], 
				   required_args, argc-2, reply_data);
      return TCL_ERROR;
    }

    i = 2;
    strcpy( request, argv[i++]);
    strcat( request, "\012");

    if ( VoltMeter_Info.state.connected ) {

      status = VoltMeter_Get( &VoltMeter_Info, request);
      if ( status ) {

	/*
	  A non-zero status is due to a timeout in reading the voltmeter.

	  This could be an error ocurred in communication or we
	    have lost connection to the voltmeter.  VoltMeter_Get(Read) will take
	    the appropriate action.
	*/
	strcpy( request, VOLTMETER_RETRIEVE_ERROR );
	strcat( request, "\012");
	status = VoltMeter_Get( &VoltMeter_Info, request);
	status = VoltMeter_GuiUpdate( interp, &VoltMeter_Info, &VoltMeter_Variables );
	status = VoltMeter_RoutineError( interp, &VoltMeter_Info, "ERROR", argv[0], argv[1], reply_data);

	return TCL_ERROR;
      }
      /*
	Send retrieved information back to the GUI
      */
      strcpy( reply_data, VoltMeter_Info.in_line);
    }

  } else if ( strncmp( argv[1], "Measure", strlen("Measure")) == 0) {

    required_args = 0;

    if (argc != 2+required_args ) {
      status = VoltMeter_ParameterError( interp, &VoltMeter_Info, "ERROR", argv[0], argv[1], 
				   required_args, argc-2, reply_data);
      return TCL_ERROR;
    }

    if ( VoltMeter_Info.state.connected ) {

      status = VoltMeter_Measure( &VoltMeter_Info);
      if ( status ) {

	/*
	  A non-zero status is due to a timeout in reading the voltmeter.

	  This could be an error ocurred in communication or we
	    have lost connection to the voltmeter.  VoltMeter_Get(Read) will take
	    the appropriate action.
	*/
	strcpy( request, VOLTMETER_RETRIEVE_ERROR );
	strcat( request, "\012");
	status = VoltMeter_Get( &VoltMeter_Info, request);
	status = VoltMeter_GuiUpdate( interp, &VoltMeter_Info, &VoltMeter_Variables );
	status = VoltMeter_RoutineError( interp, &VoltMeter_Info, "ERROR", argv[0], argv[1], reply_data);

	return TCL_ERROR;
      }

    } else {

      status = VoltMeter_RoutineError( interp, &VoltMeter_Info, "WARNING: Not connected",
				       argv[0], argv[1], reply_data);
      return TCL_OK;

    }

  } else if ( strncmp( argv[1], "Set", strlen("Set")) == 0) {

    required_args = 1;

    if (argc != 2+required_args ) {
      status = VoltMeter_ParameterError( interp, &VoltMeter_Info, "ERROR", argv[0], argv[1], 
				   required_args, argc-2, reply_data);
      return TCL_ERROR;
    }

    if ( VoltMeter_Info.state.connected ) {

      strcpy( request, argv[2]);
      strcat( request, "\012");

      status = VoltMeter_Set( &VoltMeter_Info, request, debug_VoltMeter);
      if ( status ) {

	/*
	  A non-zero status is due to a timeout in reading the voltmeter.

	  This could be an error ocurred in communication or we
	    have lost connection to the voltmeter.  VoltMeter_Get(Read) will take
	    the appropriate action.
	*/
	strcpy( request, VOLTMETER_RETRIEVE_ERROR );
	strcat( request, "\012");
	status = VoltMeter_Get( &VoltMeter_Info, request);
	status = VoltMeter_GuiUpdate( interp, &VoltMeter_Info, &VoltMeter_Variables );
	status = VoltMeter_RoutineError( interp, &VoltMeter_Info, "ERROR", argv[0], argv[1], reply_data);

	return TCL_ERROR;
      }

    } else {

      status = VoltMeter_RoutineError( interp, &VoltMeter_Info, "WARNING: Not connected",
				       argv[0], argv[1], reply_data);
      return TCL_OK;

    }


/**************************************************************************
**
**  Unknown call and cleanup
**
**************************************************************************/

  } else {
    sprintf( errorMsg, "ERROR in VoltMeter_Cmd: Could not find requested command: %s", argv[1]);
    printf("%s\n", errorMsg);
    Tcl_AppendResult(interp, errorMsg, (char *) NULL);
    strcpy(reply_data, errorMsg);
    return TCL_ERROR;
 }

  Tcl_AppendResult(interp,"OK", NULL);

  status = VoltMeter_GuiUpdate( interp, &VoltMeter_Info, &VoltMeter_Variables );

  return TCL_OK;
}
