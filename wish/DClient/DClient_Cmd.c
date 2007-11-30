/*===========================================================================*
**
**  DClient_Cmd.c
**
**  In this section, new TclTk procedures written in C are created.
**  ---------------------------------------------------------------
**
**  All new procedures must be define above in Tcl_AppInit as well as
**    declared in the headed (see List of new Tcl Commands section).
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "DClient.h"
#include "PGplot_Globals.h"

#include "Client_Globals.h"
#include "Client_Prototypes.h"

extern char *reply_data;

extern PGplot_Info *WFSC_Info;
extern PGplot_Info *Spots_Info;

#define DEBUG 0

/*===========================================================================*
 * DClient_Cmd
 *===========================================================================*/

int DClient_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
{

  /* Variables sent by TclTk and passed on to the subroutines */

  char name[SERVER_LENGTH];
  char type[SERVER_LENGTH];
  char fileName[STRING_LENGTH];
  char dirName[STRING_LENGTH];
  char request[SERVER_LENGTH];

  /* Character string used to send information back from called routines.
       This information is passed back to TckTk via the linked variable
       reply_data.
  */

  char errorMsg[IO_LENGTH];

  /* Server variables */

  static DClient_Info WFSC_Single_Info;
  static DClient_Info WFSC_Save_Info;
  static DClient_Info Spots_Single_Info;
  static DClient_Info Spots_Save_Info;
  static DClient_Info RTR_Single_Info;
  static DClient_Info RTR_Save_Info;
  static DClient_Info Slopes_Single_Info;
  static DClient_Info Slopes_Save_Info;
  static DClient_Info Slope_Single_Info;
  static DClient_Info Slope_Save_Info;
  static DClient_Info Pos_Single_Info;
  static DClient_Info Pos_Save_Info;
  static DClient_Info Cmd_Single_Info;
  static DClient_Info Cmd_Save_Info;
  static DClient_Info Cur_Single_Info;
  static DClient_Info Cur_Save_Info;
  static DClient_Info All_Single_Info;
  static DClient_Info All_Save_Info;
  static DClient_Info DM_Save_Info;
  static int first_time = 1;

  /* Local variables */

  int required_args;
  int status, i, retVal;
  int frames;

  /* Timing variables */
#if( DEBUG )
  static count = 0;
  static struct timeval *systime;
  static int start_sec, start_usec;
  int end_sec, end_usec;
  float total_time;
#endif

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

  strcpy( reply_data, "DClient OK");
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
    status = Server_ZeroState( (Server_Info *)&WFSC_Single_Info);
    status = Server_ZeroState( (Server_Info *)&WFSC_Save_Info);
    status = Server_ZeroState( (Server_Info *)&Spots_Single_Info);
    status = Server_ZeroState( (Server_Info *)&Spots_Save_Info);
    status = Server_ZeroState( (Server_Info *)&Slopes_Single_Info);
    status = Server_ZeroState( (Server_Info *)&Slopes_Save_Info);
    status = Server_ZeroState( (Server_Info *)&Slope_Single_Info);
    status = Server_ZeroState( (Server_Info *)&Slope_Save_Info);
    status = Server_ZeroState( (Server_Info *)&RTR_Single_Info);
    status = Server_ZeroState( (Server_Info *)&RTR_Save_Info);
    status = Server_ZeroState( (Server_Info *)&Pos_Single_Info);
    status = Server_ZeroState( (Server_Info *)&Pos_Save_Info);
    status = Server_ZeroState( (Server_Info *)&Cmd_Single_Info);
    status = Server_ZeroState( (Server_Info *)&Cmd_Save_Info);
    status = Server_ZeroState( (Server_Info *)&Cur_Single_Info);
    status = Server_ZeroState( (Server_Info *)&Cur_Save_Info);
    status = Server_ZeroState( (Server_Info *)&All_Single_Info);
    status = Server_ZeroState( (Server_Info *)&All_Save_Info);
    status = Server_ZeroState( (Server_Info *)&DM_Save_Info);

    first_time = 0;

  }

/**************************************************************************
**
**  Send request to Data Server on the VME
**
**************************************************************************/

  if ( debug_DClient ) {
    printf("  DClient_Cmd: request = %s\n", name);
  }

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  strcpy( request, name);
  status = Server_StringLowerCase(request);

  if ( !strncmp( request, "wfsc", strlen("wfsc")) ) {

    /*
      Requested the WSC server to send the requested type of data to
        this client.  This can be a "Single" frame or a steam of a
	requested number of frames to "Save".
    */

    required_args = 1;

    if (argc < 2+required_args ) {
      status = DClient_ParameterError( "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data);
      return TCL_ERROR;
    }

    i = 2;
    strcpy( type, argv[i++]);

    if ( debug_DClient ) {
      printf("  DClient_Cmd: request = %s\n", type);
    }

    /*
      Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
    */
    status = Server_StringLowerCase(type);

    status = DClient_Spots_Display( &WFSC_Single_Info, WFSC_Info, name, type,
				    debug_DClient, debug_PGplot, errorMsg);
    if ( status ) {
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				    reply_data, debug_PGplot || debug_DClient);
      return TCL_ERROR;
    }

  } else if ( !strncmp( request, "spots", strlen(request)) ) {

    /*
      Requested the WSC server to send the requested type of data to
        this client.  This can be a "Single" frame or a steam of a
	requested number of frames to "Save".
    */

    required_args = 1;

    if (argc < 2+required_args ) {
      status = DClient_ParameterError( "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data);
      return TCL_ERROR;
    }

    i = 2;
    strcpy( type, argv[i++]);

    if ( debug_DClient ) {
      printf("  DClient_Cmd: request = %s\n", type);
    }

    /*
      Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
    */
    status = Server_StringLowerCase(type);

    status = DClient_Spots_Display( &Spots_Single_Info, Spots_Info, name, type,
				    debug_DClient, debug_PGplot, errorMsg);
    if ( status ) {
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				    reply_data, debug_DClient);
      return TCL_ERROR;
    }

  } else if ( !strncmp( request, "slopes", strlen("slopes")) ) {

    /*
      Requested the WSC server to send the requested type of data to
        this client.  This can be a "Single" frame or a steam of a
	requested number of frames to "Save".
    */

    required_args = 1;

    if (argc < 2+required_args ) {
      status = DClient_ParameterError( "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data);
      return TCL_ERROR;
    }

    i = 2;
    strcpy( type, argv[i++]);

    if ( debug_DClient ) {
      printf("  DClient_Cmd: request = %s\n", type);
    }

    /*
      Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
    */
    status = Server_StringLowerCase(type);

    status = DClient_Slopes_Display( &Slopes_Single_Info, Slopes_Info, name, type,
				    debug_DClient, debug_PGplot, errorMsg);
    if ( status ) {
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				    reply_data, debug_DClient);
      return TCL_ERROR;
    }

  } else if ( !strncmp( request, "dm_save", strlen("dm_save")) ) {

    /*
      A specified number of consecutive DM frames are
        saved in a FITS file with the given name.
    */

    required_args = 4;

    if (argc < 1+required_args ) {
      status = DClient_ParameterError( "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data);
      return TCL_ERROR;
    }

    i = 2;
    strcpy( request, argv[i++]);
    frames = atol(argv[i++]);
    strcpy( dirName, argv[i++]);
    strcpy( fileName, argv[i++]);

    if ( debug_DClient ) {
      printf("               request = %s\n", request);
      printf("               frames = %d\n", frames);
      printf("               dirName = %s\n", dirName);
      printf("               fileName = %s\n", fileName);
    }

    if ( DM_Save_Info.server.connected ) {

      printf("  PCR_Cmd: DM_Save_Info should not be connected\n");

    } else {

      /*
	Open a socket the server, start the requested server and allocate needed memory
      */

      status = DClient_DM_Connect( &DM_Save_Info, "DServ", debug_DClient, errorMsg);
      if ( status ) {
	strcpy( reply_data, errorMsg);
	return (status);
      }

      if ( debug_DClient ) {
	printf("  DClient_Cmd: Connected to DM Stream server\n");
      }

      retVal = DClient_DM_Save( &DM_Save_Info, request, dirName, fileName, frames, debug_DClient, errorMsg);

      status = Server_Close( (Server_Info *)&DM_Save_Info, debug_DClient);

      if ( retVal < 0 ) {
	strcpy( reply_data, errorMsg);
	if ( debug_DClient ) printf("  DClient_DM_Save: %s\n", errorMsg);
	return TCL_ERROR;
      }

    }

/**************************************************************************
**
**  Unknown call and cleanup
**
**************************************************************************/

  } else {

    sprintf( errorMsg, "Unknown request: %s", argv[1]);
    status = DClient_RoutineError( "ERROR", argv[0], argv[1], errorMsg, reply_data);
    return TCL_ERROR;

  }

  return TCL_OK;
}
