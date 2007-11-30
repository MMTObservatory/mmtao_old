/*===========================================================================*
**
**  In this section, new TclTk procedures written in C are created.
**  ---------------------------------------------------------------
**
**  All new procedures must be define above in Tcl_AppInit as well as
**    declared in the headed (see List of new Tcl Commands section).
**
**  Written  27nov06  DLM  Started with Sky_Cmd.c
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "Client.h"
#include "Server.h"
#include "Static.h"

#include "Cmd_Globals.h"
#include "Cmd_Prototypes.h"

extern char *reply_data;

/* PCR_Cmd Connection Information */
extern Server_Info *PCR_Info;

/*===========================================================================*/
int Static_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
{

  /* Variables sent by TclTk and passed on to the subroutines */


  /* Character string used to send information back from called routines.
       This information is passed back to TckTk via the linked variable
       reply_data.
  */

  char errorMsg[IO_LENGTH];

  /*
    Static variables
  */

  static float Static_Matrix[NUM_SLOPES*NUM_STATIC_ZERN];
  static float Static_Offsets[NUM_SLOPES];
  static int first_time = 1;

  /* Local variables */

  int required_args;
  int status, i, j;
  float scale[NUM_STATIC_ZERN];
  char fileName[STRING_LENGTH];
  char request[STRING_LENGTH];

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

  Tcl_ResetResult(interp);

  required_args = 2;

  if (argc < required_args ) {
    sprintf( errorMsg, "ERROR in %s: requires %d arguments, found %d",
	     argv[0], required_args, argc);
    printf("%s\n", errorMsg);
    Tcl_AppendResult(interp, errorMsg, (char *) NULL);
    strcpy(reply_data, errorMsg);
    return TCL_ERROR;
  }

/**************************************************************************
**
**  Initialize Reconstructor on the Static variables.
**
**************************************************************************/

  if ( first_time ) {

    Static_Matrix_Read = 0;

    first_time = 0;

  }

  /*
    Put the request argument into the local request variable
  */
  strcpy( request, argv[1]);

  /*
    Check that a request was sent
  */
  if ( strlen(request) <= 1 ) {
    strcpy( errorMsg, "Static_Cmd: Request of zero length recieved");
    status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Static);
    return TCL_ERROR;
  }

  if ( debug_Static ) {
    printf("  Static_Cmd: request = %s\n", request);
  }

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
    ** Should use this PCR routines  4aug06 DLM
  */
  status = Server_StringLowerCase(request);

/**************************************************************************
**
**  Execute requested Reconstructor on the Static routine
**
**************************************************************************/

  if ( !strncmp( request, "readmatrix", strlen("readmatrix")) ) {

    required_args = 1;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], 
				   required_args, argc-2, reply_data, debug_Static);
      return TCL_ERROR;
    }

    i = 2;
    strcpy(fileName,argv[i]);

    status = Static_ReadMatrix( Static_Matrix, fileName, debug_Static, errorMsg);
    if ( status ) {
      status = Client_RoutineError( interp, "ERROR Reading Matrix", argv[0], argv[1], errorMsg,
                                    reply_data, debug_Static);
      return TCL_ERROR;
    }
    Static_Matrix_Read = 1;

  } else if ( !strncmp( request, "display", strlen("display")) ) {

    if ( !Static_Matrix_Read ) {
      status = Client_RoutineError( interp, "Error, Matrix Not Read Yet", argv[0], argv[1],
                                    errorMsg, reply_data, debug_Static);
      return TCL_ERROR;
    }
	
    required_args = 7;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], 
				   required_args, argc-2, reply_data, debug_Static);
      return TCL_ERROR;
    }

    i = 2;
    for ( j = 0; j<NUM_STATIC_ZERN; j++ ) {
      scale[j] = atof(argv[i++]);
    }

    status = Static_Calc( scale, Static_Matrix, Static_Offsets, debug_Static, errorMsg);
    if ( status ) {
      status = Client_RoutineError( interp, "ERROR Calculating Offsets", argv[0],
                                    argv[1], errorMsg, reply_data, debug_Static);
      return TCL_ERROR;
    }

  } else if ( !strncmp( request, "apply", strlen("apply")) ) {

    if ( !Static_Matrix_Read ) {
      status = Client_RoutineError( interp, "Error, Matrix Not Read Yet", argv[0], argv[1],
                                    errorMsg, reply_data, debug_Static);
      return TCL_ERROR;
    }
	
    required_args = 7;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], 
				   required_args, argc-2, reply_data, debug_Static);
      return TCL_ERROR;
    }

    i = 2;
    for ( j = 0; j<NUM_STATIC_ZERN; j++ ) {
      scale[j] = atof(argv[i++]);
    }

    status = Static_Calc( scale, Static_Matrix, Static_Offsets, debug_Static, errorMsg);
    if ( status ) {
      status = Client_RoutineError( interp, "ERROR Calculating Offsets", argv[0],
                                    argv[1], errorMsg, reply_data, debug_Static);
      return TCL_ERROR;
    }

    status = Static_Send( PCR_Info, Static_Offsets, scale, debug_Static, errorMsg);
    if ( status ) {
      status = Client_RoutineError( interp, "ERROR Sending Offsets to PCR", argv[0],
                                    argv[1], errorMsg, reply_data, debug_Static);
      return TCL_ERROR;
    }

/**************************************************************************
**
**  Unknown call and cleanup
**
**************************************************************************/

  } else {
    sprintf( errorMsg, "ERROR in Static_Cmd: Could not find requested command: %s", argv[2]);
    printf("%s\n", errorMsg);
    Tcl_AppendResult(interp, errorMsg, (char *) NULL);
    strcpy(reply_data, errorMsg);
    return TCL_ERROR;
 }

  Tcl_AppendResult(interp,"OK", NULL);

  return TCL_OK;
}
