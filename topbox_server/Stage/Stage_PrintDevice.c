/*===========================================================================*
**
**  Stage_PrintDevice()
**
**  Routine to print device information stored in the Device_Data structure
**
**  Returns :  0 success
**            -1 error
**
**  Modified  24aug07  tjt  Started with MVP_PrintDevice.c
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Stage.h"

/*=========================================
 * function: Stage_PrintDevice
 *=========================================*/

int Stage_PrintDevice( Socket_Info *Socket, Device_Data *Data)
{
  /*
    Print to the terminal the device status currently in the Device_Data structure
  */
  printf("  Status of %s:\n", Socket->name);
  printf("             Position = %.2f\n", Data->position);
  printf("                 Home = %d: %s\n", Data->home,
	 (Data->home)?"   Device is homed":"***Device is NOT homed***");
  printf("              Enabled = %d: %s\n", Data->enabled,
	 (Data->enabled)?"   Device is enabled":"***Device is NOT enabled or disabled by error***");
  printf("               Moving = %d: %s\n", Data->moving,
	 (Data->moving)?"***Move in progress***":"   Not moving");
  printf("       Positive Limit = %d: %s\n", Data->positive,
	 (Data->positive)?"***Device is at positive limit***":"   Device is not at positive limit");
  printf("       Negative Limit = %d: %s\n", Data->negative,
	 (Data->negative)?"***Device is at negative limit***":"   Device is not at negative limit");

  fflush(stdout);

  return(0);
}

/* THE END */
