/*
  Stage_MoveRelative.c

  This routine sends a relative move request to a device.

  Modified  24aug07  tjt  Started with MVP_MoveRelative.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Stage.h"

/*=========================================
 * Send a relative move request to the device
 *=========================================*/

int Stage_MoveRelative( Socket_Info *Socket, Device_Data *Data, double distance, int debug)
{
  char request[STRING_LENGTH];
  int local_debug = 0;

  (void) pthread_mutex_lock( &Socket->lock);

  /*
    Set the string to request a relative move
  */
  sprintf( request, "MOVR %.2f", distance * Data->gain);

  /*
    Send the request
  */
  if ( Stage_Write( Socket, request, local_debug) ) {
    if ( debug ) {
      printf("  Stage_MoveRelative: Error sending distance request to %s\n", Socket->name);
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
    printf("  Stage_MoveRelative: %s Relative Move of %f\n", Socket->name, distance);
    fflush(stdout);
  }

  return(0);
}

/* THE END */
