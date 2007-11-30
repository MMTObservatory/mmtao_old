/*===========================================================================*
**
**  Power_Status.c
**
**  Routines to retrive information from an Power device controller
**
**  Returns :  0 success
**            -1 error
**
**  Modified  24aug07  tjt  Started with MVP_Status.c
**                          Added get_thing() routine
**
**  Altered   26aug07  DLM  Added a check of the enable flag
**                          Removed set from "set HOME=*"
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Status.h"
#include "Power.h"

/*=========================================
 * function: Power_Status
 *=========================================*/

int Power_Status( Socket_Info *Socket, Device_Data *Data, int debug)
{
  char reply[STRING_LENGTH];

  if ( Power_Info ( Socket, reply, debug ) < 0 )
    return -1;

  /* Yes, in reverse order */
  sscanf( reply, "%d %d %d %d %d %d %d %d",
      &Data->relays[7],
      &Data->relays[6],
      &Data->relays[5],
      &Data->relays[4],
      &Data->relays[3],
      &Data->relays[2],
      &Data->relays[1],
      &Data->relays[0] );


  return 0;
}

/* THE END */
