/*
  MVP_Looper.c

  This routine starts a process that request the status of the requested
    motor (FSM or OAP) from the MVP motor controller at the given interval.

  Written  14aug07  DLM  Started with the Status_Master.c from pcr_server

*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

#define DEBUG 0

/*================================================================================*
 * Start status looper for the given motor, controlled by an MVP motor controller
 *================================================================================*/
void *MVP_Looper( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  Device_Info *Info;
  Socket_Info *Socket;
  Device_Data *Data;
  int *continueRunning;
  int *running;
  int *debug;

  int status;
  int loops = 0;
  int local_debug = 0;

  /* Timing variables */

#if( DEBUG )
  struct timeval *systime;
  int start_sec, start_usec;
  int end_sec, end_usec;
  float total_time;
  int count = 0;
  int samples = 10;
#endif

  /*
    Put the passed variables in to the local variables
  */
  Info = (Device_Info *)Passed_Info;
  Socket = Info->socket;
  Data = Info->data;
  continueRunning = Info->continueRunning;
  running = Info->running;
  debug = Info->status_debug;

  /*
    Check if we are connected to this device.  If not, then try to connect
      and print a warning message on the terminal.  Inside the do-while loop
      warning messages are only printed if Info->debug is set to true.
  */
  if ( !Socket->connected ) {

    /*
      Open a socket to the requested device
    */
    status =  Socket_OpenClient ( Socket, *debug);
    if ( status ) {
      printf("  MVP_Looper: Error connecting to the %s, %s:%d in Socket_OpenClient\n",
	     Socket->name, Socket->address, Socket->port);
      fflush(stdout);
    }

    /*
      Pause for a bit before getting the status
    */
    sleep(1);

   }

  /*
    Start timer to determine the rate at which status information is coming from the device
  */
#if( DEBUG )
  systime = (struct timeval *)malloc( sizeof(struct timeval) );
  status = gettimeofday(systime, NULL);
  start_sec = systime->tv_sec;
  start_usec = systime->tv_usec;
  count = 0;
#endif

  /*=====================================================================
    Start loop to retrieve status information from the requested device
    =======================================================================*/
  status = 0;

  do {

    /*
      Check if we are connected to this device.  If so then request status flag
    */
    if ( Socket->connected ) {

      /*
	Request the current position of the given device
      */
      status = MVP_Position( Socket, Data, *debug);
      if ( status ) {
	if ( *debug ) {
	  printf("  MVP_Looper: Error retrieving position from %s\n", Socket->name);
	  printf("              Connection Closed\n");
	  fflush(stdout);
	}
	status = Socket_Close( Socket, local_debug);
	status = Status_ZeroFlags( Data);
	continue;
      }

      /*
	Request status string from the MVP motor driver and update the Data structure
      */
      status = MVP_Status( Socket, Data, *debug);
      if ( status ) {
	if ( *debug ) {
	  printf("  MVP_Looper: Error retrieving status from %s\n", Socket->name);
	  printf("              Connection Closed\n");
	  fflush(stdout);
	}
	status = Socket_Close( Socket, local_debug);
	status = Status_ZeroFlags( Data);
	continue;
      }

      if ( loops >= 4 ) {

	loops = 0;

	/*
	  Request home flag from the MVP motor driver and update the Data structure.
	    Make this request every 4th loop so we don't slow down the looper.  We do
	    this because the home flag is not used to make any decisions in other
	    MVP routines where as the status and position values are.
	*/
	status = MVP_HomeFlag( Socket, Data, *debug);
	if ( status ) {
	  if ( *debug ) {
	    printf("  MVP_Looper: Error retrieving home flag from %s\n", Socket->name);
	    printf("              Connection Closed\n");
	    fflush(stdout);
	  }
	  status = Socket_Close( Socket, local_debug);
	  status = Status_ZeroFlags( Data);
	  continue;
	}
      }

      loops++;

    } else {

      if ( local_debug ) {
	printf("  MVP_Looper: Not connected to %s\n", Socket->name );
	fflush(stdout);
      }

      /*
	Open a socket to the requested device
      */
      status =  Socket_OpenClient ( Socket, local_debug);
      if ( status ) {
	if ( local_debug ) {
	  printf("  MVP_Looper: Error connecting to the %s, %s:%d in Socket_OpenClient\n",
		 Socket->name, Socket->address, Socket->port);
	  fflush(stdout);
	}
      }

      /*
	Pause for a bit before getting the status
      */
      sleep(1);

    }


#if( DEBUG )
    count++;
    if ( count >= samples ) {
      
      /*
	Calculate time spent retrieving the status
      */
      status = gettimeofday(systime, NULL);
      end_sec = systime->tv_sec;
      end_usec = systime->tv_usec;
      total_time = (float) (end_sec-start_sec) + (float) (end_usec-start_usec) /1.0e6 ;

      printf("  MVP_Looper: %s running at %4.1f Hz\n", Socket->name, (float) samples/total_time);
      fflush(stdout);

      /*
	Reset the counter and the timer
      */
      systime = (struct timeval *)malloc( sizeof(struct timeval) );
      status = gettimeofday(systime, NULL);
      start_sec = systime->tv_sec;
      start_usec = systime->tv_usec;
      count = 0;
    }
#endif

    /*
      Set the refresh rate of the status looper by sleeping this thread
    */
    usleep(MVP_LOOPER_WAIT);

  } while ( *continueRunning );

  if ( *debug ) {
    printf("  MVP_Master: MVP Server Shutdown\n");
    fflush(stdout);
  }

  return( (void *)0);
}
