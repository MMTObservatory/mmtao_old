/*===========================================================================*
**
**  MVP_Status.c
**
**  Routines to retrive information from an MVP device controller
**
**  Returns :  0 success
**            -1 error
**
**  Written  14aug07  DLM  Started with FSM_Status.
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*=========================================
 * function: MVP_Status
 *=========================================*/

int MVP_Status( Socket_Info *Socket, Device_Data *Data, int debug)
{
  int i;
  short status;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];

  /*
    Lock the mutex variable Socket->lock so other threads do not try
      to read or write while this thread is retrieving status information.
    If another thread has the variable Socket->lock locked, this process
      is blocked until the mutex is unlocked.  Likewise, if the mutex
      variable is locked here, other processed are blocked until the mutex
      is unlock by this thread.
    In all other routines that communicates to this device, same mutex lock
      function is called.
  */
  pthread_mutex_lock( &Socket->lock);

  /*
    Set the request string
  */
  strcpy( request, "0 ST");

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, debug) ) {
    if ( debug ) {
      printf("  MVP_Status: Error sending status request to %s\n", Socket->name);
      printf("              Connection closed\n");
      fflush(stdout);
    }
    status = Socket_Close( Socket, debug);
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
  if ( MVP_Read( Socket, reply, debug) ) {
    if ( debug ) {
      printf("  MVP_Status: Error reading status reply from %s\n", Socket->name);
      fflush(stdout);
    }
    /*
      Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
    */
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }

  /*
    Unlock the mutex variable "Device"->lock so other threads may use the
      "Device" port
  */
  pthread_mutex_unlock( &Socket->lock);

  /*
    Check that a reply was received
  */
  if ( strlen( reply) < 9 ) {
    if ( debug ) {
      printf("  MVP_Status: Incorrect size of reply from %s\n", Socket->name);
      printf("              Expected at least 9 characters\n");
      printf("              Received %d --> %s\n", (int) strlen( reply), reply);
      fflush(stdout);
    }
    return(-1);
  }
    
  /*
    Retrieve the appropriate part of the message
  */
  sscanf ( reply + 5, "%x", &i);
  status = (short) i;

  /*
    Decrypt the status word and put into MVP_State structure
  */
  Data->moving = (status & MVP_MOVE_IN_PROGRESS)?1:0;
  Data->in_position = (status & MVP_IN_POSITION)?1:0;
  Data->error = (status & MVP_ERROR)?1:0;
  Data->enabled = (status & MVP_NOT_ENABLED)?0:1;
  Data->positive = (status & MVP_POSITIVE_LIMIT)?1:0;
  Data->negative = (status & MVP_NEGATIVE_LIMIT)?1:0;
  Data->event_2 = (status & MVP_EXTERNAL_EVENT_2)?1:0;

  if ( debug ) {
    MVP_PrintDevice( Socket, Data);
  }

  return(0);
}
