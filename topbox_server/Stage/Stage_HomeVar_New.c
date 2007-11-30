/*
  Stage_Homevar.c

  Written 24aug07  tjt

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Stage.h"

int
Stage_HomeVar( Socket_Info *Socket, Device_Data *Data, int debug)
{
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  int local_debug = 0;

  /*
    Query if the HOME flag has been set.  If not, set it
      to 0.0 (not homed)
  */
  strcpy( request, "print_uvars");

  (void) pthread_mutex_lock( &Socket->lock);

  if ( Stage_Write( Socket, request, local_debug) < 0) {
    pthread_mutex_unlock( &Socket->lock);
    return -1;
  }
  if ( Stage_Read( Socket, reply, local_debug) < 0) {
    pthread_mutex_unlock( &Socket->lock);
    return -1;
  }

  pthread_mutex_unlock( &Socket->lock);

  if ( strstr(reply, "HOME") != NULL )
    return 0;

  strcpy(request, "var HOME=0.0");

  (void) pthread_mutex_lock( &Socket->lock);
  if ( Stage_Write( Socket, request, local_debug) < 0) {
    pthread_mutex_unlock( &Socket->lock);
    return -1;
  }

  pthread_mutex_unlock( &Socket->lock);
  return 0;
}

int
Stage_SetHomeVar( Socket_Info *Socket, Device_Data *Data, double data, int debug)
{
  char request[STRING_LENGTH];

  (void) pthread_mutex_lock( &Socket->lock);

  /* Only "sticks" if you do it twice */
  sprintf( request, "set HOME=%f", data );
  if ( Stage_Write( Socket, request, debug) < 0) {
    pthread_mutex_unlock( &Socket->lock);
    return -1;
  }

  sprintf( request, "set HOME=%f", data );
  if ( Stage_Write( Socket, request, debug) < 0) {
    pthread_mutex_unlock( &Socket->lock);
    return -1;
  }

  pthread_mutex_unlock( &Socket->lock);
  return 0;
}

/* THE END */
