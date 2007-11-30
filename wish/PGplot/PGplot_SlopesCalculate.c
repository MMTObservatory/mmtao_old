/*
  PGplot_SlopesCalculate.c

  This routine takes the short integer Slopes_Vector, converts it to a float
    array, Slopes_Image, needed by the PGplot routines.  It also keeps a
    running average of each pixel and each sup-apperature, which can be returned
    in Slopes_Image for display.  Also, the quad-cell signal (slope) is calculated
    and returned in Slopes_Slopes for possible display.

  Written  24may07  DLM

*/

#include "PCR_Wish.h"
#include "PGplot.h"
#include "WFSC.h"

int PGplot_SlopesCalculate( PGplot_Info *Slopes_Info, short *Slopes_Vector, int debug)
{

  /*
    Local variables
  */
  int i;
  float slope_value;

  /*
    Convert the Slopes from an integer to a float array and find minimum and maximum values.
  */
  Slopes_Info->slopes->tilt = 0.0;
  for(i=0; i<Slopes_Info->slopes->values; i++) {
    slope_value = (float) Slopes_Vector[i];
    Slopes_Info->slopes->current_x[i] = slope_value*subapp_used[i];
    Slopes_Info->slopes->average_x[i] = slope_value * Slopes_Info->slopes->average_fraction * subapp_used[i] +
      Slopes_Info->slopes->average_x[i] * (1.0 - Slopes_Info->slopes->average_fraction);
    Slopes_Info->slopes->tilt += Slopes_Info->slopes->average_x[i];
    if ( Slopes_Info->slopes->average_x[i] < Slopes_Info->slopes->min ) 
      Slopes_Info->slopes->min = Slopes_Info->slopes->average_x[i];
    if ( Slopes_Info->slopes->average_x[i] > Slopes_Info->slopes->max )
      Slopes_Info->slopes->max = Slopes_Info->slopes->average_x[i];
  }
  Slopes_Info->slopes->tilt /=  Slopes_Info->slopes->used_subapps;

  Slopes_Info->slopes->tip = 0.0;
  for(i=0; i<Slopes_Info->slopes->values; i++) {
    slope_value = (float) Slopes_Vector[Slopes_Info->slopes->values+i];
    Slopes_Info->slopes->current_y[i] = slope_value * subapp_used[i];
    Slopes_Info->slopes->average_y[i] = slope_value * Slopes_Info->slopes->average_fraction * subapp_used[i] +
      Slopes_Info->slopes->average_y[i] * (1.0 - Slopes_Info->slopes->average_fraction);
    Slopes_Info->slopes->tip += Slopes_Info->slopes->average_y[i];
    if ( Slopes_Info->slopes->average_y[i] < Slopes_Info->slopes->min )
      Slopes_Info->slopes->min = Slopes_Info->slopes->average_y[i];
    if ( Slopes_Info->slopes->average_y[i] > Slopes_Info->slopes->max )
      Slopes_Info->slopes->max = Slopes_Info->slopes->average_y[i];
  }
  Slopes_Info->slopes->tip /=  Slopes_Info->slopes->used_subapps;

  /*
    Fractional values of current frame to add to the running average.  In DClient_Slopes_Init()
      set it to 1.0 so the first time through the runnning average is the current frame.
    This value is reset here to a reasonable value (~0.2)
  */
  Slopes_Info->slopes->average_fraction = 0.1;

  return(0);
}
