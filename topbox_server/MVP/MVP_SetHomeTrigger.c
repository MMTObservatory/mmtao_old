/*
  MVP_SetHomeTrigger.c

  This routine sets the home trigger on the requested device.  Currently this
    is only the OAP motors

  Written  28aug07  DLM  Started with MVP_Enable.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*=========================================
 * Set the home trigger the given device
 *=========================================*/

int MVP_SetHomeTrigger( Socket_Info *Socket, Device_Data *Data, int trigger, int debug)
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
  sprintf( request, "0 HA %1d", trigger);

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, debug) ) {
    printf("  MVP_SetHomeTrigger: Error setting the home trigger on %s\n", Socket->name);
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

