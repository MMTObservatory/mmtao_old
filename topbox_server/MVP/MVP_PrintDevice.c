/*===========================================================================*
**
**  MVP_PrintDevice()
**
**  Routine to print device information stored in the Device_Data structure
**
**  Returns :  0 success
**            -1 error
**
**  Written  14aug07  DLM  Started with MVP_PrintStatus.
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "MVP.h"

/*=========================================
 * function: MVP_PrintDevice
 *=========================================*/

int MVP_PrintDevice( Socket_Info *Socket, Device_Data *Data)
{
  /*
    Print to the terminal the device status currently in the Device_Data structure
  */
  printf("  Status of %s:\n", Socket->name);
  printf("             Position = %.2f\n", Data->position);
  printf("               Moving = %d: %s\n", Data->moving,
	 (Data->moving)?"***Move in progress***":"   Not moving");
  printf("          In_Position = %d: %s\n", Data->in_position,
	 (Data->in_position)?"   Device is in position":"***Device is NOT in position***");
  printf("              Enabled = %d: %s\n", Data->enabled,
	 (Data->enabled)?"   Device is enabled":"***Device is NOT enabled or disabled by error***");
  printf("                Error = %d: %s\n", Data->error,
	 (Data->error)?"***An error has occured***":"   No errors");
  printf("       Positive Limit = %d: %s\n", Data->positive,
	 (Data->positive)?"***Device is at positive limit***":"   Device is not at positive limit");
  printf("       Negative Limit = %d: %s\n", Data->negative,
	 (Data->negative)?"***Device is at negative limit***":"   Device is not at negative limit");
  printf("       Event_2 = %d: %s\n", Data->event_2,
	 (Data->event_2)?"   Home Flag is ON":"   Home Flag is Off");

  fflush(stdout);

  return(0);
}
