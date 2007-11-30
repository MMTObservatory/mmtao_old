/*=======================================================================
 * Status_ZeroFlags.c
 *=======================================================================

  Set all Device flags to 0 or 0.0

  Written  14aug07  DLM  Started with Status_SetFlagPointers.c

*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "FSM.h"
#include "Status.h"

/*================================================================================*
 * Set all Device flags to 0 or 0.0
 *================================================================================*/
int Status_ZeroFlags( Device_Data *Data)
{

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
