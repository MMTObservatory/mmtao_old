/*
  Source_RelativeMVP.c

  This thread slowly simulates a relative device motion

  Written  21aug07  DLM  Started with Source_MVP.c

*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Source.h"

/*================================================================================*
 * Thread to make a relative move of a device
 *================================================================================*/
void *Source_RelativeMVP( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  Source_Info *Info;
  Socket_Info *Socket;
  Device_Data *Data;
  int *home_trigger;
  int *remaining;
  int *relative;
  int *absolute;
  int *running;

  int local_debug = 1;
  int delta;

  /*
    Put the passed variables in to the local variables
  */
  Info = (Source_Info *)Passed_Info;
  Socket = Info->socket;
  Data = Info->data;
  home_trigger = Info->home_trigger;
  remaining = Info->remaining;
  relative = Info->relative;
  absolute = Info->absolute;
  running = Info->running;

  *running = 1;
  Data->moving = 1;
  Data->in_position = 0;

  /*
    Check that the device is enabled
  */
  if ( !Data->enabled ) {
    printf("  Source_RelativeMVP: %s is not enabled\n", Socket->name);
    printf("                      Motion aborted\n");
    fflush(stdout);
    *running = 0;
    Data->moving = 0;
    Data->in_position = 1;
    return( (void *)0);
  }

  /*
    The MVP devices (FSM and OAP motors) have a range of about 3000 arcsec
      from the positive to negative limit.  Thus, limit the range of requested
      moves
  */
  if ( fabs((double)*relative / Data->gain) > 300.0 ) {
    printf("  Source_RelativeMVP: Relative move request of %8.2f arcsec is too large\n",
	   (double)*relative / Data->gain);
    printf("                      Maximum move = 300.00 arcsec\n");
    *relative = 0;
    *absolute = 0;
    *running = 0;
    Data->moving = 0;
    Data->in_position = 1;
    return( (void *)0);
  }

  /*
    Check the limit switch in the opposite direction that we are requesting a move
      and set the size and direction of the motion
  */
  if ( *relative < 0 ) {
    if ( Data->negative ) {
      printf("  Source_RelativeMVP: Motion stopped by Negative limit\n");
      printf("                      Can only move in the positive direction\n");
      printf("                      Motion aborted\n");
      fflush(stdout);
      *relative = 0;
      *absolute = 0;
      *running = 0;
      Data->moving = 0;
      Data->in_position = 1;
      return( (void *)0);
    }
    delta = -100;
  } else {
    if ( Data->positive ) {
      printf("  Source_RelativeMVP: Motion stopped by Positive limit\n");
      printf("                      Can only move in the netative direction\n");
      printf("                      Motion aborted\n");
      fflush(stdout);
      *relative = 0;
      *absolute = 0;
      *running = 0;
      Data->moving = 0;
      Data->in_position = 1;
      return( (void *)0);
    }
    delta = 100;
  }

  if ( local_debug ) {
    printf("  Source_RelativeMVP: Position = %8.2f", Data->position);
    fflush(stdout);
  }
  for ( *remaining = *relative; abs(*remaining) > abs(delta); *remaining -= delta ) {
    Data->position += (double)delta / Data->gain;
    if ( local_debug ) {
      printf("\b\b\b\b\b\b\b\b%8.2f", Data->position);
      fflush(stdout);
    }
    if ( Data->position > 170.00 ) {
      Data->positive = 1;
      if ( delta > 0 ) {
	if ( local_debug ) {
	  printf("\n  Source_RelativeMVP: Motion stopped by Positive limit\n");
	  fflush(stdout);
	}
	*remaining = 0;
	*relative = 0;
	*absolute = 0;
	*running = 0;
	Data->moving = 0;
	Data->in_position = 1;
	return( (void *)0);
      }
    } else {
      Data->positive = 0;
    }
    if ( Data->position < -170.00 ) {
      Data->negative = 1;
      if ( delta < 0 ) {
	if ( local_debug ) {
	  printf("\n  Source_RelativeMVP: Motion stopped by Negative limit\n");
	  fflush(stdout);
	}
	*remaining = 0;
	*relative = 0;
	*absolute = 0;
	*running = 0;
	Data->moving = 0;
	Data->in_position = 1;
	return( (void *)0);
      }
    } else {
      Data->negative = 0;
    }

    /*
      For the OAP motors, there is a home trigger flag (event_2) that is set about mid-range.  Thus,
        we will set this home trigger flag at -100.0.  This in needed for the OAP homing
	procedure to work
    */
    if ( Data->position < -100.00 ) {
      Data->event_2 = 0;
    } else {
      Data->event_2 = 1;
      if ( *home_trigger == 1 ) {
	if ( local_debug ) {
	  printf("\n  Source_RelativeMVP: Motion stopped by Home Trigger\n");
	  fflush(stdout);
	}
	*remaining = 0;
	*relative = 0;
	*absolute = 0;
	*running = 0;
	Data->moving = 0;
	Data->in_position = 1;
	return( (void *)0);
      }
    }

    usleep(1000);
  }
  Data->position += (double)*remaining / Data->gain;
  if ( local_debug ) {
    printf("\b\b\b\b\b\b\b\b%8.2f\n", Data->position);
    printf("  Source_RelativeMVP: Thread done\n");
    fflush(stdout);
  }

  *remaining = 0;
  *relative = 0;
  *absolute = 0;
  *running = 0;
  Data->moving = 0;
  Data->in_position = 1;
  return( (void *)0);
}
