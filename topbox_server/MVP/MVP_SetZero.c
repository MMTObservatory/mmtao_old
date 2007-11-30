/*
  MVP_SetZero.c

  This routine sets the current position of the device to 0

  Written  23aug07  DLM  Started with MVP_SetSpeed.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*=========================================
 * Set the current position to zero
 *=========================================*/

int MVP_SetZero( Socket_Info *Socket, Device_Data *Data, int debug)
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
    Set the string to set the position to zero
  */
  strcpy( request, "0 HO");

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, debug) ) {
    if ( debug ) {
      printf("  MVP_SetZero: Error sending zero request to %s\n", Socket->name);
      printf("                    Connection closed\n");
      fflush(stdout);
    }
    status = Socket_Close( Socket, local_debug);
    status = Status_ZeroFlags( Data);
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

