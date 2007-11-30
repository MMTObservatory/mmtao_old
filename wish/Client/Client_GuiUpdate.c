/*===========================================================================*

  Client_GuiUpdate.c

  Update variable linked between C and TclTk

  Written  17jun05  DLM  Started with FSM_GuiUpdate.c

  ===========================================================================*/

#include "PCR_Wish.h"
#include "Client.h"

/*=========================================
 * function: Client_GuiUpdate
 *=========================================*/
int Client_GuiUpdate( Tcl_Interp *interp, Client_Info *Info, Gui_Variables *Gui)
{

  *Gui->connected.ptr = Info->connected;
  Tcl_UpdateLinkedVar (interp, Gui->connected.name);
    
  *Gui->error.ptr = Info->error;
  Tcl_UpdateLinkedVar (interp, Gui->error.name);

  return(0);
}
