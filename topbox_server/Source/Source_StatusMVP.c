/*
  Source_StatusMVP.c

  This routine creates the status string for a device controlled
    by an MVP controller from the flags in the Device_Data structure

  Written  15aug07  DLM

*/

#include "Topbox.h"
#include "Device.h"
#include "MVP.h"
#include "Source.h"

/*================================================================================*
 * Create a status string
 *================================================================================*/
int Source_StatusMVP( Socket_Info *Socket, Device_Data *Data, char *statusString, int debug)
{
  int status;

  /*
    Encrypt the status word from the device flags
  */
  status = Data->moving * MVP_MOVE_IN_PROGRESS +
    Data->in_position * MVP_IN_POSITION +
    Data->error * MVP_ERROR +
    (!Data->enabled) * MVP_NOT_ENABLED +
    Data->positive * MVP_POSITIVE_LIMIT +
    Data->negative * MVP_NEGATIVE_LIMIT +
    Data->event_2 * MVP_EXTERNAL_EVENT_2;

  sprintf( statusString, "01234567\n  %8x\r", status);

  if ( debug ) {
    printf("  Source_StatusMVP: Sending status word %8x\n", status);
  }

  if ( debug ) {
    MVP_PrintDevice( Socket, Data);
  }

  return(0);
}
