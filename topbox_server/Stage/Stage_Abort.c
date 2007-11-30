/*
  Stage_Abort.c

  This routine abort the current move.  We then need to set
    the device back to local velocity mode in order to move
    the device again.

  Modified 24aug07  tjt  Started with MVP_Abort.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Stage.h"

/*=========================================
 * Abort the current move
 *=========================================*/

int Stage_Abort( Socket_Info *Socket, Device_Data *Data, int debug)
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
    Set the request string to abort the current move
  */
  strcpy( request, "abort");

  /*
    Send the request
  */
  if ( Stage_Write( Socket, request, local_debug) ) {
    if ( debug ) {
      printf("  Stage_Abort: Error sending disable request to %s\n", Socket->name);
      fflush(stdout);
    }
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }

  pthread_mutex_unlock( &Socket->lock);

  if ( debug ) {
    printf("  Stage_Abort: %s motion Aborted\n", Socket->name);
    fflush(stdout);
  }

  return(0);
}

/* THE END */
