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


  /* Getting position needs a dummy exchange before we get
   *  the real thing .. don't know why.
   */
  strcpy( request, "print pos\r");
  (void) Stage_Write ( Socket, request, local_debug );
  (void) Stage_Read ( Socket, reply, local_debug );

  /* Send the request, the second time for real.
  */
  strcpy( request, "print pos\r");
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

  Data->position = atof ( reply );

  if ( local_debug || debug ) {
    printf("  Stage_Position: reply = %s\n", reply );
    printf("  Stage_Position: Position in user = %f\n", Data->position );
    fflush(stdout);
  }

  return(0);
}

/* THE END */
