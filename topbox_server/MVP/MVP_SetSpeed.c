/*
  MVP_SetSpeed.c

  This routine sets the speed of the given device and them makes sure
    that it received the correct value

  Written  21aug07  DLM  Started with MVP_MoveRelative.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*=========================================
 * Set the device speed
 *=========================================*/

int MVP_SetSpeed( Socket_Info *Socket, Device_Data *Data, int speed, int debug)
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
    Set the string to set the speed
  */
  sprintf( request, "0 SP %d", speed);

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, debug) ) {
    if ( debug ) {
      printf("  MVP_SetSpeed: Error sending speed request to %s\n", Socket->name);
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
    Set the string to return the device speed
  */
  strcpy( request, "0 SP");

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, local_debug) ) {
    if ( debug ) {
      printf("  MVP_SetSpeed: Error sending speed return to %s\n", Socket->name);
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
    Get the status reply.  Must use a special MVP read instead of Socket_Read
  */
  status = MVP_Read( Socket, reply, local_debug);

  /*
    Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
  */
  pthread_mutex_unlock( &Socket->lock);

  if ( status ) {
    if ( debug ) {
      printf("  MVP_SetSpeed: Error retrieving requested speed from %s\n", Socket->name);
      fflush(stdout);
    }
    return(-1);
  }

  /*
    Check that a reply was received
  */
  if ( strlen( reply) < 9 ) {
    printf("  MVP_SetSpeed: Incorrect size of reply from MVP_Read\n");
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
    Check that the correct speed was received
  */
  if ( speed != temp ) {
    printf("  MVP_SetSpeed: Speed received by %s not correct\n", Socket->name);
    printf("                Sent %d\n", speed);
    printf("                Received %d\n", temp);
    fflush(stdout);
    return(-1);
  }
    
  return(0);
}

