/*
  DClient_Spots_Display.c

  Routine to display a WFSC Spots image.  The steps to display an image are:
    - Check to see if we are connected to the PCR_Server.  If not:
        - Connect
	- Allocate needed memory
    - Check to see if the size of the Spots image has changed.  If so:
        - Reallocate needed memory
	- Set up needed PGplot variable (scale, transformation, etc)
    - Reqest a Spots image from the PCR_Server
    - Display the imbedded PGplot image

  Written   9jun07  DLM  Moved the 6 or so routine calls from DClient_Cmd
                           so it could be called for either WFSC images from
			   either the WFSC PCR port or from the DDS PCR port
*/

#include "PCR_Wish.h"
#include "DClient.h"
#include "PGplot_Globals.h"

#define TIMER 0

int DClient_Spots_Display( DClient_Info *Client_Info, PGplot_Info *Spots_Info, char *name, char *type,
			int debug_DClient, int debug_PGplot, char *errorMsg)
{

  int status, retVal;
  int frameNumber;

  static short *Spots_Array;

  /* Timing variables */
#if( TIMER )
  static count = 0;
  static struct timeval *systime;
  static int start_sec, start_usec;
  int end_sec, end_usec;
  float total_time;
#endif

/*************************************************************************
 **
 ** Check to see what to do
 **
 *************************************************************************/

  if ( !strncmp( type, "single", strlen("single")) ) {

#if( TIMER )
    if ( count == 0 ) {
      systime = (struct timeval *)malloc( sizeof(struct timeval) );
      status = gettimeofday(systime, NULL);
      start_sec = systime->tv_sec;
      start_usec = systime->tv_usec;
    }
    count++;
#endif

    if ( !Client_Info->server.connected ) {

      /*
	Open a socket the server, start the requested server and allocate needed memory
      */
      status = DClient_Spots_Init( Client_Info, Spots_Info, name, &Spots_Array,
				   debug_DClient, errorMsg);
      if ( status ) {
	return (status);
      }

      /*
	Set the scaling and transformation need for the current Spots image
	  size (72x72 or 24x24)
      */
      status = PGplot_SpotsSetup( Spots_Info, debug_PGplot);
      if ( status ) {
	return (status);
      }

      if ( debug_DClient ) {
	printf("  DClient_Spots_Display: Connected to Spots server\n");
      }

    }

    retVal = DClient_Spots_Single( Client_Info, Spots_Array, &frameNumber,
				   debug_DClient, errorMsg);

    if ( retVal < 0 ) {
      
      if ( retVal == -1 ) {

	status = Server_Close( (Server_Info *)Client_Info, debug_DClient);

      } else if ( retVal == -11 ) {
	/*
	  WFSC size has changed.  Re-Initialize variables used here and return
	    NOTE: WFSC data is not retreived on this call.  WFSC:Single must
	          be requested again after re-initialization to get a frame
	*/
	status = DClient_Spots_Init( Client_Info, Spots_Info, name, &Spots_Array,
				     debug_DClient, errorMsg);
	if ( status ) {
	  return (status);
	}

	/*
	  Set the scaling and transformation need for the current Spots image
	    size (72x72 or 24x24)
	*/
	status = PGplot_SpotsSetup( Spots_Info, debug_PGplot);
	if ( status ) {
	  return (status);
	}

	return(retVal);

      } else  {

	return(retVal);

      }

    } else if ( retVal == 0 ) {

      /*
	If we get to here, then a new frame has been received.  Increment
	  the "running" array.  There are 8 characters that are displayed
	  as a new frame is display (|/-\|/-\) so the counter is reset to
	  0 when it becomes greater than 7.  The 8 characters are defined
	  in the the tcl file ./tcl/pgplot/pgplot_globals_set.tcl
      */
      New_Spot++;
      if (New_Spot> 7) New_Spot = 0;

      Spots_Number = frameNumber;

      /*========================================
	Select the PGPLOT device for Spots and set new page.  cpgpage() is required for a
	  new page to be updated by /xtk.  However, if PGplot_SpotsUpdate is called
	  by a separate thread, a call to cpgpage() will cause the display to flicker
	  ========================================*/
      cpgslct( Spots_Info->id);
      cpgpage();

      status = PGplot_SpotsCalculate( Spots_Info, Spots_Array, debug_PGplot);
      if ( status ) {
	return (status);
      }

      status = PGplot_SpotsUpdate( Spots_Info, Spots_Type, Spots_Slopes, Spots_Arrow, debug_PGplot);
      if ( status ) {
	return (status);
      }

    }
#if( TIMER )

    if ( count > 100 ) {
      /*
	Calculate time spent retrieving data
      */
      status = gettimeofday(systime, NULL);
      end_sec = systime->tv_sec;
      end_usec = systime->tv_usec;
      total_time = (float) (end_sec-start_sec) + (float) (end_usec-start_usec) /1.0e6 ;
      printf(" %d frames in %f seconds (%3.0f Hz)\n", count, total_time,
	     (float) count/total_time);
      count = 0;
    }
#endif

    return(retVal);

  } else if ( !strncmp( type, "close", strlen("close")) ) {

    if ( Client_Info->server.connected ) {

      /*
	Send quit to server on VME
      */
      strcpy( Client_Info->server.out_line, "Quit\n");
      status = Server_Write( (Server_Info *)Client_Info);

      /*
	Close the local connection
      */
      status = Server_Close( (Server_Info *)Client_Info, debug_DClient);
    }

  } else {

    sprintf( errorMsg, "Invalid %s request %s", name, type);
    return(-1);

  }

  return(0);

}
