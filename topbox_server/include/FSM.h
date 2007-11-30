/*

  FSM definitions

  Written  15aug07  DLM  Started with FSM.h from wish

*/

/*
  The default home positions.  The home position for each science
    instrument can be set in the AO GUI
*/
#define FSM_TIP_HOMEVALUE 112.5
#define FSM_TILT_HOMEVALUE 93.0

/*
  Conversion factors to change from device units to user units
*/
#define FSM_TIP_GAIN   1800.0
#define FSM_TILT_GAIN  1610.0

/*
  Default acceleration and velocity for motion of FSM
*/

#define FSM_SPEED 1000
#define FSM_ACCEL 100

/*
  Fast and slow speeds and acceleration used durning home proceedure
*/
#define FSM_FAST_SPEED 2000
#define FSM_SLOW_SPEED 100
#define FSM_SLOW_ACCEL 10

/*
  Default distance to move the FSM motor to insure the negative limit has been tripped
*/
#define FSM_NEGATIVE_DISTANCE -250

/*
  Maximum time in seconds to wait for FSM to move to negative limit while homing
*/
#define FSM_NEGATIVE_WAIT 10
