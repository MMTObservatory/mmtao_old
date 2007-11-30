/*
  Stage_MoveAbsolute.c

  This routine sends a absolute move request to a device, checks
    that it received the correct value and then sends a move request

  Modified  24aug07  tjt  Started with MVP_MoveAbsolute.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Stage.h"

/*=========================================
 * Send a absolute move request to the device
 *=========================================*/

int Stage_MoveAbsolute( Socket_Info *Socket, Device_Data *Data, double distance, int debug)
{
  char request[STRING_LENGTH];
  int local_debug = 1;

  (void) pthread_mutex_lock( &Socket->lock);

  /* Set the string to request an absolute move
  */
  sprintf( request, "mova %f", distance * Data->gain);

  /*
    Send the request
  */
  if ( Stage_Write( Socket, request, local_debug) ) {
    if ( debug ) {
      printf("  Stage_MoveAbsolute: Error sending distance request to %s\n", Socket->name);
      printf("                    Connection closed\n");
      fflush(stdout);
    }
    (void) Socket_Close( Socket, local_debug);
    (void) Status_ZeroFlags( Data);
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }

  pthread_mutex_unlock( &Socket->lock);

  if ( debug ) {
    printf("  Stage_MoveAbsolute: %s Absolute Move of %f\n", Socket->name, distance);
    fflush(stdout);
  }

    return(0);
}

/* THE END */
