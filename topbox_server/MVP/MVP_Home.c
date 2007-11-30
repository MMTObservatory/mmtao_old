/*
  MVP_Home.c

  This routine homes the requested device.  This routine will
    send requests to the device, and then depend on the MVP_Looper()
    for this device to update the device data structure.  Once the
    required state is acheieved, this homing proceedure will send
    the next request.

  Written  23aug07  DLM  Started with FSM_Home.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"
#include "FSM.h"
#include "OAP.h"

/*=========================================
 * Start the homing procedure
 *=========================================*/

void *MVP_Home( void *Passed_Info)
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
    printf("==> MVP_Home: Preparing to home %s\n", Socket->name);
    fflush(stdout);
  }

  /*
    Set the home flag to FALSE so if the homing procedure fails, the home
      flag has the correct value
  */
  status = MVP_SetHomeFlag( Socket, Data, 0, *debug);
  if ( status ) {
    printf("  MVP_Home: Error setting home flag to FALSE for %s\n", Socket->name);
    printf("            Terminating MVP_Home thread\n");
    fflush(stdout);
    *running = 0;
    return( (void *)-1);
  }

  /*
    Request the device to move a relative distance 0.0.  This reset
    all the move parameter in the MVP controller so it reads that
    its in position, so the device can be enabled.
  */
  status = MVP_MoveRelative( Socket, Data, 0.0, *debug);
  if ( status ) {
    printf("  MVP_Home: Error setting relative move to 0.0 for %s\n", Socket->name);
    printf("            Terminating MVP_Home thread\n");
    fflush(stdout);
    *running = 0;
    return( (void *)-1);
  }

  /*
    When the MVP is powered on the default state Disabled.  Thus, we must
      enable the device to perform the homing sequence.
  */
  status = MVP_Enable( Socket, Data, *debug);
  if ( status ) {
    printf("  MVP_Home: Error enabling %s\n", Socket->name);
    printf("            Terminating MVP_Home thread\n");
    fflush(stdout);
    *running = 0;
    return( (void *)-1);
  }

  /*==================================================
   * Home the FSM motor
   *==================================================*/

  if ( strstr( Socket->name, "FSM" ) != NULL ) {

    /*
      Set the default acceleration
    */
    status = MVP_SetAccel( Socket, Data, FSM_ACCEL, *debug);
    if ( status ) {
      printf("  MVP_Home: Error setting acceleration of %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
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
	printf("  MVP_Home: Moving %s to negative limit\n", Socket->name);
	fflush(stdout);
      }

      /*
	This motion can be done quickly.
      */
      status = MVP_SetSpeed( Socket, Data, FSM_FAST_SPEED, *debug);
      if ( status ) {
	printf("  MVP_Home: Error setting speed of %s\n", Socket->name);
	printf("            Terminating MVP_Home thread\n");
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      }

      /* 
	 Move in negative direction till negative limit is triggered.  The breaking the
           negative limit automatically stops the motor's motion.  We will then only be
	   allowed to move in the positive direction.

	 Total range from negative to positive limit is about 300 arcsec so will move
           -250 arcsec to guarentee we find the negative limit.  We now send Motor_MoveRelative
	   a distance in arcsec
      */
      status = MVP_MoveRelative( Socket, Data, FSM_NEGATIVE_DISTANCE, *debug);
      if ( status ) {
	printf("  MVP_Home: Error making relative move with %s\n", Socket->name);
	printf("            Terminating MVP_Home thread\n");
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      }

    }

    /*
      Call MVP_Status to make sure the status structure has been updated
    */
    status = MVP_Status( Socket, Data, 0);


    /*
      Wait till negative limit has been triggered before finding home.

      Use count to limit the number of tries to check if negative limit
        has been triggered.  This will act as a timeout in case motor is
	not moving, or an error occurs.

      We do not have to call MVP_Status() here because the MVP_Looper() for
        this device is updating the Data structure.
    */
    tries = 1000000/MVP_LOOPER_WAIT * FSM_NEGATIVE_WAIT;
    count = 0;
    do {

      if ( Data->error || !Data->enabled ) {
	printf("  MVP_Home: Move of %s to negative limit not completed.  Device Error\n",
	       Socket->name);
	printf("            Terminating MVP_Home thread\n");
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      } else if ( count >= tries) {
	printf("  MVP_Home: Move of %s to negative limit not completed.  Timeout\n",
	       Socket->name);
	printf("            Terminating MVP_Home thread\n");
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      }

      /*
	Set the check rate to the same at the the update rate of the MVP_Looper() for this device
      */
      usleep(MVP_LOOPER_WAIT);

      count++;

    } while ( !(Data->negative) );

    /*
      Perform the homing sequence
    */
    if ( *debug ) {
      printf("  MVP_Home: %s is now at its negaive limit.\n", Socket->name);
      fflush(stdout);
    }

    /*
      Set the motor speed to the default values
    */
    status = MVP_SetSpeed( Socket, Data, FSM_FAST_SPEED, *debug);
    if ( status ) {
      printf("  MVP_Home: Error setting speed of %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Move the motor in steps of 5 arcsec till the negative limit is no longer triggered.
    */
    steps = 0;
    do {

      if ( *debug ) {
	printf("  MVP_Home: Move %s slowly off negative limit\n", Socket->name);
	fflush(stdout);
      }

      /*
	Move FSM 5 arcsec
      */
      status = MVP_MoveRelative( Socket, Data, 5.0, *debug);
      if ( status ) {
	printf("  MVP_Home: Error making relative move with %s\n", Socket->name);
	printf("            Terminating MVP_Home thread\n");
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      }

      /*
	Call MVP_Status to make sure the status structure has been updated
      */
      status = MVP_Status( Socket, Data, 0);

      /*
	Wait till motor is no longer moving

	Use count to limit the number of tries to check if the motor is moving

	We do not have to call MVP_Status() here because the MVP_Looper() for
	  this device is updating the Data structure.
      */
      count = 0;
      do {

	if ( Data->error || !Data->enabled ) {
	  printf("  MVP_Home: Move of %s to negative limit not completed.  Device Error\n",
		 Socket->name);
	  printf("            Terminating MVP_Home thread\n");
	  fflush(stdout);
	  *running = 0;
	  return( (void *)-1);
	} else if ( count >= tries) {
	  printf("  MVP_Home: Move of %s to negative limit not completed.  Timeout\n",
		 Socket->name);
	  printf("            Terminating MVP_Home thread\n");
	  fflush(stdout);
	  *running = 0;
	  return( (void *)-1);
	}

	/*
	  Set the check rate to the same at the the update rate of the MVP_Looper() for this device
	*/
	usleep(MVP_LOOPER_WAIT);

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
    status = MVP_SetSpeed( Socket, Data, FSM_SLOW_SPEED, *debug);
    if ( status ) {
      printf("  MVP_Home: Error setting speed of %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }
    status = MVP_SetAccel( Socket, Data, FSM_SLOW_ACCEL, *debug);
    if ( status ) {
      printf("  MVP_Home: Error setting acceleration of %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      We have move a maximum of -50.0 in the positive direction or until
        negative limit is encountered.
    */
    if ( *debug ) {
      printf("  MVP_Home: Move %s slowly to negative limit\n", Socket->name);
    }
    status = MVP_MoveRelative( Socket, Data, -50.0, *debug);
    if ( status ) {
      printf("  MVP_Home: Error making relative move with %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Call MVP_Status to make sure the status structure has been updated
    */
    status = MVP_Status( Socket, Data, 0);

    /*
      Wait till motor is long at a negative limit

      Use count to limit the number of tries to check if the motor is of the its
        negative limit

      We do not have to call MVP_Status() here because the MVP_Looper() for
        this device is updating the Data structure.
    */
    count = 0;
    do {

      if ( Data->error || !Data->enabled ) {
	printf("  MVP_Home: Move of %s slowly to negative limit not completed.  Device Error\n",
	       Socket->name);
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      } else if ( count >= tries) {
	printf("  MVP_Home: Move of %s slowly to negative limit not completed.  Timeout\n",
	       Socket->name);
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      }

      /*
	Set the check rate to the same at the the update rate of the MVP_Looper() for this device
      */
      usleep(MVP_LOOPER_WAIT);

      count++;

    } while ( !Data->negative );

    /*
      Set this position as a temporary 0.0
    */
    status = MVP_SetZero( Socket, Data, *debug);
    if ( status ) {
      *running = 0;
      return( (void *)-1);
    }

    if ( *debug ) {
      printf("  MVP_Home: Negative home position of %s set\n", Socket->name);
      fflush(stdout);
    }

    /*
      Set the device velocity and acceration to the default values
    */
    status = MVP_SetSpeed( Socket, Data, FSM_SPEED, *debug);
    if ( status ) {
      *running = 0;
      return( (void *)-1);
    }
    status = MVP_SetAccel( Socket, Data, FSM_ACCEL, *debug);
    if ( status ) {
      *running = 0;
      return( (void *)-1);
    }

    /*
      Move the device to its home postion
    */
    if ( *debug ) {
      printf("  MVP_Home: Move %s to its home position\n", Socket->name);
    }
    if ( strstr( Socket->name, "Tip" ) != NULL ) {
      status = MVP_MoveRelative( Socket, Data, FSM_TIP_HOMEVALUE, *debug);
    } else if ( strstr( Socket->name, "Tilt" ) != NULL ) {
      status = MVP_MoveRelative( Socket, Data, FSM_TILT_HOMEVALUE, *debug);
    } else {
      printf("  MVP_Home: Unknown FSM axis %s\n", Socket->name);
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }
    if ( status ) {
      printf("  MVP_Home: Error making relative move with %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Call MVP_Status to make sure the status structure has been updated
    */
    status = MVP_Status( Socket, Data, 0);

    /*
      Wait till motor is no longer moving

      Use count to limit the number of tries to check if the motor is moving

      We do not have to call MVP_Status() here because the MVP_Looper() for
        this device is updating the Data structure.
    */
    count = 0;
    do {

      if ( Data->error || !Data->enabled ) {
	printf("  MVP_Home: Move of %s to home position not completed.  Device Error\n",
	       Socket->name);
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      } else if ( count >= tries) {
	printf("  MVP_Home: Move of %s to home position not completed.  Timeout\n",
	       Socket->name);
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      }

      /*
	Set the check rate to the same at the the update rate of the MVP_Looper() for this device
      */
      usleep(MVP_LOOPER_WAIT);

      count++;

    } while ( Data->moving );

    /*
      Set this position as the "real" home (0.0)
    */
    status = MVP_SetZero( Socket, Data, *debug);
    if ( status ) {
      *running = 0;
      return( (void *)-1);
    }

    /*
      Set the home flag to TRUE
    */
    status = MVP_SetHomeFlag( Socket, Data, 1, *debug);
    if ( status ) {
      printf("  MVP_Home: Error setting home flag to TRUE for %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    if ( *debug ) {
      printf("==> MVP_Home: Homing of %s complete\n", Socket->name);
      fflush(stdout);
    }

    /*
      Inform the user the homing thead is done
    */
    printf("               Homing of %s complete\n", Socket->name);
    fflush(stdout);

  } else if ( strstr( Socket->name, "OAP" ) != NULL ) {

    /*
      Set the default acceleration
    */
    status = MVP_SetAccel( Socket, Data, OAP_ACCEL, *debug);
    if ( status ) {
      printf("  MVP_Home: Error setting acceleration of %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      If event_2 flag (internal home flag) is triggered then we must
        move in the negative direction until the flag is not triggered.
      To do this we send the stage in the negative direction, then poll
        the status flag until the event_2 flag.  When this occurs, this
	routine sends an abort command.
    */
    if ( Data->event_2 ) {

      /*
	Move in the negative direction
      */
      if ( *debug ) {
	printf("  MVP_Home: Moving %s in negative direction\n", Socket->name);
	fflush(stdout);
      }

      /*
	This motion can be done quickly.
      */
      status = MVP_SetSpeed( Socket, Data, OAP_FAST_SPEED, *debug);
      if ( status ) {
	printf("  MVP_Home: Error setting speed of %s\n", Socket->name);
	printf("            Terminating MVP_Home thread\n");
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      }

      /* 
	 Move in negative direction till negative limit is triggered.  The breaking the
           negative limit automatically stops the motor's motion.  We will then only be
	   allowed to move in the positive direction.

	 Total range from negative to positive limit is about 300 arcsec so will move
           -250 arcsec to guarentee we find the negative limit.  We now send Motor_MoveRelative
	   a distance in arcsec
      */
      status = MVP_MoveRelative( Socket, Data, OAP_NEGATIVE_DISTANCE, *debug);
      if ( status ) {
	printf("  MVP_Home: Error making relative move with %s\n", Socket->name);
	printf("            Terminating MVP_Home thread\n");
	fflush(stdout);
	status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
	*running = 0;
	return( (void *)-1);
      }

    }

    /*
      Call MVP_Status to make sure the status structure has been updated
    */
    status = MVP_Status( Socket, Data, 0);

    /*
      Wait till event_2 flag is no longer triggered

      Use count to limit the number of tries to check if negative limit
        has been triggered.  This will act as a timeout in case motor is
	not moving, or an error occurs.

      We do not have to call MVP_Status() here because the MVP_Looper() for
        this device is updating the Data structure.
    */
    tries = 1000000/MVP_LOOPER_WAIT * OAP_NEGATIVE_WAIT;
    count = 0;
    do {

      if ( Data->error || !Data->enabled ) {
	printf("  MVP_Home: Move of %s in negative direction not completed.  Device Error\n",
	       Socket->name);
	printf("            Terminating MVP_Home thread\n");
	fflush(stdout);
	status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
	*running = 0;
	return( (void *)-1);
      } else if ( count >= tries) {
	printf("  MVP_Home: Move of %s in negative direction not completed.  Timeout\n",
	       Socket->name);
	printf("            Terminating MVP_Home thread\n");
	fflush(stdout);
	status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
	*running = 0;
	return( (void *)-1);
      }

      /*
	Set the check rate to the same at the the update rate of the MVP_Looper() for this device
      */
      usleep(MVP_LOOPER_WAIT);

      count++;

    } while ( Data->event_2 );

    /*
      The internal home flag is no longer tripped so send the OAP an abort command to
        stop the motion
    */
    status = MVP_Abort( Socket, Data, *debug);
    if ( status ) {
      printf("  MVP_Home: Error sending abort command to %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Perform the homing sequence
    */
    if ( *debug ) {
      printf("  MVP_Home: %s is past its homing flag.\n", Socket->name);
      fflush(stdout);
    }

    /*
      Set the motor speed to a slow value for the homing
    */
    status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
    if ( status ) {
      printf("  MVP_Home: Error setting speed of %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Arm the homing trigger
    */
    status = MVP_SetHomeTrigger( Socket, Data, 1, *debug);
    if ( status ) {
      printf("  MVP_Home: Error setting home trigger on %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Set the ampifier to execute a "hard stop".
    */
    status = MVP_HardStop( Socket, Data, *debug);
    if ( status ) {
      printf("  MVP_Home: Error setting hard stop on %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Move OAP in the positive direction until the home flag (event_2) has
        been triggered
    */
    status = MVP_MoveRelative( Socket, Data, OAP_POSITIVE_DISTANCE, *debug);
    if ( status ) {
      printf("  MVP_Home: Error making relative move with %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Call MVP_Status to make sure the status structure has been updated
    */
    status = MVP_Status( Socket, Data, 0);

    /*
      Wait till motor is no longer moving

      Use count to limit the number of tries to check if the motor is moving

      We do not have to call MVP_Status() here because the MVP_Looper() for
        this device is updating the Data structure.
    */
    count = 0;
    do {

      if ( Data->error || !Data->enabled ) {
	printf("  MVP_Home: Move of %s to negative limit not completed.  Device Error\n",
	       Socket->name);
	printf("            Terminating MVP_Home thread\n");
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      } else if ( count >= tries) {
	printf("  MVP_Home: Move of %s to negative limit not completed.  Timeout\n",
	       Socket->name);
	printf("            Terminating MVP_Home thread\n");
	fflush(stdout);
	*running = 0;
	return( (void *)-1);
      }

      /*
	Set the check rate to the same at the the update rate of the MVP_Looper() for this device
      */
      usleep(MVP_LOOPER_WAIT);

      count++;

    } while ( !Data->event_2 );

    /*
      disarm the homing trigger
    */
    status = MVP_SetHomeTrigger( Socket, Data, 0, *debug);
    if ( status ) {
      printf("  MVP_Home: Error unsetting home trigger on %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Set this position as a temporary 0.0
    */
    status = MVP_SetZero( Socket, Data, *debug);
    if ( status ) {
      status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
      *running = 0;
      return( (void *)-1);
    }

    /*
      The home trigger (event_2) has been found and the motor has stopped.  Now
        set the speed to the nominal value.
    */
    status = MVP_SetSpeed( Socket, Data, OAP_FAST_SPEED, *debug);
    if ( status ) {
      printf("  MVP_Home: Error setting speed of %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Move the device to its home postion
    */
    if ( *debug ) {
      printf("  MVP_Home: Move %s to its home position\n", Socket->name);
    }
    if ( strstr( Socket->name, "Tip" ) != NULL ) {
      status = MVP_MoveRelative( Socket, Data, OAP_TIP_HOMEVALUE, *debug);
    } else if ( strstr( Socket->name, "Tilt" ) != NULL ) {
      status = MVP_MoveRelative( Socket, Data, OAP_TILT_HOMEVALUE, *debug);
    } else {
      printf("  MVP_Home: Unknown OAP axis %s\n", Socket->name);
      fflush(stdout);
      status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
      *running = 0;
      return( (void *)-1);
    }
    if ( status ) {
      printf("  MVP_Home: Error making relative move with %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Call MVP_Status to make sure the status structure has been updated
    */
    status = MVP_Status( Socket, Data, 0);

    /*
      Wait till motor is no longer moving

      Use count to limit the number of tries to check if the motor is moving

      We do not have to call MVP_Status() here because the MVP_Looper() for
        this device is updating the Data structure.
    */
    count = 0;
    do {

      if ( Data->error || !Data->enabled ) {
	printf("  MVP_Home: Move of %s to home position not completed.  Device Error\n",
	       Socket->name);
	fflush(stdout);
	status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
	*running = 0;
	return( (void *)-1);
      } else if ( count >= tries) {
	printf("  MVP_Home: Move of %s to home position not completed.  Timeout\n",
	       Socket->name);
	fflush(stdout);
	status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
	*running = 0;
	return( (void *)-1);
      }

      /*
	Set the check rate to the same at the the update rate of the MVP_Looper() for this device
      */
      usleep(MVP_LOOPER_WAIT);

      count++;

    } while ( Data->moving );

    /*
      Set this position as the "real" home (0.0)
    */
    status = MVP_SetZero( Socket, Data, *debug);
    if ( status ) {
      status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
      *running = 0;
      return( (void *)-1);
    }

    /*
      Set the home flag to TRUE
    */
    status = MVP_SetHomeFlag( Socket, Data, 1, *debug);
    if ( status ) {
      printf("  MVP_Home: Error setting home flag to TRUE for %s\n", Socket->name);
      printf("            Terminating MVP_Home thread\n");
      fflush(stdout);
      status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
      *running = 0;
      return( (void *)-1);
    }

    if ( *debug ) {
      printf("==> MVP_Home: Homing of %s complete\n", Socket->name);
      fflush(stdout);
    }

    /*
      Inform the user the homing thead is done
    */
    printf("               Homing of %s complete\n", Socket->name);
    fflush(stdout);

  } else {

    printf("  MVP_Home: Unknown device %s\n", Socket->name);
    fflush(stdout);
    *running = 0;
    return( (void *)-1);

  }

  /*
    Set the speed to the nominal value
  */
  status = MVP_SetSpeed( Socket, Data, Data->speed, *debug);
  if ( status ) {
    printf("  MVP_Home: Error setting speed of %s\n", Socket->name);
    printf("            Terminating MVP_Home thread\n");
    fflush(stdout);
    *running = 0;
    return( (void *)-1);
  }

  *running = 0;
  return( (void *)0);
}
