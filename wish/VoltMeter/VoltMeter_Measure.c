/*===========================================================================*
**
**  VoltMeter_Measure.c
**
**  Routines to retrive measurements from the HP VoltMeter
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
 * function: VoltMeter_Measure
 *=========================================*/

int VoltMeter_Measure( Motor_Info *Info)
{
  int i, status;

  /*
    Send request to HP VoltMeter to update its measured values
  */
  if ( (status = VoltMeter_Set (Info, "INIT\012", 0 )) ) return(status);

  /*
    Loop over all channels
  */
  for ( i=101; i<101+RESISTANCE_CHANNELS; i++ ) {

    /*
      Set and send the outgoing request
    */
    sprintf (Info->out_line, "DATA:LAST? (@%d)\012", i);
    if ( (status = VoltMeter_Write (Info)) ) return(status);

    /*
      Get the incoming reply
    */
    if ( (status = VoltMeter_Read (Info)) ) return(status);

    *Resistance_Value[i-101] = atof(Info->in_line);

  }

  return(0);
}
