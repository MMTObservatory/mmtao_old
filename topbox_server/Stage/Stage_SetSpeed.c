/*
  Stage_SetSpeed.c

  This routine sets the speed of the given device and them makes sure
    that it received the correct value

  Modified  24aug07  tjt  Started with MVP_SetSpeed.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Stage.h"

/*=========================================
 * Set the device speed
 *=========================================*/

int Stage_SetSpeed( Socket_Info *Socket, Device_Data *Data, double speed, int debug)
{
  char request[STRING_LENGTH];
  int local_debug = 0;

  (void) pthread_mutex_lock( &Socket->lock);

  /*
    Set the string to set the speed
  */
  sprintf( request, "VM=%.2f", speed);

  /*
    Send the request
  */
  if ( Stage_Write( Socket, request, local_debug) ) {
    if ( debug ) {
      printf("  Stage_SetSpeed: Error sending speed request to %s\n", Socket->name);
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
    printf("  Stage_SetSpeed: %s speed set to %.0f\n", Socket->name, speed);
    fflush(stdout);
  }

  return(0);
}

/* THE END */
