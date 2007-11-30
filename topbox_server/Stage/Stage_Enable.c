/*
  Stage_Enable.c

  This routine enables the stage to move.

  Modified 26aug07  DLM  Started with Stage_Abort.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Stage.h"

/*=========================================
 * Enable the stage to move
 *=========================================*/

int Stage_Enable( Socket_Info *Socket, Device_Data *Data, int debug)
{
  int status;
  char request[STRING_LENGTH];
  int local_debug = 0;

  /*
    Lock the mutex variable "Device"->lock so other threads do not try
      to read or write while this thread is retrieving status information.
    If another thread has the variable "Device"->lock locked, this process
      is blocked until the mutex is unlocked.  Likewise, if the mutex
      variable is locked here, other processed are blocked until the mutex
      is unlock by this thread.
    In all other routines that communication on the "Device" port the
      same mutex lock function is called.
  */
  status = pthread_mutex_lock( &Socket->lock);

  /*
    Set the request string to enable the stage to move
  */
  strcpy( request, "DRVEN=1");

  /*
    Send the request
  */
  if ( Stage_Write( Socket, request, local_debug) ) {
    if ( debug ) {
      printf("  Stage_Enable: Error sending enable request to %s\n", Socket->name);
      fflush(stdout);
    }
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }

  pthread_mutex_unlock( &Socket->lock);

  if ( debug ) {
    printf("  Stage_Enable: %s Enabled\n", Socket->name);
    fflush(stdout);
  }

  return(0);
}

/* THE END */
