/*
  Definitions to be include in all VoltMeter routines

  Written  8jan04  DLM

  Altered 27mar04  DLM  Started with OAP.h.  Try to make
                          generic Micro-Mo amplifier/driver routine

  Altered 30nov07  DLM  Included Motor.h in VoltMeter.h
                           NOTE: VoltMeter_Cmd is currently not included in
			         the compilation of PCR_Wish.  The VoltMeter
				 GUI is about 90% done.
*/

/*

  Generic motor definitions

  Written  30mar04  DLM

*/

#include <termios.h>

/*
  Maximum motor I/O line size
*/
#define MOTOR_MAXLINE 256 

/*
  Delay time for motor response
*/
#define MOTOR_DELAY_S 1
#define MOTOR_DELAY_U 0

/*
  Maximum number of times to query motor to see if an event has
    occured.  This will act as a time limit.
*/
#define MOTOR_MAX_TRIES 1000

/*
  VoltMeter controller status information structure
*/
typedef struct _VoltMeter_State {
  int connected;
  int error;
  double res_1;
  double res_2;
  double res_3;
  double res_4;
  double res_5;
  double res_6;
  double res_7;
  double res_8;
  double res_9;
  double res_10;
  double res_11;
  double res_12;
  double res_13;
  double res_14;
  double res_15;
} VoltMeter_State;

/*
  Structure to hold pointers to variables linked between C and TclTk
    used to update values on the GUI when they are change in a C routine.
  This requires the storage of name of the TclTk variable and the pointer
    to the memory location of this variable.
*/
typedef struct _Int_Info {
  char name[STRING_LENGTH];
  int *ptr;
} Int_Info;

typedef struct _Double_Info {
  char name[STRING_LENGTH];
  double *ptr;
} Double_Info;

typedef struct _Gui_Variables {
  Int_Info connected;
  Int_Info error;
  Double_Info res_1;
  Double_Info res_2;
  Double_Info res_3;
  Double_Info res_4;
  Double_Info res_5;
  Double_Info res_6;
  Double_Info res_7;
  Double_Info res_8;
  Double_Info res_9;
  Double_Info res_10;
  Double_Info res_11;
  Double_Info res_12;
  Double_Info res_13;
  Double_Info res_14;
  Double_Info res_15;
} Gui_Variables;

/*
  Motor socket information structure
*/
typedef struct _Motor_Info {
  char motor[10];                 // motor name
  char name[10];                  // drive name
  char address[MOTOR_MAXLINE];    // motor address
  int port;                       // motor port
  int driveid;                    // drive number (0 for RS232)
  int sockfd;                     // socket fd
  int gain;                       // conversion to user units (counts/arcsec)
  int accel;                      // default acceleration
  int speed;                      // default speed
  char in_line[MOTOR_MAXLINE];    // line for input
  char out_line[MOTOR_MAXLINE];   // line for output
  VoltMeter_State state;          // structure with the state of the VoltMeter
} Motor_Info;


/*
  Request to retrieve and error from the HP VoltMeter
*/
#define VOLTMETER_RETRIEVE_ERROR "SYST:ERR?"
#define RESISTANCE_CHANNELS 15

double *Resistance_Value[RESISTANCE_CHANNELS];

int VoltMeter_Close (Motor_Info *Info, int debug);
int VoltMeter_Connect(Motor_Info *Info, int debug);
int VoltMeter_Flush (Motor_Info *Info);
int VoltMeter_Get( Motor_Info *Info, char *request);
int VoltMeter_GuiUpdate( Tcl_Interp *interp, Motor_Info *Info, Gui_Variables *Gui);
int VoltMeter_Init (Motor_Info *Info, int debug);
int VoltMeter_Measure( Motor_Info *Info);
int VoltMeter_ParameterError( Tcl_Interp *interp, Motor_Info *Info, char *type, char *routine,
			      char *request, int required, int found, char *returnString);
int VoltMeter_Read (Motor_Info *Info);
int VoltMeter_RoutineError( Tcl_Interp *interp, Motor_Info *Info, char *type, char *routine,
			    char *request, char *returnString);
int VoltMeter_Set (Motor_Info *Info, char *request, int debug);
int VoltMeter_Write (Motor_Info *Info);
int VoltMeter_ZeroState( Motor_Info *Info, Gui_Variables *Gui);
