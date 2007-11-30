/*
  MVP_HardStop.c

  This routine tells the device to do a hard stop when the homing flag (event_2)
    is triggered.

  Written  28aug07  DLM  Started with MVP_SetHomeTrigger.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*=========================================
 * Set the device to perform a hard stop
 *=========================================*/

int MVP_HardStop( Socket_Info *Socket, Device_Data *Data, int debug)
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
    Set the request string
  */
  strcpy( request, "0 HF 1");

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, debug) ) {
    printf("  MVP_HardStop: Error setting hard stop on %s\n", Socket->name);
    fflush(stdout);
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

