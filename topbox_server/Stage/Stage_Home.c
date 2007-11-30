/*
  Stage_Home.c

  This routine homes the requested device.  This routine will
    send requests to the device, and then depend on the Stage_Looper()
    for this device to update the device data structure.  Once the
    required state is acheieved, this homing proceedure will send
    the next request.

  Modified  24aug07  tjt  Started with MVP_Home.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Stage.h"

/*=========================================
 * Start the homing procedure
 *=========================================*/

void *Stage_Home( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  Device_Info *Info;
  Socket_Info *Socket;
  Device_Data *Data;
  int *running;
  int *debug;

  int status;
  int count, tries, steps;

  /*
    Put the passed variables in to the local variables
  */
  Info = (Device_Info *)Passed_Info;
  Socket = Info->socket;
  Data = Info->data;
  running = Info->running;
  debug = Info->cmd_debug;

  *running = 1;

  if ( *debug ) {
    printf("==> Stage_Home: Preparing to home %s\n", Socket->name);
    fflush(stdout);
  }

  /*
    Set the Home flag to false so if the homing sequence fails the flag
      will be set to the proper value.
  */
  (void) Stage_SetHomeVar ( Socket, Data, 0.0, *debug );

  /*
    When the LYNX stages are power on the default state is enable.  But, we
      may have disabled the stage for some other reason and this will insure
      that the homing procedure can be executed.
  */
  (void) Stage_Enable( Socket, Data, *debug);

  /*
    Set the default speed
  */
  status = Stage_SetSpeed( Socket, Data, Data->speed, *debug);
  if ( status ) {
    printf("  Stage_Home: Error setting speed of %s\n", Socket->name);
    printf("            Terminating Stage_Home thread\n");
    fflush(stdout);
    *running = 0;
    return( (void *)-1);
  }

  /*
    If negative limit is triggered then we are ready to find home
      position (edge of negative limit=photogate)
  */
  if ( !Data->negative ) {

    /*
      If negative limit is not triggered then we must move so the
        negative limit is broken
    */
    if ( *debug ) {
      printf("  Stage_Home: Moving %s to negative limit\n", Socket->name);
      fflush(stdout);
    }

    /* 
       Move in negative direction till negative limit is triggered.  The breaking the
         negative limit automatically stops the motor's motion.  We will then only be
	 allowed to move in the positive direction.
    */
    status = Stage_MoveRelative( Socket, Data, STAGE_NEG1_DISTANCE, *debug);
    if ( status ) {
      printf("  Stage_Home: Error making relative move with %s\n", Socket->name);
      printf("            Terminating Stage_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

  }

  /*
    Call Stage_Status to make sure the status structure has been updated
  */
  status = Stage_Status(Socket, Data, 0);

  /*
    Wait till negative limit has been triggered before finding home.

    Use count to limit the number of tries to check if negative limit
      has been triggered.  This will act as a timeout in case motor is
      not moving, or an error occurs.

    We do not have to call Stage_Status() here because the Stage_Looper() for
      this device is updating the Data structure.
  */
  tries = 1000000/STAGE_LOOPER_WAIT * STAGE_NEGATIVE_WAIT;
  count = 0;
  do {

    if ( Data->error || !Data->enabled ) {
      printf("  Stage_Home: Move of %s to negative limit not completed.  Device Error\n",
	     Socket->name);
      printf("            Terminating Stage_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    } else if ( count >= tries) {
      printf("  Stage_Home: Move of %s to negative limit not completed.  Timeout\n",
	     Socket->name);
      printf("            Terminating Stage_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Set the check rate to the same at the the update rate of the Stage_Looper() for this device
    */
    usleep(STAGE_LOOPER_WAIT);

    count++;

  } while ( !(Data->negative) );

  /*
    Perform the homing sequence
  */
  if ( *debug ) {
    printf("  Stage_Home: %s is now at its negaive limit.\n", Socket->name);
    fflush(stdout);
  }

  /*
    Move the motor in steps of 5 mm till the negative limit is no longer triggered.
  */
  steps = 0;
  do {

    if ( *debug ) {
      printf("  Stage_Home: Move %s slowly off negative limit\n", Socket->name);
      fflush(stdout);
    }

    /*
      Move 5 mm
    */
    status = Stage_MoveRelative( Socket, Data, STAGE_JOG_DISTANCE, *debug);
    if ( status ) {
      printf("  Stage_Home: Error making relative move with %s\n", Socket->name);
      printf("            Terminating Stage_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Call Stage_Status to make sure the status structure has been updated
    */
    status = Stage_Status( Socket, Data, 0);

    /*
      Wait till motor is no longer moving

      Use count to limit the number of tries to check if the motor is moving

      We do not have to call Stage_Status() here because the Stage_Looper() for
        this device is updating the Data structure.
    */
    count = 0;
    do {
      
      if ( Data->error || !Data->enabled ) {
	printf("  Stage_Home: Move of %s to negative limit not completed.  Device Error\n",
	       Socket->name);
	printf("            Terminating Stage_Home thread\n");
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      } else if ( count >= tries) {
	printf("  Stage_Home: Move of %s to negative limit not completed.  Timeout\n",
	       Socket->name);
	printf("            Terminating Stage_Home thread\n");
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      }

      /*
	Set the check rate to the same at the the update rate of the Stage_Looper() for this device
      */
      usleep(STAGE_LOOPER_WAIT);
	
      count++;

    } while ( Data->moving );

    /*
      Use steps to limit the number of 10 moves
    */
    steps++;

  } while ( !( !(Data->negative) || !(steps < 10) ) );

  /*
    Move the motor really slow in the negative direction and allow the triggering
      of the negative limit stop the motion.  This will define the home position
  */
  status = Stage_SetSpeed( Socket, Data, Data->speed/10.0, *debug);
  if ( status ) {
    printf("  Stage_Home: Error setting speed of %s\n", Socket->name);
    printf("            Terminating Stage_Home thread\n");
    fflush(stdout);
    *running = 0;
    return( (void *)-1);
  }

  /*
    We have move a maximum of -50.0 in the positive direction or until
      negative limit is encountered.
  */
  if ( *debug ) {
    printf("  Stage_Home: Move %s slowly to negative limit\n", Socket->name);
  }
  status = Stage_MoveRelative( Socket, Data, STAGE_NEG2_DISTANCE, *debug);
  if ( status ) {
    printf("  Stage_Home: Error making relative move with %s\n", Socket->name);
    printf("            Terminating Stage_Home thread\n");
    fflush(stdout);
    *running = 0;
    return( (void *)-1);
  }

  /*
    Call Stage_Status to make sure the status structure has been updated
  */
  status = Stage_Status( Socket, Data, 0);

  /*
    Wait till motor is long at a negative limit

    Use count to limit the number of tries to check if the motor is of the its
      negative limit
	
    We do not have to call Stage_Status() here because the Stage_Looper() for
      this device is updating the Data structure.
  */
  count = 0;
  do {
    
    if ( Data->error || !Data->enabled ) {
      printf("  Stage_Home: Move of %s slowly to negative limit not completed.  Device Error\n",
	     Socket->name);
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    } else if ( count >= tries) {
      printf("  Stage_Home: Move of %s slowly to negative limit not completed.  Timeout\n",
	     Socket->name);
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Set the check rate to the same at the the update rate of the Stage_Looper() for this device
    */
    usleep(STAGE_LOOPER_WAIT);

    count++;

  } while ( !Data->negative );

  /*
    Set this position as a temporary 0.0
  */
  status = Stage_SetZero( Socket, Data, *debug);
  if ( status ) {
    *running = 0;
    return( (void *)-1);
  }

  if ( *debug ) {
    printf("  Stage_Home: Negative home position of %s set\n", Socket->name);
    fflush(stdout);
  }

  /*
    Set the device velocity to the default value
  */
  status = Stage_SetSpeed( Socket, Data, Data->speed, *debug);
  if ( status ) {
    *running = 0;
    return( (void *)-1);
  }

  /*
    Move the device to its home postion
  */
  if ( *debug ) {
    printf("  Stage_Home: Move %s to its home position\n", Socket->name);
  }

  status = Stage_MoveRelative( Socket, Data, STAGE_HOMEVALUE, *debug);

  if ( status ) {
    printf("  Stage_Home: Error making relative move with %s\n", Socket->name);
    printf("            Terminating Stage_Home thread\n");
    fflush(stdout);
    *running = 0;
    return( (void *)-1);
  }

  /*
    Call Stage_Status to make sure the status structure has been updated
  */
  status = Stage_Status( Socket, Data, 0);

  /*
    Wait till motor is no longer moving

    Use count to limit the number of tries to check if the motor is moving

    We do not have to call Stage_Status() here because the Stage_Looper() for
      this device is updating the Data structure.
  */
  count = 0;
  do {

    if ( Data->error || !Data->enabled ) {
      printf("  Stage_Home: Move of %s to negative limit not completed.  Device Error\n",
	     Socket->name);
      printf("            Terminating Stage_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    } else if ( count >= tries) {
      printf("  Stage_Home: Move of %s to negative limit not completed.  Timeout\n",
	     Socket->name);
      printf("            Terminating Stage_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Set the check rate to the same at the the update rate of the Stage_Looper() for this device
    */
    usleep(STAGE_LOOPER_WAIT);

    count++;

  } while ( Data->moving );

  /*
    Set the Home flag to true.
  */
  (void) Stage_SetHomeVar ( Socket, Data, 1.0, *debug );

  /*
    The default state of the Indigo and F15 stages is in the disabled state.
      This is to make sure the motors are not accidentally moved into the
      light path while in closed loop.  The user must manually enable the
      stage, make a move and then disable the motor again
  */
  (void) Stage_Disable( Socket, Data, *debug);

  /*
    Inform the user the homing thead is done
  */
  printf("               Homing of %s complete\n", Socket->name);
  fflush(stdout);

  *running = 0;
  return( (void *)0);

}

/* THE END */
