/*===========================================================================*
**
**  Server_ZeroState.c
**
**  State all the state values in the structure Info->state to zero and
**    create the TclTk variable names need to update shared varibles.
**
**  Returns :  0 success
**            -1 error
**
**  Written   17nov04  DLM  Started with F15stage_ZeroState.c
**
**===========================================================================*/

#include "PCR_Wish.h"

#include "DClient.h"

/*=========================================
 * function: Server_ZeroState
 *=========================================*/

int Server_ZeroState( Server_Info *Info)
{

  strcpy( Info->name, "none");
  strcpy( Info->type, "none");
  strcpy( Info->address, "none");
  Info->port = 0;
  Info->error = 0;
  Info->connected = 0;
  Info->fd = 0;

  return(0);
}
