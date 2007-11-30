/*

  OAP definitions

  Written  28aug07  DLM  Started with FSM.h

*/

/*
  The default home positions.  The home position for each science
    instrument can be set in the AO GUI
*/
#define OAP_TIP_HOMEVALUE -155.0
#define OAP_TILT_HOMEVALUE -147.5

/*
  Conversion factors to change from device units to user units
*/
#define OAP_TIP_GAIN   3333.0
#define OAP_TILT_GAIN  3333.0

/*
  Default acceleration and velocity for motion of OAP
*/

#define OAP_SPEED 1000
#define OAP_ACCEL 100

/*
  Fast and slow speeds and acceleration used durning home proceedure
*/
#define OAP_FAST_SPEED 4000
#define OAP_SLOW_ACCEL 10

/*
  Default distance to move the OAP motor to insure the negative limit has been tripped
*/
#define OAP_NEGATIVE_DISTANCE -200
#define OAP_POSITIVE_DISTANCE 200

/*
  Maximum time in seconds to wait for OAP to move to negative limit while homing
*/
#define OAP_NEGATIVE_WAIT 30
