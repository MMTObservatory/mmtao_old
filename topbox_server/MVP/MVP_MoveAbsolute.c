/*
  MVP_MoveAbsolute.c

  This routine sends a absolute move request to a device, checks
    that it received the correct value and then sends a move request

  Written  21aug07  DLM  Started with MVP_Position.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*=========================================
 * Send a absolute move request to the device
 *=========================================*/

int MVP_MoveAbsolute( Socket_Info *Socket, Device_Data *Data, float distance, int debug)
{
  int status;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  int local_debug = 1;
  int value, temp;

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
    Set the string to request an absolute move
  */
  value = (int) (distance * Data->gain);
  sprintf( request, "0 LA %d", value);

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, debug) ) {
    if ( debug ) {
      printf("  MVP_MoveAbsolute: Error sending distance request to %s\n", Socket->name);
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
    Set the string to return the requested move
  */
  strcpy( request, "0 LA");

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, local_debug) ) {
    if ( debug ) {
      printf("  MVP_MoveAbsolute: Error sending distance return to %s\n", Socket->name);
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
  if ( status ) {
    if ( debug ) {
      printf("  MVP_MoveAbsolute: Error retrieving requested distance from %s\n", Socket->name);
      fflush(stdout);
    }
    /*
      Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
    */
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }

  /*
    Check that a reply was received
  */
  if ( strlen( reply) < 12 ) {
    if ( debug ) {
      printf("  MVP_MoveAbsolute: Incorrect size of reply from MVP_Read\n");
      printf("                    Expected at least 12 characters\n");
      printf("                    Received %d --> %s\n", (int) strlen( reply), reply);
      fflush(stdout);
    }
    /*
      Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
    */
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }
    
  /*
    Retrieve the appropriate part of the message
  */
  sscanf ( reply + 5, "%x", &temp);

  /*
    Check that the correct distance was received
  */
  if ( value != temp ) {
    printf("  MVP_MoveAbsolute: Distance received by %s not correct\n", Socket->name);
    printf("                    Sent %d\n", value);
    printf("                    Received %d\n", temp);
    fflush(stdout);
    /*
      Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
    */
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }
    
  /*
    Set the string to request the device to apply the move
  */
  strcpy( request, "0 M");

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, debug) ) {
    if ( debug ) {
      printf("  MVP_MoveAbsolute: Error sending move request to %s\n", Socket->name);
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

