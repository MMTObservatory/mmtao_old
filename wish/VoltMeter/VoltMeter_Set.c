/*===========================================================================*
**
**  VoltMeter_Set.c
**
**      VoltMeter_Set()
**
**  Routines to set parameter on the HP VoltMeter
**
**  Returns :  0 success
**            -1 error
**
**  Written  13apr04  DLM  
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "VoltMeter.h"

/*=========================================
 * function: VoltMeter_Set
 *=========================================*/

int VoltMeter_Set( Motor_Info *Info, char *request, int debug)
{
  int i;

  if ( debug ) {
    printf("  VoltMeter_Set: Sending %s:%s --> %s",
	   Info->motor, Info->name, request);
  }

  /*
    Set and send the outgoing request
  */
  sprintf (Info->out_line, request);
  if ( (i=VoltMeter_Write (Info)) ) {
    printf("error setting\n");
    return(i);
  }

  /*
    Send a request to see if an error ocurred
  */
  sprintf (Info->out_line, "%s\012", VOLTMETER_RETRIEVE_ERROR );
  if ( (i=VoltMeter_Write (Info)) ) {
    printf("error requesting error\n");
    return(i);
  }

  /*
    Get the error request reply
  */
  if ( (i=VoltMeter_Read (Info)) ) {
    printf("error reading error\n");
    return(i);
  }

  return(0);
}
