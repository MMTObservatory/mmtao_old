/*=======================================================================
 * Status_InitFlags.c
 *=======================================================================

  Set all Device flags to 0 or 0.0

  Written  14aug07  DLM  Started with Status_SetFlagPointers.c

*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "FSM.h"
#include "OAP.h"
#include "Power.h"
#include "Smart.h"
#include "Stage.h"
#include "Status.h"

/*================================================================================*
 * Set all Device flags to 0 or 0.0
 *================================================================================*/
int Status_InitFlags( Socket_Info *Socket, Device_Data *Data)
{

  int i;

  Socket->connected = 0;
  Socket->error = 0;

  if ( !strncmp( Socket->name, "FSM Tip Motor", strlen(Socket->name)) ) {

    Data->gain = FSM_TIP_GAIN;
    Data->speed = FSM_SPEED;
    Data->accel = FSM_ACCEL;

  } else if ( !strncmp( Socket->name, "FSM Tilt Motor", strlen(Socket->name)) ) {

    Data->gain = FSM_TILT_GAIN;
    Data->speed = FSM_SPEED;
    Data->accel = FSM_ACCEL;

  } else if ( !strncmp( Socket->name, "OAP Tip Motor", strlen(Socket->name)) ) {

    Data->gain = OAP_TIP_GAIN;
    Data->speed = OAP_SPEED;
    Data->accel = OAP_ACCEL;

  } else if ( !strncmp( Socket->name, "OAP Tilt Motor", strlen(Socket->name)) ) {

    Data->gain = OAP_TILT_GAIN;
    Data->speed = OAP_SPEED;
    Data->accel = OAP_ACCEL;

  } else if ( !strncmp( Socket->name, "Indigo Stage", strlen(Socket->name)) ) {

    Data->gain = INDIGO_GAIN;
    Data->speed = INDIGO_SPEED;
    Data->accel = 0;

  } else if ( !strncmp( Socket->name, "F15 Stage", strlen(Socket->name)) ) {

    Data->gain = F15_GAIN;
    Data->speed = F15_SPEED;
    Data->accel = 0;

  } else if ( !strncmp( Socket->name, "Filter Wheel", strlen(Socket->name)) ) {

    Data->gain = 0;
    Data->speed = 0;
    Data->accel = 0;
    strcpy( Data->filter, "bogus");

  } else if ( !strncmp( Socket->name, "Power", strlen(Socket->name)) ) {

    Data->gain = 0;
    Data->speed = 0;
    Data->accel = 0;
    for ( i=0; i<POWER_NCHAN; i++ )
      Data->relays[i] = 0;

  } else if ( !strncmp( Socket->name, "Focus", strlen(Socket->name)) ) {

    strcpy( Data->name, "Focus");
    Data->gain = FOCUS_GAIN;
    Data->speed = 0;
    Data->accel = 0;

  } else if ( !strncmp( Socket->name, "Rotation", strlen(Socket->name)) ) {

    strcpy( Data->name, "Rotation");
    Data->gain = ROTATION_GAIN;
    Data->speed = 0;
    Data->accel = 0;

  } else  {

    printf("  Status_InitFlags: name not found: %s\n", Socket->name);
    fflush(stdout);
    return(-1);

  }

  Data->home = 0;
  Data->position = 0.0;
  Data->moving = 0;
  Data->in_position = 0;
  Data->error = 0;
  Data->enabled = 0;
  Data->negative = 0;
  Data->positive = 0;
  Data->event_2 = 0;

  return(0);
}
