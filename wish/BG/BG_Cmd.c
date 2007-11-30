/*===========================================================================*
**
**  BG_Cmd.c
**
**  In this section, new TclTk procedures written in C are created.
**  ---------------------------------------------------------------
**
**  All new procedures must be define above in Tcl_AppInit as well as
**    declared in the headed (see List of new Tcl Commands section).
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "Cmd_Globals.h"
#include "Cmd_Prototypes.h"

#include "DClient.h"

#include "BG.h"
#include "PCR.h"

extern char *reply_data;

/* PCR_Cmd Connection Information */
extern Server_Info *PCR_Info;

/*===========================================================================*
 * BG_Cmd
 *===========================================================================*/

int BG_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
{

  /* Variables sent by TclTk and passed on to the subroutines */
  char name[SERVER_LENGTH];
  char request[SERVER_LENGTH];
  long bg_num, bg_display;
  char fileName[STRING_LENGTH];

  /* Character string used to send information back from called routines.
       This information is passed back to TckTk via the linked variable
       reply_data.
  */
  char errorMsg[IO_LENGTH];

  /* Server variables */

  static DClient_Info BG_Info;
  static int first_time = 1;

  /* Local variables */
  int required_args;
  int status, retVal;
  int error, i;

/*************************************************************************
**
**  Check that we have at least one parameter.  This is the request and
**    is copied into name[].
**
**************************************************************************/

  Tcl_ResetResult(interp);

  required_args = 1;

  if (argc < 1+required_args ) {
    status = DClient_ParameterError( "ERROR", argv[0], argv[1], required_args,
				     argc-2, reply_data);
    return TCL_ERROR;
  }
  strcpy( name, argv[1]);

  if ( debug_BG ) {
    printf("  BG_Cmd: request = %s\n", name);
  }

  strcpy( reply_data, "BG_Cmd OK");
  strcpy( errorMsg, "");

/**************************************************************************
**
**  Initialize Power socket information structures
**
**************************************************************************/

  if ( first_time ) {

    /*
      Setup Server information in all structures
    */
    status = Server_ZeroState( (Server_Info *)&BG_Info);

    first_time = 0;

  }

/**************************************************************************
**
**  Process Request
**
**************************************************************************/

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  strcpy( request, name);
  status = Server_StringLowerCase(request);

  if ( !strncmp( request, "acquire", strlen(request) ) ) {

    /********************************************************
     *  Acquire the requested number of WFSC frames, determine their average
     *    and write them to a .fits and ascii file.
     ********************************************************/

    required_args = 3;

    if (argc != 2+required_args ) {
      error = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_BG);
      return TCL_ERROR;
    }

    i=2;
    bg_num = atol( argv[i++]);
    bg_display = atol( argv[i++]);
    strcpy( fileName, argv[i++]);
    if ( debug_BG ) {
      printf("  BG_Cmd: Number of Frames = %ld\n", bg_num);
      printf("        Display Background = %ld\n", bg_display);
      printf("                  FileName = %s\n", fileName);
    }

    if ( !BG_Info.server.connected ) {

      /*
	Open a socket the server, start the requested server and allocate needed memory
      */

      status = DClient_DM_Connect( &BG_Info, "DServ", debug_BG, errorMsg);
      if ( status ) {
	strcpy( reply_data, errorMsg);
	return (status);
      }

      if ( debug_BG ) {
	printf("  BG_Cmd: Connected to DM Stream server\n");
      }

      retVal = BG_Acquire( &BG_Info, fileName, bg_num, debug_BG, errorMsg);

      status = Server_Close( (Server_Info *)&BG_Info, debug_BG);

      if ( retVal < 0 ) {
	strcpy( reply_data, errorMsg);
	if ( debug_BG ) printf("  BG_Acquire: %s\n", errorMsg);
	return TCL_ERROR;
      }

    }

  } else if ( !strncmp( request, "send", strlen("send")) ) {

    required_args = 1;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], 
				      required_args, argc-2, reply_data, debug_Mod);
      return TCL_ERROR;
    }

    i = 2;
    strcpy( fileName, argv[i++]);

    status = BG_Send( PCR_Info, fileName, debug_BG, errorMsg);
    if ( status ) {
      status = Client_RoutineError( interp, "ERROR Sending Background to PCR", argv[0],
				    argv[1], errorMsg, reply_data, debug_Mod);
      return TCL_ERROR;
    }

  } else if ( !strncmp( request, "on", strlen("on")) ) {

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], 
				      required_args, argc-2, reply_data, debug_Mod);
      return TCL_ERROR;
    }

    /*
      Send request to connect to a PCR server
    */
    strcpy( request, "cam_bg_on");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = DClient_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

  } else if ( !strncmp( request, "off", strlen("off")) ) {

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], 
				      required_args, argc-2, reply_data, debug_Mod);
      return TCL_ERROR;
    }

    /*
      Send request to connect to a PCR server
    */
    strcpy( request, "cam_bg_off");
    status = PCR_Request( PCR_Info, request, errorMsg);
    if ( status ) {
      status = DClient_RoutineError( "ERROR sending request to PCR", argv[0], argv[1],
				     errorMsg, reply_data);
      return TCL_ERROR;
    }

/**************************************************************************
**
**  Unknown call and cleanup
**
**************************************************************************/

  } else {

    sprintf( errorMsg, "Unknown request: %s", argv[1]);
    error = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				   reply_data, debug_BG);
    return TCL_ERROR;

  }

  Tcl_AppendResult(interp,"OK", NULL);
  strcpy(reply_data, "OK Fine");

  return TCL_OK;
}
