/*===========================================================================*
**
**  Stage_Status.c
**
**  Routines to retrive information from an Stage device controller
**
**  Returns :  0 success
**            -1 error
**
**  Modified  24aug07  tjt  Started with MVP_Status.c
**                          Added get_thing() routine
**
**  Altered   26aug07  DLM  Added a check of the enable flag
**                          Removed set from "set HOME=*"
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Status.h"
#include "Stage.h"

static int
get_thing ( Socket_Info *Socket, Device_Data *Data, char *request, char *reply, int debug )
{
  char rqst[STRING_LENGTH];

  pthread_mutex_lock( &Socket->lock);

  strcpy( rqst, request);

  if ( Stage_Write( Socket, rqst, debug) ) {
    if ( debug ) {
      printf("  Stage_Status: Error sending status request to %s\n", Socket->name);
      printf("              Connection closed\n");
      fflush(stdout);
    }
    (void) Socket_Close( Socket, debug);
    (void) Status_ZeroFlags( Data);
    pthread_mutex_unlock( &Socket->lock);
    return -1;
  }

  if ( Stage_Read( Socket, reply, debug) ) {
    if ( debug ) {
      printf("  Stage_Status: Error reading status reply from %s\n", Socket->name);
      fflush(stdout);
    }
    pthread_mutex_unlock( &Socket->lock);
    return -1;
  }

  pthread_mutex_unlock( &Socket->lock);
  return 0;
}

/*=========================================
 * function: Stage_Status
 *=========================================*/

int Stage_Status( Socket_Info *Socket, Device_Data *Data, int debug)
{
  char reply[STRING_LENGTH];
  int local_debug = 0;

  if ( get_thing ( Socket, Data, "PRINT DRVEN", reply, local_debug ) < 0 )
    return -1;
  Data->enabled = strncmp(reply, "TRUE", 4) ? 0 : 1;

  if ( get_thing ( Socket, Data, "PRINT MVG", reply, local_debug ) < 0 )
    return -1;
  Data->moving = strncmp(reply, "TRUE", 4) ? 0 : 1;

  if ( get_thing ( Socket, Data, "PRINT IO 25", reply, local_debug ) < 0 )
    return -1;
  Data->negative = atoi ( reply );

  if ( get_thing ( Socket, Data, "PRINT IO 26", reply, local_debug ) < 0 )
    return -1;
  Data->positive = atoi ( reply );

  /* This gets a bunch of variables, the first one of which is
   * HOME, the rest must be flushed
   */
  if ( get_thing ( Socket, Data, "PRINT UVARS", reply, local_debug ) < 0 )
    return -1;
  Data->home = strstr(reply, "1.0") == NULL ? 0 : 1;
  Socket_Flush ( Socket->sockfd );

  if ( debug ) {
    Stage_PrintDevice( Socket, Data);
  }

  return(0);
}

/* THE END */
