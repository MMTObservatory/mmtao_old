/*===========================================================================*
**
**  PGplot_Cmd.c
**
**  In this section, new TclTk procedures written in C are created.
**  ---------------------------------------------------------------
**
**  All new procedures must be define above in Tcl_AppInit as well as
**    declared in the headed (see List of new Tcl Commands section).
**
**===========================================================================*/

#include <pthread.h>

#include "PCR_Wish.h"

#include "PGplot.h"

#include "PGplot_Globals.h"
#include "PGplot_Prototypes.h"

#include "Client.h"

extern char *reply_data;

/*===========================================================================*
 * PGplot_Cmd
 *===========================================================================*/

int PGplot_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
{

  /* Variables sent by TclTk and passed on to the subroutines */
  char name[STRING_LENGTH];
  char request[STRING_LENGTH];
  char path[STRING_LENGTH];
  double x_min, x_max;

  /* Character string used to send information back from called routines.
       This information is passed back to TckTk via the linked variable
       reply_data.
  */
  char errorMsg[IO_LENGTH];

  /* Server variables */
  static PGplot_Info Seeing_Info, Intensity_Info;
  static int num_pixels = 0;

  /*
    Data arrays that will be allocated when the size of the array is known
  */
  static short *WFSC_Array = NULL;
  static float *WFSC_Image = NULL;

  /* Local variables */
  int required_args;
  int status, i;

/*************************************************************************
**
**  Check that we have at least one parameter.  This is the request and
**    is copied into name[].
**
**************************************************************************/

  Tcl_ResetResult(interp);

  required_args = 2;

  if (argc < 1+required_args ) {
    status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				 argc-2, reply_data, debug_PGplot);
    if ( debug_PGplot ) {
        printf("%s\n", reply_data);
    }
    return TCL_ERROR;
  }
  strcpy( name, argv[1]);
  strcpy( request, argv[2]);

  strcpy( reply_data, "OK: PGplot_Cmd");
  strcpy( errorMsg, "");

  if ( debug_PGplot ) {
    printf("  PGplot_Cmd: Name = %s\n", name);
    printf("           Request = %s\n", request);
  }

/**************************************************************************
**
**  Process the request
**
**************************************************************************/

  if ( !strncmp( name, "Spots", strlen("Spots") ) ) {

    if ( !strncmp( request, "Open", strlen("Open") ) ) {

    /********************************************************
     *  Open a PGplot /xtk canvas
     ********************************************************/

      required_args = 1;

      if (argc != 3+required_args ) {
	status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
					argc-3, reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      i=3;
      strcpy( path, argv[i++]);

      if ( debug_PGplot ) {
	printf( "              Path = %s\n", path);
      }

      /*
	Open the /xtk canvas
      */
      status = PGplot_Open( Spots_Info, path, debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s", request);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      /*
	Allow the display of open-loop WFSC image to display on the same /xtk window
      */
      WFSC_Info->id = Spots_Info->id;
      strcpy( WFSC_Info->path, Spots_Info->path);

      /*
	Allocate needed memory for closed-loop Spots image
      */
      status = PGplot_SetupInfo( Spots_Info, "Spots", debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s", request);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      /*
	Allocate needed memory for open-loop WFSC image
      */
      status = PGplot_SetupInfo( WFSC_Info, "Spots", debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s", request);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      /*
	Load the "Heat" color table
      */
      status = PGplot_ColorTable( Spots_Info, 0, debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error loading Heat color table");
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }

    } else if ( !strncmp( request, "Setup", strlen("Setup") ) ) {

      /********************************************************
       *  Setup plot parameters
       ********************************************************/

      required_args = 0;

      if (argc != 3+required_args ) {
	status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-3, reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      status = PGplot_SpotsSetup( Spots_Info, debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s", request);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }

    } else if ( !strncmp( request, "Update", strlen("Update") ) ) {

      /********************************************************
       *  Create the plot
       ********************************************************/

      required_args = 0;

      if (argc != 3+required_args ) {
	status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-3, reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      /*
	If this is the first time, or the size of the WFSC image has changed, the
	  we must re-allocate the memory needed and reset the plot parameters.
      */
      if ( Spots_Info->spots->values != num_pixels ) {

	num_pixels = Spots_Info->spots->values;

	status = PGplot_SpotsPattern( Spots_Info, &WFSC_Array, debug_PGplot);
	if ( status ) {
	  sprintf( errorMsg, "Error with %s, returned %d from PGplot_SpotsPattern", request, status);
	  status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
					reply_data, debug_PGplot);
	  return TCL_ERROR;
	}

	if ( WFSC_Image != NULL ) {
	  free(WFSC_Image);
	  printf("  PGplot_SpotsPattern: WFSC_Image memory freed\n");
	}

	if (( WFSC_Image = (float *)malloc(Spots_Info->spots->values*Spots_Info->spots->values*sizeof(float)) ) == NULL ) {
	  printf("  PGplot_SpotsPattern: Error allocating WFSC_Image\n");
	  return(-1);
	}

	for(i=0; i<Spots_Info->spots->values*Spots_Info->spots->values; i++) {
	  WFSC_Image[i] = (float) WFSC_Array[i];
	}

      }

      /*========================================
	Select the PGPLOT device for Spots and set new page.  cpgpage() is required for a
	  new page to be updated by /xtk.  However, if PGplot_SpotsUpdate is called
	  by a separate thread, a call to cpgpage() will cause the display to flicker
	========================================*/
      cpgslct( Spots_Info->id);
      cpgpage();

      status = PGplot_SpotsUpdate( Spots_Info, Spots_Type, Spots_Slopes, Spots_Arrow, debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s, returned %d from PGplot_SpotsUpdate", request, status);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }

    }

  } else if ( !strncmp( name, "Slopes", strlen("Slopes") ) ) {

    if ( !strncmp( request, "Open", strlen("Open") ) ) {

    /********************************************************
     *  Open a PGplot /xtk canvas
     ********************************************************/

      required_args = 1;

      if (argc != 3+required_args ) {
	status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
					argc-3, reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      i=3;
      strcpy( path, argv[i++]);

      if ( debug_PGplot ) {
	printf( "              Path = %s\n", path);
      }

      /*
	Open the /xtk canvas
      */
      status = PGplot_Open( Slopes_Info, path, debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s", request);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      /*
	Allocate needed memory for closed-loop Slopes Vector image
      */
      status = PGplot_SetupInfo( Slopes_Info, "Slopes", debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s", request);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }

    } else {

      sprintf( errorMsg, "Error with %s: Unknown request %s", name, request);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				    reply_data, debug_PGplot);
      return TCL_ERROR;

    }      

  } else if ( !strncmp( name, "Seeing", strlen("Seeing") ) ) {

    if ( !strncmp( request, "Open", strlen("Open") ) ) {

    /********************************************************
     *  Open a PGplot /xtk canvas
     ********************************************************/

      required_args = 1;

      if (argc != 3+required_args ) {
	status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
					argc-3, reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      i=3;
      strcpy( path, argv[i++]);

      if ( debug_PGplot ) {
	printf( "              Path = %s\n", path);
      }

      status = PGplot_Open( &Seeing_Info, path, debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s", request);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      status = PGplot_SetupInfo( &Seeing_Info, "Seeing", debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s", request);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }

    } else if ( !strncmp( request, "TestDraw", strlen("TestDraw") ) ) {

      /********************************************************
       *  Create the plot
       ********************************************************/

      required_args = 2;

      if (argc != 3+required_args ) {
	status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-3, reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      i=3;
      x_min = atof( argv[i++]);
      x_max = atof( argv[i++]);

      if ( debug_PGplot ) {
	printf( "             x_min = %f\n", x_min);
	printf( "             x_max = %f\n", x_max);
      }

      status = PGplot_TestDraw( &Seeing_Info, x_min, x_max, debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s", request);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }
    }

  } else if ( !strncmp( name, "Intensity", strlen("Intensity") ) ) {

    if ( !strncmp( request, "Open", strlen("Open") ) ) {

    /********************************************************
     *  Open a PGplot /xtk canvas
     ********************************************************/

      required_args = 1;

      if (argc != 3+required_args ) {
	status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
					argc-3, reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      i=3;
      strcpy( path, argv[i++]);

      if ( debug_PGplot ) {
	printf( "              Path = %s\n", path);
      }

      status = PGplot_Open( &Intensity_Info, path, debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s", request);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      status = PGplot_SetupInfo( &Intensity_Info, "Intensity", debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s", request);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
	return TCL_ERROR;
      }

    } else if ( !strncmp( request, "TestDraw", strlen("TestDraw") ) ) {

      /********************************************************
       *  Create the plot
       ********************************************************/

      required_args = 2;

      if (argc != 3+required_args ) {
	status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-3, reply_data, debug_PGplot);
	return TCL_ERROR;
      }

      i=3;
      x_min = atof( argv[i++]);
      x_max = atof( argv[i++]);

      if ( debug_PGplot ) {
	printf( "             x_min = %f\n", x_min);
	printf( "             x_max = %f\n", x_max);
      }

      status = PGplot_TestDraw( &Intensity_Info, x_min, x_max, debug_PGplot);
      if ( status ) {
	sprintf( errorMsg, "Error with %s", request);
	status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				      reply_data, debug_PGplot);
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
    status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				   reply_data, debug_PGplot);
    return TCL_ERROR;

  }

  Tcl_AppendResult(interp,"OK", NULL);
  strcpy(reply_data, "OK Fine");

  return TCL_OK;
}
