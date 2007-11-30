/*
  MVP_Position.c

  This routine queries the given device for its current position

  Written  20aug07  DLM  Started with MVP_Enable.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*=========================================
 * Request the position of the device
 *=========================================*/

int MVP_Position( Socket_Info *Socket, Device_Data *Data, int debug)
{
  int status;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  int local_debug = 0;
  int position;
  float temp;

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
  strcpy( request, "0 POS");

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, local_debug) ) {
    if ( debug ) {
      printf("  MVP_Position: Error sending position request to %s\n", Socket->name);
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
    Any request the expects a reply string must wait a bit for junk to
      be output to the serial port after the driver is
      enabled.  Thus, for our MVP_Status (MVP_Read) command
      to work properly we must wait for the output to 
      show up on the port.
  */
  //  usleep(MVP_REPLY_WAIT);

  /*
    Get the status reply.  Must use a special MVP read instead of Socket_Read
  */
  status = MVP_Read( Socket, reply, local_debug);

  /*
    Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
  */
  pthread_mutex_unlock( &Socket->lock);

  /*
    If there was an error reading the reply string, then close the connection and
      report the error
  */
  if ( status ) {
    if ( debug ) {
      printf("  MVP_Position: Error retrieving position from %s\n", Socket->name);
      printf("                Connection Closed\n");
      fflush(stdout);
    }
    return(-1);
  }

  /*
    Check that a reply was received
  */
  if ( strlen( reply) < 13 ) {
    if ( debug ) {
      printf("  MVP_Position: Incorrect size of reply from MVP_Read\n");
      printf("                Expected at least 13 characters\n");
      printf("                Received %d --> %s\n", (int) strlen( reply), reply);
      fflush(stdout);
    }
    return(-1);
  }
    
  /*
    Retrieve the appropriate part of the message
  */
  sscanf ( reply + 5, "%x", &position);

  /*
    Place the value in user units in the device structure
  */
  temp = (int) ( (double)position / Data->gain * 1000.0 );
  Data->position = (double)temp / 1000.0;

  if ( local_debug || debug ) {
    printf("  MVP_Position: Position in user = %f\n", Data->position);
    fflush(stdout);
  }

  if ( local_debug ) {
    printf("                          device = %d\n", position);
    printf("                     hex (reply) = %x\n", position);
    fflush(stdout);
  }

  return(0);
}

