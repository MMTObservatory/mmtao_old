/*
  Smart motor definitions
  Hacked  12sep07 tjt
*/

#include "Socket.h"

/* This is counts per degree */
#define ROTATION_GAIN   137.4

#define ROTATION_ABSOLUTE_ZERO   581141

/* counts per motion required to match hexapod motion of 1 micron */
#define FOCUS_GAIN   1500

/* Maximum number of times to check for motion to stop during
 * a homing sequence.
 */
#define SMART_MAX_TRIES 1000

#define SMART_LOOPER_WAIT 10000

/* Smart_IO.c */
void Smart_Flush ( Socket_Info *, int );
// int Smart_Read ( Socket_Info *, char *, int );
// int Smart_Write ( Socket_Info *, Device_Data *, char *, int );
int Smart_ReadEncoder ( int *, int );
int Smart_Cmd ( Socket_Info *, Device_Data *, char *, char *, int );
int Smart_Cmdw ( Socket_Info *, Device_Data *, char *, int );
int Smart_Init ( Socket_Info *, int );

/* Smart_Home.c */
void * Smart_Home ( void * );

/* Smart_Move.c */
int Smart_MoveAbsolute ( Socket_Info *, Device_Data *, double, int );
int Smart_MoveRelative ( Socket_Info *, Device_Data *, double, int );
int Smart_MoveAbort ( Socket_Info *, Device_Data *, int );

/* Smart_Set.c */
int Smart_SetPosition ( Socket_Info *, Device_Data *, int );
int Smart_SetSpeed ( Socket_Info *, Device_Data *, double, int );

/* Smart_Status.c */
int Smart_Status ( Socket_Info *, Device_Data *, int );
int Smart_PrintDevice ( Socket_Info *, Device_Data * );

/* Smart_Looper.c */
void * Smart_Looper ( void * );

/* Smart_Process.c */
int Smart_Process ( Device_Info *, char * );

/* THE END */
