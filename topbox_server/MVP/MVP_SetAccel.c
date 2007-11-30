/*
  MVP_SetAccel.c

  This routine sets the accel of the given device and them makes sure
    that it received the correct value

  Written  23aug07  DLM  Started with MVP_SetSpeed.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*=========================================
 * Set the device accel
 *=========================================*/

int MVP_SetAccel( Socket_Info *Socket, Device_Data *Data, int accel, int debug)
{
  int status;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  int local_debug = 0;
  int temp;

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
    Set the string to set the accel
  */
  sprintf( request, "0 AC %d", accel);

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, debug) ) {
    if ( debug ) {
      printf("  MVP_SetAccel: Error sending accel request to %s\n", Socket->name);
      printf("                Connection closed\n");
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
    Set the string to return the device accel
  */
  strcpy( request, "0 AC");

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, local_debug) ) {
    if ( debug ) {
      printf("  MVP_SetAccel: Error sending accel return to %s\n", Socket->name);
      printf("                Connection closed\n");
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
    Get the status reply.  Must use a special MVP read instead of Socket_Read
  */
  status = MVP_Read( Socket, reply, local_debug);

  /*
    Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
  */
  pthread_mutex_unlock( &Socket->lock);

  if ( status ) {
    if ( debug ) {
      printf("  MVP_SetAccel: Error retrieving requested accel from %s\n", Socket->name);
      fflush(stdout);
    }
    return(-1);
  }

  /*
    Check that a reply was received
  */
  if ( strlen( reply) < 9 ) {
    printf("  MVP_SetAccel: Incorrect size of reply from MVP_Read\n");
    printf("                Expected at least 9 characters\n");
    printf("                Received %d --> %s\n", (int) strlen( reply), reply);
    fflush(stdout);
    return(-1);
  }
    
  /*
    Retrieve the appropriate part of the message
  */
  sscanf ( reply + 5, "%x", &temp);

  /*
    Check that the correct accel was received
  */
  if ( accel != temp ) {
    printf("  MVP_SetAccel: Acceleration received by %s not correct\n", Socket->name);
    printf("                Sent %d\n", accel);
    printf("                Received %d\n", temp);
    fflush(stdout);
    return(-1);
  }
    
  return(0);
}

