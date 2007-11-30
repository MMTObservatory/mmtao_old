/*
  MVP_Abort.c

  This routine abort the current move.  We then need to set
    the device back to local velocity mode in order to move
    the device again.

  Written  23aug07  DLM  Started with MVP_Disable.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*=========================================
 * Abort the current move
 *=========================================*/

int MVP_Abort( Socket_Info *Socket, Device_Data *Data, int debug)
{
  int status;
  char request[STRING_LENGTH];

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
  strcpy( request, "0 AB");

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, debug) ) {
    if ( debug ) {
      printf("  MVP_Abort: Error sending abort request to %s\n", Socket->name);
      fflush(stdout);
    }
    /*
      Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
    */
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }

  /*
    Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
  */
  pthread_mutex_unlock( &Socket->lock);

  return(0);

}

