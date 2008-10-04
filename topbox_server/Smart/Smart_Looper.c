/*
  Smart_Looper.c

  This routine starts a process that request the status of the requested
    motor (Focus or Rotation) from the Smart motor controller at the given interval.

  Stolen  12sep07  tjt  Started with the MVP_Looper.c

*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Status.h"
#include "Smart.h"

#define DEBUG 0

static int
Smart_Reconnect ( Socket_Info *Socket, int debug )
{
  int status;
  int local_debug = 0;

  /* Open a socket to the requested device
   */
  status =  Socket_OpenClient ( Socket, local_debug );
  if ( status ) {
    if ( debug ) {
      printf("  Smart_Looper: Error connecting to the %s, %s:%d in Socket_Reconnect\n",
	     Socket->name, Socket->address, Socket->port);
      fflush(stdout);
    }
    return 0;
  }

  /* Initialize the communication to the Smart Motors.
   * This is a bit of work because they are daisy-chained.
   */
  status = Smart_Init ( Socket, local_debug );
  if ( status ) {
    if ( debug ) {
      printf("  Smart_Looper: Error initializing communication to the Smart Motors\n");
      fflush(stdout);
    }
    (void) Socket_Close ( Socket, local_debug );
    return 0;
  }

  return 1;
}

/*================================================================================*
 * Start status looper for the given motor, controlled by an Smart motor controller
 *================================================================================*/
void *
Smart_Looper( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  Device_Info *Info;
  Socket_Info *Socket;
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
  continueRunning = Info->continueRunning;
  running = Info->running;
  debug = Info->status_debug;

  /*
    The Smart Motors do not have enable mechanism so we set the enable
      flag to TRUE so that the gui will be able to move the smart motor
      (the gui checks the Data->enabled flag)
  */
  Info->data->enabled = 1;
  Info->data2->enabled = 1;

  /*
    Check if we are connected to this device.  If not, then try to connect
      and print a warning message on the terminal.  Inside the do-while loop
      warning messages are only printed if Info->debug is set to true.
  */
  if ( !Socket->connected ) {

    if ( ! Smart_Reconnect ( Socket, 1 ) ) {
      (void) Status_ZeroFlags ( Info->data );
      (void) Status_ZeroFlags ( Info->data2 );
    } else {
      /*
	The Smart Motors do not have enable mechanism so we set the enable
	  flag to TRUE so that the gui will be able to move the smart motor
	  (the gui checks the Data->enabled flag)
      */
      Info->data->enabled = 1;
      Info->data2->enabled = 1;
    }
      
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

    /* Check if we are connected to this device.  If so then request status flag
     */
    if ( Socket->connected ) {

      /* update the focus data structure (includes position)
      */
      status = Smart_Status( Socket, Info->data, *debug);
      if ( status ) {
	if ( *debug ) {
	  printf("  Smart_Looper: Error retrieving focus status from %s\n", Socket->name);
	  printf("              Connection Closed\n");
	  fflush(stdout);
	}
	continue;
      }

      /* update the rotation data structure (includes position)
      */
      status = Smart_Status( Socket, Info->data2, *debug);
      if ( status ) {
	if ( *debug ) {
	  printf("  Smart_Looper: Error retrieving rotation status from %s\n", Socket->name);
	  printf("              Connection Closed\n");
	  fflush(stdout);
	}
	continue;
      }

    } else {

      if ( local_debug ) {
	printf("  Smart_Looper: Not connected to %s\n", Socket->name );
	fflush(stdout);
      }

      if ( ! Smart_Reconnect ( Socket, *debug ) ) {
        (void) Status_ZeroFlags ( Info->data );
        (void) Status_ZeroFlags ( Info->data2 );
      } else {
	/*
	  The Smart Motors do not have enable mechanism so we set the enable
	    flag to TRUE so that the gui will be able to move the smart motor
	    (the gui checks the Data->enabled flag)
	*/
	Info->data->enabled = 1;
	Info->data2->enabled = 1;
      }

      /* Pause for a bit before getting the status
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

      printf("  Smart_Looper: %s running at %4.1f Hz\n", Socket->name, (float) samples/total_time);
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
    usleep(SMART_LOOPER_WAIT);

  } while ( *continueRunning );

  if ( *debug ) {
    printf("  Smart_Master: Smart Server Shutdown\n");
    fflush(stdout);
  }

  return( (void *)0);
}

/* THE END */
