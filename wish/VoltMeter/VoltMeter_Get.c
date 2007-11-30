/*===========================================================================*
**
**  VoltMeter_Get.c
**
**  Routines to retrive information from a motor
**
**  Returns :  0 success
**            -1 error
**
**  Written  30mar04  DLM  Started with OAP_Status.  Trying to make this a
**                           a generic motor driver.
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "VoltMeter.h"

/*=========================================
 * function: VoltMeter_Get
 *=========================================*/

int VoltMeter_Get( Motor_Info *Info, char *request)
{
  int i;

  /*
    Set and send the outgoing request
  */
  sprintf (Info->out_line, request);
  if ( (i = VoltMeter_Write (Info)) ) return(i);

  /*
    Get the incoming reply
  */
  if ( (i = VoltMeter_Read (Info)) ) return(i);

  return(0);
}
