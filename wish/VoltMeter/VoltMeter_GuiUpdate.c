/*===========================================================================*
**
**  VoltMeter_GuiUpdate.c
**
**  Update variable linked between C and TclTk
**
**  Returns :  0 success
**            -1 error
**
**  Written   8apr04  DLM  
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "VoltMeter.h"

/*=========================================
 * function: VoltMeter_GuiUpdate
 *=========================================*/

int VoltMeter_GuiUpdate( Tcl_Interp *interp, Motor_Info *Info, Gui_Variables *Gui)
{
  *Gui->connected.ptr = Info->state.connected;
  Tcl_UpdateLinkedVar (interp, Gui->connected.name);

  *Gui->error.ptr = Info->state.error;
  Tcl_UpdateLinkedVar (interp, Gui->error.name);

  *Gui->res_1.ptr = Info->state.res_1;
  Tcl_UpdateLinkedVar (interp, Gui->res_1.name);

  *Gui->res_2.ptr = Info->state.res_2;
  Tcl_UpdateLinkedVar (interp, Gui->res_2.name);

  *Gui->res_3.ptr = Info->state.res_3;
  Tcl_UpdateLinkedVar (interp, Gui->res_3.name);

  *Gui->res_4.ptr = Info->state.res_4;
  Tcl_UpdateLinkedVar (interp, Gui->res_4.name);

  *Gui->res_5.ptr = Info->state.res_5;
  Tcl_UpdateLinkedVar (interp, Gui->res_5.name);

  *Gui->res_6.ptr = Info->state.res_6;
  Tcl_UpdateLinkedVar (interp, Gui->res_6.name);

  *Gui->res_7.ptr = Info->state.res_7;
  Tcl_UpdateLinkedVar (interp, Gui->res_7.name);

  *Gui->res_8.ptr = Info->state.res_8;
  Tcl_UpdateLinkedVar (interp, Gui->res_8.name);

  *Gui->res_9.ptr = Info->state.res_9;
  Tcl_UpdateLinkedVar (interp, Gui->res_9.name);

  *Gui->res_10.ptr = Info->state.res_10;
  Tcl_UpdateLinkedVar (interp, Gui->res_10.name);

  *Gui->res_11.ptr = Info->state.res_11;
  Tcl_UpdateLinkedVar (interp, Gui->res_11.name);

  *Gui->res_12.ptr = Info->state.res_12;
  Tcl_UpdateLinkedVar (interp, Gui->res_12.name);

  *Gui->res_13.ptr = Info->state.res_13;
  Tcl_UpdateLinkedVar (interp, Gui->res_13.name);

  *Gui->res_14.ptr = Info->state.res_14;
  Tcl_UpdateLinkedVar (interp, Gui->res_14.name);

  *Gui->res_15.ptr = Info->state.res_15;
  Tcl_UpdateLinkedVar (interp, Gui->res_15.name);

  return(0);
}
