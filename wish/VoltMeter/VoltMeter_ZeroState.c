/*===========================================================================*
**
**  VoltMeter_ZeroState.c
**
**  State all the state values in the structure Info->state to zero and
**    create the TclTk variable names need to update shared varibles.
**
**  Returns :  0 success
**            -1 error
**
**  Written   12apr04  DLM  
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "VoltMeter_Globals.h"

#include "VoltMeter.h"

/*=========================================
 * function: VoltMeter_ZeroState
 *=========================================*/

int VoltMeter_ZeroState( Motor_Info *Info, Gui_Variables *Gui)
{

  Info->state.connected = 0;
  Info->state.error = 0;
  Info->state.res_1 = 0.0;
  Info->state.res_2 = 0.0;
  Info->state.res_3 = 0.0;
  Info->state.res_4 = 0.0;
  Info->state.res_5 = 0.0;
  Info->state.res_6 = 0.0;
  Info->state.res_7 = 0.0;
  Info->state.res_8 = 0.0;
  Info->state.res_9 = 0.0;
  Info->state.res_10 = 0.0;
  Info->state.res_11 = 0.0;
  Info->state.res_12 = 0.0;
  Info->state.res_13 = 0.0;
  Info->state.res_14 = 0.0;
  Info->state.res_15 = 0.0;

  strcpy( Gui->connected.name, "VoltMeter_Connected");
  strcpy( Gui->error.name, "VoltMeter_Error");
  strcpy( Gui->res_1.name, "VoltMeter_Res_1");
  strcpy( Gui->res_2.name, "VoltMeter_Res_2");
  strcpy( Gui->res_3.name, "VoltMeter_Res_3");
  strcpy( Gui->res_4.name, "VoltMeter_Res_4");
  strcpy( Gui->res_5.name, "VoltMeter_Res_5");
  strcpy( Gui->res_6.name, "VoltMeter_Res_6");
  strcpy( Gui->res_7.name, "VoltMeter_Res_7");
  strcpy( Gui->res_8.name, "VoltMeter_Res_8");
  strcpy( Gui->res_9.name, "VoltMeter_Res_9");
  strcpy( Gui->res_10.name, "VoltMeter_Res_10");
  strcpy( Gui->res_11.name, "VoltMeter_Res_11");
  strcpy( Gui->res_12.name, "VoltMeter_Res_12");
  strcpy( Gui->res_13.name, "VoltMeter_Res_13");
  strcpy( Gui->res_14.name, "VoltMeter_Res_14");
  strcpy( Gui->res_15.name, "VoltMeter_Res_15");

  Gui->connected.ptr = &VoltMeter_Connected;
  Gui->error.ptr = &VoltMeter_Error;
  Gui->res_1.ptr = &VoltMeter_Res_1;
  Gui->res_2.ptr = &VoltMeter_Res_2;
  Gui->res_3.ptr = &VoltMeter_Res_3;
  Gui->res_4.ptr = &VoltMeter_Res_4;
  Gui->res_5.ptr = &VoltMeter_Res_5;
  Gui->res_6.ptr = &VoltMeter_Res_6;
  Gui->res_7.ptr = &VoltMeter_Res_7;
  Gui->res_8.ptr = &VoltMeter_Res_8;
  Gui->res_9.ptr = &VoltMeter_Res_9;
  Gui->res_10.ptr = &VoltMeter_Res_10;
  Gui->res_11.ptr = &VoltMeter_Res_11;
  Gui->res_12.ptr = &VoltMeter_Res_12;
  Gui->res_13.ptr = &VoltMeter_Res_13;
  Gui->res_14.ptr = &VoltMeter_Res_14;
  Gui->res_15.ptr = &VoltMeter_Res_15;

  Resistance_Value[0] = &Info->state.res_1;
  Resistance_Value[1] = &Info->state.res_2;
  Resistance_Value[2] = &Info->state.res_3;
  Resistance_Value[3] = &Info->state.res_4;
  Resistance_Value[4] = &Info->state.res_5;
  Resistance_Value[5] = &Info->state.res_6;
  Resistance_Value[6] = &Info->state.res_7;
  Resistance_Value[7] = &Info->state.res_8;
  Resistance_Value[8] = &Info->state.res_9;
  Resistance_Value[9] = &Info->state.res_10;
  Resistance_Value[10] = &Info->state.res_11;
  Resistance_Value[11] = &Info->state.res_12;
  Resistance_Value[12] = &Info->state.res_13;
  Resistance_Value[13] = &Info->state.res_14;
  Resistance_Value[14] = &Info->state.res_15;

  return(0);
}
