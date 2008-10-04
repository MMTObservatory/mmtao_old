/*
  Fwheel_Looper.c

  This routine starts a process that checks the connect status
  of the filter wheel.

  Modified  29aug07  tjt  Started with Stage_Looper.c

*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Status.h"
#include "Fwheel.h"

#define DEBUG 0

void *
Fwheel_Looper( void *Passed_Info)
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
  if ( ! Socket->connected ) {

    /*
      Open a socket to the requested device
    */
    status =  Socket_OpenClient ( Socket, *debug);
    if ( status ) {
      printf("  Fwheel_Looper: Error connecting to the %s, %s:%d in Socket_OpenClient\n",
	     Socket->name, Socket->address, Socket->port);
      fflush(stdout);
    }

    /*
      Pause for a bit before getting the status
    */
    sleep(1);
  }

#if( DEBUG )
  /*
    Start timer to determine the rate at which status information is coming from the device
  */
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
    if ( ! Socket->connected ) {

      if ( local_debug ) {
	printf("  Fwheel_Looper: Not connected to %s\n", Socket->name );
	fflush(stdout);
      }

      /*
	Open a socket to the requested device
      */
      status =  Socket_OpenClient ( Socket, local_debug);
      if ( status ) {
	if ( local_debug ) {
	  printf("  Fwheel_Looper: Error connecting to the %s, %s:%d in Socket_OpenClient\n",
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

      printf("  Fwheel_Looper: %s running at %4.1f Hz\n", Socket->name, (float) samples/total_time);
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
    usleep(FWHEEL_LOOPER_WAIT);

  } while ( *continueRunning );

  if ( *debug ) {
    printf("  Fwheel_Master: Fwheel Server Shutdown\n");
    fflush(stdout);
  }

  return( (void *)0);
}

/* THE END */
