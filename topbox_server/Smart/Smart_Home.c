/*
  Smart_Home.c

  This routine homes a smart motor axis.
    => Rotation: read the abosolute encoder position and set the position
         in the Rotation Smart Controller
    => Focus: set the current position to zero in the Focus Smart Controller

  Written  19sep07  DLM, tjt  Started with Smart_SetPosition.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Smart.h"

/*=========================================
  Set Home values
  =========================================*/

/* Runs as a thread, just in case it takes a long time to
 * get encoder information (which it shouldn't)
 *
 * The Focus stage is easy, we just call wherever we are
 * zero and that is that.  There are good reasons for this
 * according to Doug, in one direction there is an ugly
 * collision before hitting the travel limit, in the other
 * we get badly "out of balance" and the motors will begin
 * to skip and fail to move.  The thing to do is to come
 * alive, and then an operator can move the stage as needed.
 *
 * The Rotation is even simpler, just read the encoder and
 * set position accordingly.
 */
void *Smart_Home ( void *Passed_Info )
{
    char buf[STRING_LENGTH];
    int *running;
    int *debug;
    int position;
    int rv;

    Device_Info *Info = (Device_Info *)Passed_Info;
    Socket_Info *Socket = Info->socket;
    Device_Data *Data = Info->data;

    /* Both of these are pointers. */
    running = Info->running;
    debug = Info->cmd_debug;

    *running = 1;

    if ( *debug ) {
        printf("==> Smart_Home: Preparing to home %s\n", Socket->name);
        fflush(stdout);
    }

    if ( strcmp ( Data->name, "Focus" ) == 0 ) {
	position = 0;
    } else {	/* Rotation */
    	rv = Smart_ReadEncoder ( &position, *debug );
	if ( rv ) return (void *) -1;
    }

    sprintf ( buf, "O=%d", position );
    rv = Smart_Cmd ( Socket, Data, buf, buf, *debug );
    if ( rv ) return (void *)-1;

    /* set the homed variable */
    rv = Smart_Cmd ( Socket, Data, "h=1", buf, *debug );
    if ( rv ) return (void *)-1;

    return (void *) 0;
}

/* THE END */
