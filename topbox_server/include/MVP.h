/*

  Generic MVP definitions

  Written  15aug07  DLM  Started with FSM.h from wish

*/

/*
  Delay time for MVP response
*/
#define MVP_DELAY_S 0
#define MVP_DELAY_U 200000

/*
  Delay time to acquire reply after a request has been sent to the MVP device
*/
#define MVP_REPLY_WAIT 200000

/*
  Delay time to acquire status looper for MVP device
*/
#define MVP_LOOPER_WAIT 10000

/*
  Baud rate is determined by the settings of the Cyclades terminal server
*/
//#define MVP_BAUDRATE B38400
#define MVP_BAUDRATE B9600

/*
  Maximum number of times to query fsm to see if an event has
    occured.  This will act as a time limit.
*/
#define MVP_MAX_TRIES 1000

/*
  MicroMo Motor controller status word bit flags
*/
#define MVP_MOVE_IN_PROGRESS        0x0001
#define MVP_IN_POSITION             0x0002
#define MVP_VELOCITY_MODE           0x0004
#define MVP_TRAJECTORY_COMPLETE     0x0008
#define MVP_ERROR                   0x0010
#define MVP_DEVICENET_MODE          0x0020
#define MVP_DEVICENET_ERROR         0x0040
#define MVP_MOVEMENT_OFF_TRAJECTORY 0x0080
#define MVP_NOT_ENABLED             0x0100
#define MVP_REACHED_RANGE_LIMIT     0x0200
#define MVP_LOCAL_MODE              0x0400
#define MVP_EMERGENCY_STOP          0x0800
#define MVP_EXTERNAL_EVENT_1        0x1000
#define MVP_POSITIVE_LIMIT          0x2000
#define MVP_EXTERNAL_EVENT_2        0x4000
#define MVP_NEGATIVE_LIMIT          0x8000

/*
  MVP routines
*/
int MVP_Abort( Socket_Info *Socket, Device_Data *Data, int debug);
int MVP_Disable( Socket_Info *Socket, Device_Data *Data, int debug);
int MVP_Enable( Socket_Info *Socket, Device_Data *Data, int debug);
int MVP_HardStop( Socket_Info *Socket, Device_Data *Data, int debug);
void *MVP_Home( void *Passed_Info);
int MVP_HomeFlag( Socket_Info *Socket, Device_Data *Data, int debug);
void *MVP_Looper( void *Passed_Info);
int MVP_MoveAbsolute( Socket_Info *Socket, Device_Data *Data, float distance, int debug);
int MVP_MoveRelative( Socket_Info *Socket, Device_Data *Data, float distance, int debug);
int MVP_Position( Socket_Info *Socket, Device_Data *Info, int debug);
int MVP_Process( Device_Info *Info, char *request);
int MVP_PrintDevice( Socket_Info *Socket, Device_Data *Data);
int MVP_PrintStatus( Socket_Info *Socket, Device_Data *Data, int debug);
int MVP_Read( Socket_Info *Socket, char *reply, int debug);
int MVP_ResetNode( Socket_Info *Socket, Device_Data *Data, int debug);
int MVP_SetAccel( Socket_Info *Socket, Device_Data *Data, int accel, int debug);
int MVP_SetHomeFlag( Socket_Info *Socket, Device_Data *Data, int home, int debug);
int MVP_SetHomeTrigger( Socket_Info *Socket, Device_Data *Data, int trigger, int debug);
int MVP_SetSpeed( Socket_Info *Socket, Device_Data *Data, int speed, int debug);
int MVP_SetZero( Socket_Info *Socket, Device_Data *Data, int debug);
int MVP_Status( Socket_Info *Socket, Device_Data *Data, int debug);
int MVP_Write( Socket_Info *Socket, char *request, int debug);
