/* Fwheel.h
 */
#define FWHEEL_LOOPER_WAIT	1000000

#define FW_MAXLINE  32

// This many filter positions on wheel.
#define FW_NFILTERS 8

// First position is this many steps past home
#define FW_OFFSET  17

// Steps from position to position (33).
#define FW_STEPSIZE (264 / FW_NFILTERS)

// At most this many steps chasing home
#define FW_MAX_HOME	500

// lots of delays
#define FW_PULSE_DELAY   10000
#define FW_CHECK_DELAY   10000
#define FW_HOME_DELAY    10000
#define FW_MOVE_DELAY    10000

// define controls for turning home on or off
#define FW_HOMEON  "0"
#define FW_HOMEOFF "1"

//  XXX - how in the world can this be the same as
//  STEPA and HOMEON ???
#define FW_ALLOFF  "0"

#define FW_HOMEASK "?"

// define controls for 'stepping' the wheel
#define FW_STEPA   "0"
#define FW_STEPB   "2"

// A bit to check after we interrogate via HOMEASK
#define FW_HOMEBIT 0x1

// Prototypes
int Fwheel_Read ( Socket_Info *, char *, int );
int Fwheel_Write( Socket_Info *, char *, int );
int Fwheel_Send ( Socket_Info *, char *, int );

int Fwheel_Step ( Socket_Info *, int );
void * Fwheel_Home ( void * );
int Fwheel_SeekHome ( Socket_Info *, Device_Data *, int, int );
int Fwheel_MoveCount ( Socket_Info *, Device_Data *, int, int );
int Fwheel_MoveThird ( Socket_Info *, Device_Data *, int );
int Fwheel_MoveAbsolute ( Socket_Info *, Device_Data *, double, int );
int Fwheel_MoveRelative ( Socket_Info *, Device_Data *, double, int );
int Fwheel_PrintDevice ( Socket_Info *, Device_Data * );
void Fwheel_FilterSetPos ( Device_Data *, int );

int Fwheel_Process( Device_Info *, char * );
void * Fwheel_Looper( void * );

/* THE END */
