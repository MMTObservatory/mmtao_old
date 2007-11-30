/*
  Stage_SetZero.c

  This routine sets the current position of the device to 0

  Modified  24aug07  tjt  Started with MVP_SetZero.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Stage.h"

/*=========================================
 * Set the current position to zero
 *=========================================*/

int Stage_SetZero( Socket_Info *Socket, Device_Data *Data, int debug)
{
  int status;
  char request[STRING_LENGTH];
  int local_debug = 0;

  status = pthread_mutex_lock( &Socket->lock);

  /*
    Set the string to set the position to zero
  */
  strcpy( request, "POS=0.00");

  /*
    Send the request
  */
  if ( Stage_Write( Socket, request, local_debug) ) {
    if ( debug ) {
      printf("  Stage_SetZero: Error sending zero request to %s\n", Socket->name);
      printf("                    Connection closed\n");
      fflush(stdout);
    }
    status = Socket_Close( Socket, local_debug);
    status = Status_ZeroFlags( Data);
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }

  pthread_mutex_unlock( &Socket->lock);

  if ( debug ) {
    printf("  Stage_SetZero: %s position set to 0.0\n", Socket->name );
    fflush(stdout);
  }

  return(0);
}

/* THE END */
