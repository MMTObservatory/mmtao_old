/*===========================================================================*
**
**  Fwheel_PrintDevice()
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
#include "Fwheel.h"

int Fwheel_PrintDevice( Socket_Info *Socket, Device_Data *Data)
{
  printf("  Status of %s:\n", Socket->name );
  printf("             Position = %.2f\n", Data->position );
  printf("               Filter = %s\n", Data->filter );
  printf("                 Home = %d: %s\n", Data->home,
	 (Data->home)?"   Device is homed":"***Device is NOT homed***");
  printf("               Moving = %d: %s\n", Data->moving,
	 (Data->moving)?"***Move in progress***":"   Not moving");

  fflush(stdout);

  return(0);
}

/* THE END */
