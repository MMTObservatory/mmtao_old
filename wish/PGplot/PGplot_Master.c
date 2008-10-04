/*
  PGplot_Master.c

  Top level thread started to display Realtime WFSC images

  This routine creates a 2-D plot with the given parameters.  PGlot_Open
    must be called before this routine to create a device.  The device id
    is stored in the PGplot_Info structure.

  Written  28mar06  Copied example program from
                      http://www.astro.caltech.edu/~tjp/pgplot/tkdriv.html

  Altered  29mar06  Modified so PGplot_TestDraw is no long called directly from tcl
                      but a C routine that is called by PGplot_Cmd.
*/

#include <pthread.h>

#include "PCR_Wish.h"

#include "DClient.h"
#include "PGplot_Thread.h"
#include "PGplot_Globals.h"

extern PGplot_Info *Spots_Info;

void *PGplot_Master( void *Passed_Info)
{

  /*
    Structure with all the passed information
  */
  PGplot_Thread *Main_Info;

  /*
    Structure with the PGplot information that will be filled
      by this thread
  */
  PGplot_Info *Spots_Info;

  /*
    Data arrays that will be allocated when the size of the array is known
  */
  char errorMsg[STRING_LENGTH];
  static DClient_Info Spots_Single_Info;
  short *WFSC_Array = NULL;

  /*
    Local pointer to flag in PGplot_Info structure
  */
  char *path;
  int *running;
  int *continueRunning;
  int *debug;
  int *update;
  int *realtime;
  int frameNumber;
  int debug_local = 0;

  int debug_PGplot = 1;
  int status;
  int cir_low, cir_high;      /* low and high colour indexes for images */

  /*
    Access structures that are common to the main process and this thread
  */
  Main_Info = (PGplot_Thread *)Passed_Info;
  Spots_Info = Main_Info->info;
  path = Main_Info->path;
  running = Main_Info->running;
  continueRunning = Main_Info->continueRunning;
  debug = Main_Info->debug;
  update = Main_Info->update;
  realtime = Main_Info->realtime;

  /*
    Since we are just starting up this routine we can assume we want this
      thread to continue to run until the main process sets continueRunning = 0
      and we assume we should display at least one WFSC image
  */
  *continueRunning = 1;
  *update = 1;

  printf("  PGplot_Master: sleeping 2 seconds\n");
  sleep(2);

  printf("  PGplot_Master: Trying to opening %s\n", path);
  status = PGplot_Open( Spots_Info, path, debug_PGplot);
  if ( status ) {
    printf("  PGplot_Master: Error opening %s", path);
    *running = 0;
    return( (void *) -1);
  }
  printf("opened\n");

  /*
    Do this elsewhere!!
  */
  //  Spots_Info->spots->values = NUM_PIXELS;

  //  Spots_Info->id = cpgopen("/xs");
  cpgslct(Spots_Info->id);
  cpgpap(5.0, 1.0);           /* paper size - width, aspect */
  printf("size\n");

  /*
    Setup Info Client Info structure and Gui variables
  */
  status = PGplot_SetupInfo( Spots_Info, "Spots", debug_PGplot);

  /*
    Do this elsewhere!!
  */
  Spots_Info->spots->values = 24;

  printf("Spots_Info->spots->values = %d\n", Spots_Info->spots->values);

  /*
    Load the "Heat" color table
  */
  status = PGplot_ColorTable( Spots_Info, 0, debug_PGplot);

  cpgqcir(&cir_low, &cir_high);
  printf("%d  %d\n", cir_low, cir_high);

  /*
    Make sure that a device has already been opened (PGplot_Open).  If
      so, then select that device
  */
  if ( Spots_Info->id <= 0) {
    printf("  PGplot_Master: Device not opened yet\n");
    *running = 0;
    return( (void *) -1);
  }
  cpgslct(Spots_Info->id);

  /*
    Open a socket the server, start the requested server and allocate needed memory
  */
  status = DClient_Spots_Init( &Spots_Single_Info, Spots_Info, "Spots", &WFSC_Array,
			       debug_PGplot, errorMsg);

  /*
    Setup scaling and transformation need to make the Spots image
  */
  status = PGplot_SpotsSetup( Spots_Info, debug_PGplot);
  if ( status ) {
    printf("  PGplot_Master: Error calling PGplot_SpotsSetup\n");
    *running = 0;
    return( (void *) -1);
  }

  /*
    Display a test pattern image
  */
  status = PGplot_SpotsPattern( Spots_Info, &WFSC_Array, debug_PGplot);
  if ( status ) {
    printf( "  PGplot_Master: Error with PGplot_SpotsPattern\n");
    *running = 0;
    return( (void *) -1);
  }

  status = PGplot_SpotsCalculate( Spots_Info, WFSC_Array, debug_PGplot);
  /*
  for(i=0; i<Spots_Info->spots->values*Spots_Info->spots->values; i++) {
    Spots_Info->spots->current[i] = (float) WFSC_Array[i];
  }
  */

  do {

    if ( *realtime || *update ) {

      if ( !Spots_Single_Info.server.connected ) {

	/*
	  Open a socket the server, start the requested server and allocate needed memory
	*/
	status = DClient_Spots_Init( &Spots_Single_Info, Spots_Info, "Spots", &WFSC_Array,
				     debug_PGplot, errorMsg);

	/*
	  Setup scaling and transformation need to make the Spots image
	*/
	status = PGplot_SpotsSetup( Spots_Info, debug_PGplot);
	if ( status ) {
	  printf("  PGplot_Master: Error calling PGplot_SpotsSetup\n");
	  *running = 0;
	  return( (void *) -1);
	}
      }

      status = DClient_WFSC_Single( &Spots_Single_Info, WFSC_Array, &frameNumber,
				    debug_local, errorMsg);
      status = PGplot_SpotsCalculate( Spots_Info, WFSC_Array, debug_PGplot);
      /*
	Set update flag to 0 so we only update once.  This flag
	  is set to 1 only by the AO GUI
      */
      *update = 0;

      /*
	Display an image
      */
      status = PGplot_SpotsUpdate( Spots_Info, Spots_Type, Spots_Slopes, Spots_Arrow, debug_PGplot);
      if ( status ) {
	printf("  PGplot_Master: Error calling PGplot_SpotsSetup\n");
	*running = 0;
	return( (void *) -1);
      }

    }

    usleep(10000);

  } while ( *continueRunning );

  if ( *debug ) {
    printf("  PGplot_Master: Stop signal received\n");
  }

  /*
    Send the "close" signal to xwin_server
  */
  //  cpgend();

  /*
    Let the calling process know that this thread has exited
  */
  *running = 0;

  return( (void *) 0);
}
