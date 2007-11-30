/*
  Source_StageRelativeMove.c

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
void *Source_StageRelativeMove( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  Source_Info *Info;
  Socket_Info *Socket;
  Device_Data *Data;
  float *distance;
  int *running;

  int local_debug = 1;
  float delta, remaining;

  /*
    Put the passed variables in to the local variables
  */
  Info = (Source_Info *)Passed_Info;
  Socket = Info->socket;
  Data = Info->data;
  distance = Info->distance;
  running = Info->running;

  *running = 1;
  Data->moving = 1;

  /*
    Check that the device is enabled
  */
  if ( !Data->enabled ) {
    printf("  Source_StageRelativeMove: %s is not enabled\n", Socket->name);
    printf("                            Motion aborted\n");
    fflush(stdout);
    *running = 0;
    Data->moving = 0;
    return( (void *)0);
  }

  /*
    The Stage have a range of about 200 mm from the positive to negative limit.  Thus,
      limit the range of requested moves
  */
  if ( *distance > 200.0 ) {
    printf("  Source_StageRelativeMove: Relative move request of %8.2f arcsec is too large\n",
	   *distance);
    printf("                            Maximum move = 200.0 mm\n");
    *distance = 0.0;
    *running = 0;
    Data->moving = 0;
    return( (void *)0);
  }

  /*
    Check the limit switch in the opposite direction that we are requesting a move
      and set the size and direction of the motion
  */
  if ( *distance < 0.0 ) {
    if ( Data->negative ) {
      printf("  Source_StageRelativeMove: Motion stopped by Negative limit\n");
      printf("                            Can only move in the positive direction\n");
      printf("                            Motion aborted\n");
      fflush(stdout);
      *distance = 0.0;
      *running = 0;
      Data->moving = 0;
      return( (void *)0);
    }
    delta = -0.05;
  } else {
    if ( Data->positive ) {
      printf("  Source_StageRelativeMove: Motion stopped by Positive limit\n");
      printf("                            Can only move in the netative direction\n");
      printf("                            Motion aborted\n");
      fflush(stdout);
      *distance = 0.0;
      *running = 0;
      Data->moving = 0;
      return( (void *)0);
    }
    delta = 0.05;
  }

  if ( local_debug ) {
    printf("  Source_StageRelativeMove: Position = %8.2f", Data->position);
    fflush(stdout);
  }
  for ( remaining = *distance; abs(remaining) > abs(delta); remaining -= delta ) {
    Data->position += delta;
    if ( local_debug ) {
      printf("\b\b\b\b\b\b\b\b%8.2f", Data->position);
      fflush(stdout);
    }
    if ( Data->position > 150.00 ) {
      Data->positive = 1;
      if ( delta > 0.0 ) {
	if ( local_debug ) {
	  printf("\n  Source_StageRelativeMove: Motion stopped by Positive limit\n");
	  fflush(stdout);
	}
	*distance = 0.0;
	*running = 0;
	Data->moving = 0;
	return( (void *)0);
      }
    } else {
      Data->positive = 0;
    }
    if ( Data->position < -150.00 ) {
      Data->negative = 1;
      if ( delta < 0.0 ) {
	if ( local_debug ) {
	  printf("\n  Source_StageRelativeMove: Motion stopped by Negative limit\n");
	  fflush(stdout);
	}
	*distance = 0.0;
	*running = 0;
	Data->moving = 0;
	return( (void *)0);
      }
    } else {
      Data->negative = 0;
    }
      
    usleep(1000);
  }
  Data->position += remaining;
  if ( local_debug ) {
    printf("\b\b\b\b\b\b\b\b%8.2f\n", Data->position);
    printf("  Source_StageRelativeMove: Thread done\n");
    fflush(stdout);
  }

  *distance = 0.0;
  *running = 0;
  Data->moving = 0;
  return( (void *)0);
}
