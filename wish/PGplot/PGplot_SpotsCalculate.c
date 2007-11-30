/*
  PGplot_SpotsCalculate.c

  This routine takes the short integer Spots_Array, converts it to a float
    array, Spots_Image, needed by the PGplot routines.  It also keeps a
    running average of each pixel and each sup-apperature, which can be returned
    in Spots_Image for display.  Also, the quad-cell signal (slope) is calculated
    and returned in Spots_Slopes for possible display.

  Written  24may07  DLM

*/

#include "PCR_Wish.h"

#include "PGplot.h"

int PGplot_SpotsCalculate( PGplot_Info *Spots_Info, short *Spots_Array, int debug)
{

  /*
    Local variables
  */
  int i;
  float range, bit;
  float pixel_value;
  float focus;

  /*
    Zero arrays used in slope and pupil calculations
  */
  for(i=0; i<SUB_APPS*SUB_APPS; i++) {
    Spots_Info->spots->x_slopes[i] = 0.0;
    Spots_Info->spots->y_slopes[i] = 0.0;
    Spots_Info->spots->pupil[i] = 0.0;
  }

  /*
    Reset min and max values used to determine image scaling]
  */
  Spots_Info->spots->min = 32768.0;
  Spots_Info->spots->max = 0.0;

  /*
    Convert the Spots from an integer to a float array and find minimum and maximum values
  */
  for(i=0; i<Spots_Info->spots->values*Spots_Info->spots->values; i++) {
    pixel_value = (float) Spots_Array[i];
    Spots_Info->spots->current[i] = pixel_value;
    Spots_Info->spots->average[i] = pixel_value * Spots_Info->spots->average_fraction +
      Spots_Info->spots->average[i] * (1.0 - Spots_Info->spots->average_fraction);
    Spots_Info->spots->x_slopes[Spots_Info->spots->slope_grid[i]] +=
      Spots_Info->spots->average[i] * Spots_Info->spots->x_slopes_factor[i];
    Spots_Info->spots->y_slopes[Spots_Info->spots->slope_grid[i]] +=
      Spots_Info->spots->average[i] * Spots_Info->spots->y_slopes_factor[i];
    Spots_Info->spots->pupil[Spots_Info->spots->slope_grid[i]] += Spots_Info->spots->average[i];
    if ( Spots_Info->spots->average[i] < Spots_Info->spots->min ) Spots_Info->spots->min = Spots_Info->spots->average[i];
    if ( Spots_Info->spots->average[i] > Spots_Info->spots->max ) Spots_Info->spots->max = Spots_Info->spots->average[i];
  }

  /*
    Reset min and max values used to determine image scaling]
  */
  Spots_Info->spots->min_pupil = 32768.0;
  Spots_Info->spots->max_pupil = 0.0;

  /*
    Finish the slope and pupil calculations
  */
  Spots_Info->spots->tilt = 0.0;
  Spots_Info->spots->tip = 0.0;
  Spots_Info->spots->focus = 0.0;
  for(i=0; i<SUB_APPS*SUB_APPS; i++) {
    Spots_Info->spots->x_slopes[i] /= Spots_Info->spots->pupil[i];
    Spots_Info->spots->tilt += Spots_Info->spots->x_slopes[i];
    Spots_Info->spots->y_slopes[i] /= Spots_Info->spots->pupil[i];
    Spots_Info->spots->tip += Spots_Info->spots->y_slopes[i];
    Spots_Info->spots->pupil[i] /= 4.0;
    if ( Spots_Info->spots->pupil[i] < Spots_Info->spots->min_pupil ) Spots_Info->spots->min_pupil = 
      Spots_Info->spots->pupil[i];
    if ( Spots_Info->spots->pupil[i] > Spots_Info->spots->max_pupil ) Spots_Info->spots->max_pupil = 
      Spots_Info->spots->pupil[i];
    focus = Spots_Info->spots->x_slopes[i] * Spots_Info->spots->focus_sin[i] +
      Spots_Info->spots->y_slopes[i] * Spots_Info->spots->focus_cos[i];
    Spots_Info->spots->x_focus[i] = focus * Spots_Info->spots->focus_sin[i];
    Spots_Info->spots->y_focus[i] = focus * Spots_Info->spots->focus_cos[i];
    Spots_Info->spots->focus += focus;
  }
  Spots_Info->spots->tilt /=  Spots_Info->spots->used_subapps;
  Spots_Info->spots->tip /=  Spots_Info->spots->used_subapps;
  Spots_Info->spots->focus /=  Spots_Info->spots->used_subapps;

  /*
    Determine the value range for the color scale, adding a little bit
      so the scale will not rescale every time a new image is displayed

    Image display
  */
  if( Spots_Info->spots->min < Spots_Info->spots->min_lower ||
      Spots_Info->spots->min > Spots_Info->spots->min_upper ||
      Spots_Info->spots->max < Spots_Info->spots->max_lower ||
      Spots_Info->spots->max > Spots_Info->spots->max_upper ) {
    range = (Spots_Info->spots->max - Spots_Info->spots->min);
    bit = range * 0.1;
    Spots_Info->spots->min_lower = Spots_Info->spots->min - bit;
    Spots_Info->spots->min_upper = Spots_Info->spots->min + bit;
    if (Spots_Info->spots->min_lower < 0.0 ) Spots_Info->spots->min_lower = 0.0;
    Spots_Info->spots->max_lower = Spots_Info->spots->max - bit;
    Spots_Info->spots->max_upper = Spots_Info->spots->max + bit;
  }

  /*
    Pupil display
  */
  if( Spots_Info->spots->min_pupil < Spots_Info->spots->min_pupil_lower ||
      Spots_Info->spots->min_pupil > Spots_Info->spots->min_pupil_upper ||
      Spots_Info->spots->max_pupil < Spots_Info->spots->max_pupil_lower ||
      Spots_Info->spots->max_pupil > Spots_Info->spots->max_pupil_upper ) {
    range = (Spots_Info->spots->max_pupil - Spots_Info->spots->min_pupil);
    bit = range * 0.1;
    Spots_Info->spots->min_pupil_lower = Spots_Info->spots->min_pupil - bit;
    Spots_Info->spots->min_pupil_upper = Spots_Info->spots->min_pupil + bit;
    if (Spots_Info->spots->min_pupil_lower < 0.0 ) Spots_Info->spots->min_pupil_lower = 0.0;
    Spots_Info->spots->max_pupil_lower = Spots_Info->spots->max_pupil - bit;
    Spots_Info->spots->max_pupil_upper = Spots_Info->spots->max_pupil + bit;
  }

  /*
    Fractional values of current frame to add to the running average.  In DClient_Spots_Init()
      set it to 1.0 so the first time through the runnning average is the current frame.
    This value is reset here to a reasonable value (~0.2)
  */
  Spots_Info->spots->average_fraction = 0.1;

  return(0);
}
