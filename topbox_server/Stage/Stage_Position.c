/*
  Stage_Position.c

  This routine queries the given device for its current position

  Modified  24aug07  tjt  Started with MVP_Position.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Stage.h"

/*=========================================
 * Request the position of the device
 *=========================================*/

int Stage_Position( Socket_Info *Socket, Device_Data *Data, int debug)
{
  int status;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  int local_debug = 0;

  (void) pthread_mutex_lock( &Socket->lock);

  /*
    Set the request string
  */
  strcpy( request, "PRINT POS");

  /*
    Send the request
  */
  if ( Stage_Write( Socket, request, local_debug) ) {
    if ( debug ) {
      printf("  Stage_Position: Error sending position request to %s\n", Socket->name);
      printf("                Connection closed\n");
      fflush(stdout);
    }
    (void) Socket_Close( Socket, local_debug);
    (void) Status_ZeroFlags( Data);
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }
  status = Stage_Read( Socket, reply, local_debug);

  pthread_mutex_unlock( &Socket->lock);

  /*
    If there was an error reading the reply string, then close the connection and
      report the error
  */
  if ( status ) {
    if ( debug ) {
      printf("  Stage_Position: Error retrieving position from %s\n", Socket->name);
      printf("                Connection Closed\n");
      fflush(stdout);
    }
    return(-1);
  }

  Data->position = atof ( reply ) / Data->gain;

  if ( debug ) {
    printf("  Stage_Position: Position = %f\n", Data->position );
    fflush(stdout);
  }

  return(0);
}

/* THE END */
