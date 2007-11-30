/*
  Fwheel_Home.c

  This routine homes the filter wheel.

  Modified  24aug07  tjt  Started with Stage_Home.c
  		and wish/FWheel/FWheel_Home.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Fwheel.h"

static int
Fwheel_CheckHome ( Socket_Info *Socket, int debug )
{
  char jive[FW_MAXLINE];

  Fwheel_Send ( Socket, FW_HOMEASK, debug );
  usleep   ( FW_CHECK_DELAY );

  Fwheel_Read ( Socket, jive, debug );

  return atoi ( jive ) & FW_HOMEBIT;
}

int
Fwheel_SeekHome ( Socket_Info *Socket, Device_Data *Data, int limit, int debug )
{
    int at_home;
    int maxTries;

    // turn the homing LED on //
    Fwheel_Send ( Socket, FW_HOMEON, debug );
  
    Data->moving = 1;

    maxTries = limit;

    do {
      // move the wheel by one step //
      (void) Fwheel_Step ( Socket, debug );

      // check if now at home //
      at_home = Fwheel_CheckHome ( Socket, debug );
      usleep   ( FW_HOME_DELAY );

      // repeat until at home or timeout //
    } while ( ! at_home && --maxTries );

    Data->moving = 0;

    // turn the homing LED off //
    Fwheel_Send ( Socket, FW_HOMEOFF, debug );

    return at_home;
}

/*=========================================
 * The homing procedure
 *  called as a thread, so only can get void * argument
 *=========================================*/

void *
Fwheel_Home ( void *Passed_Info )
{
  Device_Info *Info;
  Socket_Info *Socket;
  Device_Data *Data;

  int *running;
  int *debug;

    int at_home;

    Info = (Device_Info *)Passed_Info;
    Socket = Info->socket;
    Data = Info->data;

    /* Both of these are pointers. */
    running = Info->running;
    debug = Info->cmd_debug;

    *running = 1;

    if ( *debug ) {
      printf("==> Stage_Home: Preparing to home %s\n", Socket->name);
      fflush(stdout);
    }

    Status_ZeroFlags ( Data );

    /* Get rid of leakage and pollution. */
    Socket_Flush ( Socket->sockfd );

    at_home = Fwheel_SeekHome ( Socket, Data, FW_MAX_HOME, *debug );

    // We timed out.... //
    if ( ! at_home ) {
      Fwheel_FilterSetPos ( Data, 0 );
      fprintf (stderr, "Fwheel_Home ... Failed\n");
      return( (void *)-1);
    }

    Data->home = 1;

    // The first filter wheel is not at the home position,
    // so offset to center the first filter.
    (void) Fwheel_MoveCount ( Socket, Data, FW_OFFSET, *debug );

    /* We use filter positions 1-N */
    Fwheel_FilterSetPos ( Data, 1 );

    printf("               Homing of %s complete\n", Socket->name);
    fflush(stdout);

    *running = 0;
    return( (void *)0);
}

/* THE END */
