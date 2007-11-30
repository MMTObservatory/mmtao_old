/*===========================================================================*
**
**  MVP_PrintStatus()
**
**  Routines to retrive and print information from an MVP device controller
**
**  Returns :  0 success
**            -1 error
**
**  Written  14aug07  DLM  Started with FSM_PrintStatus.
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "MVP.h"

/*=========================================
 * function: MVP_PrintStatus
 *=========================================*/

int MVP_PrintStatus( Socket_Info *Socket, Device_Data *Data, int debug)
{
  short status;

  /*
    Force the device information to update
  */
  status = MVP_Status( Socket, Data, debug);
  if ( status ) {
    return(-1);
  }

  /*
  Print the status word
  */
  if ( debug ) {
    printf("  MVP_PrintStatus: Status word for %s %04X\n", Socket->name, status);
    fflush(stdout);
  }

  /*
    Decrypt the status word and printout relavent ones
  */
  if (status & MVP_MOVE_IN_PROGRESS)
    printf ("\tMove in progress.\n");
  if (status & MVP_IN_POSITION)
    printf ("\tMotor is in position.\n");
  if (status & MVP_NOT_ENABLED)
    printf ("\tMotor not enabled or disabled by error.\n");
  if (status & MVP_ERROR)
    printf ("\t***An error has occured***\n");
  if (status & MVP_EMERGENCY_STOP)
    printf ("\t***Emergency stop***\n");
  if (status & MVP_POSITIVE_LIMIT)
    printf ("\tMotor is at positive limit.\n");
  if (status & MVP_NEGATIVE_LIMIT)
    printf ("\tMotor is at negative limit.\n");

  /*
    Not so interesting status flags

    if (status & MVP_VELOCITY_MODE)
      printf ("\tMotor is in velocity mode.\n");
    if (status & MVP_TRAJECTORY_COMPLETE)
      printf ("\tTrajectory percentage command complete.\n");
    if (status & MVP_REACHED_RANGE_LIMIT)
      printf ("\t  Motor has reached programmed range limits.\n");
    if (status & MVP_DEVICENET_MODE)
      printf ("\tMotor is in DeviceNet mode.\n");
    if (status & MVP_DEVICENET_ERROR)
      printf ("\tA DeviceNet error has occured.\n");
    if (status & MVP_MOVEMENT_OFF_TRAJECTORY)
      printf ("\tMovement off program trajectory by more than allowed.\n");
    if (status & MVP_LOCAL_MODE)
      printf ("\tMotor is is local mode.\n");
  */

  fflush(stdout);

  return(0);
}
