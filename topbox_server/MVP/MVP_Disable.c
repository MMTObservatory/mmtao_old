/*
  MVP_Disable.c

  This routine disables (allows motion of) the given device controlled
    by an MVP Controller

  Written  20aug07  DLM  Started with PCR_Process.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*=========================================
 * Disable the given device
 *=========================================*/

int MVP_Disable( Socket_Info *Socket, Device_Data *Data, int debug)
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
    Set the request string
  */
  strcpy( request, "0 DI");

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, debug) ) {
    if ( debug ) {
      printf("  MVP_Disable: Error sending disable request to %s\n", Socket->name);
      fflush(stdout);
    }
    /*
      Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
    */
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }

  /*
    The request to disable the motor driver cause junk to
      be output to the serial port after the driver is
      disabled.  Thus, for our MVP_Status (MVP_Read) command
      to work properly we must wait for the output to 
      show up on the port.
  */
  usleep(MVP_REPLY_WAIT);

  /* 
     Flush any unexpected input
  */
  Socket_Flush ( Socket->sockfd);

  /*
    Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
  */
  pthread_mutex_unlock( &Socket->lock);

  /*
    Request status string from the MVP motor driver and update the Data structure
  */
  status = MVP_Status( Socket, Data, local_debug);
  if ( status ) {
    if ( debug ) {
      printf("  MVP_Disable: Error retrieving status from %s\n", Socket->name);
      printf("               Connection Closed\n");
      fflush(stdout);
    }
    status = Socket_Close( Socket, local_debug);
    status = Status_ZeroFlags( Data);
    return(-1);
  }

  if ( Data->enabled ) {
    printf ("  MVP_Disable: Error disabling %s\n", Socket->name);
    fflush(stdout);
    MVP_PrintDevice( Socket, Data);
    return(-1);
  }

  return(0);
}

