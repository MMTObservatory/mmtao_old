/*
  Generic Stage definitions

  Modified 24aug07  tjt  Started with MVP.h
*/

/*
  Delay time for Stage response
*/
#define STAGE_DELAY_S 0
#define STAGE_DELAY_U 200000

/*
  Delay time to acquire reply after a request has been sent to the Stage device
*/
#define STAGE_REPLY_WAIT 200000

/*
  Delay time to acquire status looper for Stage device
*/
#define STAGE_LOOPER_WAIT 10000

/*
  Parameters used in the homing routine
*/

#define STAGE_NEG1_DISTANCE	-300.0
#define STAGE_NEG2_DISTANCE	-50.0
#define STAGE_JOG_DISTANCE	5.0
#define STAGE_HOMEVALUE		2.0

/* These are sent in float format, but are stored in the "Data" structure
     as integers because the MVP routines require integer.  Either the MVP
     routines could have been changed, or a conversion done in the Stagge
     routines.  The later was easier and faster, and will not have an effect
     as long at velocities are integer value (as they are here)

     27aug07  DLM
*/
#define INDIGO_SPEED	        1

#define F15_SPEED	        8

#define STAGE_NEGATIVE_WAIT	10


/* Stuff specific to the topbox stages:
 */

/*
 *   Indigo parameters.  Stage units are inches so the
 *       gain is the conversion from inches to mm
 *           (1.0/25.4)
 *           */
#define INDIGO_GAIN   0.03937

/*
 *   F15 parameters  Stage units are mm so the
 *       gain is 1.0
 *       */
#define F15_GAIN   1.0

/*
  Stage routines
*/
int Stage_Disable( Socket_Info *Socket, Device_Data *Data, int debug);
int Stage_Enable( Socket_Info *Socket, Device_Data *Data, int debug);
void *Stage_Home( void *);
void *Stage_Looper( void *);
int Stage_MoveAbsolute( Socket_Info *, Device_Data *, double , int );
int Stage_MoveRelative( Socket_Info *, Device_Data *, double , int );
int Stage_Position( Socket_Info *, Device_Data *, int );
int Stage_Process( Device_Info *, char *);
int Stage_PrintDevice( Socket_Info *, Device_Data *);
int Stage_PrintStatus( Socket_Info *, Device_Data *, int );
int Stage_SetHomeVar( Socket_Info *Socket, Device_Data *Data, double data, int debug);
int Stage_SetSpeed( Socket_Info *, Device_Data *, double , int );
int Stage_SetZero( Socket_Info *, Device_Data *, int );
int Stage_Status( Socket_Info *, Device_Data *, int );
int Stage_Read( Socket_Info *, char *, int );
int Stage_Write( Socket_Info *, char *, int );

/* THE END */
