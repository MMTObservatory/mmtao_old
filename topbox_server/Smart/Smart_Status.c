/*===========================================================================*
**
**  Smart_Status.c
**
**  Routines to retrive information from an Smart device controller
**
**  Returns :  0 success
**            -1 error
**
**  Written  14aug07  DLM  Started with FSM_Status.
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Status.h"
#include "Smart.h"

/*=========================================
 * function: Smart_Status
 *=========================================*/

int
Smart_Status( Socket_Info *Socket, Device_Data *Data, int debug)
{
  char reply[STRING_LENGTH];
  int rv;

  rv = Smart_Cmd ( Socket, Data, "RP", reply, debug );
  if ( rv )
    return -1;

  Data->position = atof(reply) / Data->gain;

  rv = Smart_Cmd ( Socket, Data, "RBt", reply, debug );
  if ( rv )
    return -1;

  Data->moving = atoi(reply);

  rv = Smart_Cmd ( Socket, Data, "RBm", reply, debug );
  if ( rv )
    return -1;

  Data->negative = atoi(reply);

  rv = Smart_Cmd ( Socket, Data, "RBp", reply, debug );
  if ( rv )
    return -1;

  Data->positive = atoi(reply);

  rv = Smart_Cmd ( Socket, Data, "Rh", reply, debug );
  if ( rv )
    return -1;

  Data->home = atoi(reply);

#ifdef notdef
  Data->in_position = xxx;
  Data->error = xxx;
  Data->enabled = xxx;
#endif

  if ( debug )
    Smart_PrintDevice ( Socket, Data );

  return 0;
}

int 
Smart_PrintDevice ( Socket_Info *Socket, Device_Data *Data )
{
    printf("  Status of %s:\n", Socket->name );
    printf("             Position = %.4f\n", Data->position );
    printf("                 Home = %d: %s\n", Data->home,
	 (Data->home)?"   Device is homed":"***Device is NOT homed***");
    printf("               Moving = %d: %s\n", Data->moving,
	 (Data->moving)?"***Move in progress***":"   Not moving");
    if ( Data->negative )
	printf("             at the negative limit\n" );
    if ( Data->positive )
	printf("             at the positive limit\n" );

    fflush(stdout);

    return(0);
}

/* THE END */
