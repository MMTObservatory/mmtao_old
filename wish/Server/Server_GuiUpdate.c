/*===========================================================================*

  Server_GuiUpdate.c

  Update variable linked between C and TclTk

  Written  29jan07  DLM  Started with Client_GuiUpdate.c

  ===========================================================================*/

#include "PCR_Wish.h"
#include "DClient.h"

/*=========================================
 * function: Server_GuiUpdate
 *=========================================*/
int Server_GuiUpdate( Tcl_Interp *interp, Server_Info *Info, Gui_Variables *Gui)
{

  *Gui->connected.ptr = Info->connected;
  Tcl_UpdateLinkedVar (interp, Gui->connected.name);
    
  *Gui->error.ptr = Info->error;
  Tcl_UpdateLinkedVar (interp, Gui->error.name);

  return(0);
}
