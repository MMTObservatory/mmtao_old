/*
  MVP_ResetNode.c

  This routine sends a reset node signal to the MVP controller.  This
    reset puts the into its default state (device disabled and not homed).

  Written  24aug07  DLM  Started with MVP_Abort.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*=========================================
 * Reset the requested MVP controller
 *=========================================*/

int MVP_ResetNode( Socket_Info *Socket, Device_Data *Data, int debug)
{
  int status;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  int local_debug = 0;

  struct timeval delay;
  fd_set mySet;

  /*
    Read data from the motor (open socket => Socket->sockfd)
      => select is used to wait for the requested data to become
         available.
      => if nothing to read, return(1)
  */
  FD_ZERO (&mySet);
  FD_SET (Socket->sockfd, &mySet);
  delay.tv_sec  = 10;
  delay.tv_usec = 0;

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
  strcpy( request, "0 RN");

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, debug) ) {
    if ( debug ) {
      printf("  MVP_ResetNode: Error sending reset request to %s\n", Socket->name);
      fflush(stdout);
    }
    /*
      Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
    */
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }

  /*
    Wait for the reset node to send back the string "MVP 2001 Ready"
  */
  if (!select (FD_SETSIZE, &mySet, NULL, NULL, &delay) ) {
    if ( debug ) {
      printf("  MVP_ResetNode: Reset of %s timed out in %d seconds\n", Socket->name, MVP_DELAY_S);
      fflush(stdout);
    }
    return(-1);
  }

  /*
    Read the Reset Node string "0000 00001D28\n MVP 2001 Ready"
  */
  status = MVP_Read( Socket, reply, local_debug);
  printf("  MVP_ResetNode: %s reset string \"MVP 2001 Ready\"\n", Socket->name);

  /*
    Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
  */
  pthread_mutex_unlock( &Socket->lock);

  return(0);

}

