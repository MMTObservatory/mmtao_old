/*
  Stage_SetHomeVar.c

  Written 24aug07  tjt

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Stage.h"

int Stage_SetHomeVar( Socket_Info *Socket, Device_Data *Data, double data, int debug)
{
  char request[STRING_LENGTH];
  int local_debug = 0;

  if ( !( (data == 0.0) || (data == 1.0) ) ) {

    printf("  Stage_SetHomeVar: Home flag should be 0.0 or 1.0\n");
    printf("                    Found %.1f\n", data);
    fflush(stdout);
    return(-1);

  }

  (void) pthread_mutex_lock( &Socket->lock);

  /* We used to think this only works if it
   * was done twice (like position), but now we aren't
   * so sure, but are leaving it in.  
   * Probably this whack it twice can be removed.
   */
  sprintf( request, "HOME=%.1f", data );
  if ( Stage_Write( Socket, request, local_debug) < 0) {
    pthread_mutex_unlock( &Socket->lock);
    return -1;
  }

  sprintf( request, "HOME=%.1f", data );
  if ( Stage_Write( Socket, request, local_debug) < 0) {
    pthread_mutex_unlock( &Socket->lock);
    return -1;
  }

  pthread_mutex_unlock( &Socket->lock);

  if ( debug ) {
    printf("  Stage_SetHomeVar: %s HOME = %.1f\n", Socket->name, data );
    fflush(stdout);
  }

  return 0;
}

/* THE END */
