/*
  PGplot_SpotsSetup.c

  This routine creates a 2-D plot with the given parameters.  PGlot_Open
    must be called before this routine to create a device.  The device id
    is stored in the PGplot_Info structure.

  Written  28mar06  Copied example program from
                      http://www.astro.caltech.edu/~tjp/pgplot/tkdriv.html

  Altered  29mar06  Modified so PGplot_TestDraw is no long called directly from tcl
                      but a C routine that is called by PGplot_Cmd.
*/

#include "PCR_Wish.h"

#include "PGplot.h"

int PGplot_SpotsSetup( PGplot_Info *Spots_Info, int debug)
{

  /*
    Make sure that a device has already been opened (PGplot_Open)
  */
  if ( Spots_Info->id <= 0) {
    printf("  PGplot_SpotsSetup: Device not opened yet\n");
    return(-1);
  }

  /*
   * Select the PGPLOT device and draw the plot.
   */
  cpgslct( Spots_Info->id);

  /*
    Don't ever ask the user for input
  */
  cpgask(0);

  /*
    Allow the use of the full window.  cpgvstd(), the default
      viewport settings, does not use a margin of 4 character
      on all sides
  */
  cpgsvp(0.0, 1.0, 0.0, 1.0);

  /*
    Set axis length in units of sub-apperatures
  */
  cpgwnad(0.0, (float) SUB_APPS,
 	  0.0, (float) SUB_APPS);

  /*
    Set the scale needed to stretch the 24*24 or 72*72 array
      onto the plot area requested
  */
  Spots_Info->spots->image_scale = (float) SUB_APPS/ (float) Spots_Info->spots->values;
  /*
    Set the scale needed to stretch the 12x12 pupil image
      onto the plot area requested
  */
  Spots_Info->spots->pupil_scale = 1.0;

  /*
    Set up transformation parameters.  cpgimag uses the equation

      x = Spots_Info->spots->transform[0] + Spots_Info->spots->transform[1]*I + Spots_Info->spots->transform[2]*J
      x = Spots_Info->spots->transform[3] + Spots_Info->spots->transform[4]*I + Spots_Info->spots->transform[5]*J

    where I and J are the indices of the array image[I,J], to
    take the 2-D array and create an image the size requested in
    the original

      pgplot arg1 arg2 ....

    call in tcl

    Image display transformation
  */
  Spots_Info->spots->image_transform[0] = -0.5 * Spots_Info->spots->image_scale;
  Spots_Info->spots->image_transform[1] = Spots_Info->spots->image_scale;
  Spots_Info->spots->image_transform[2] = 0.0;
  Spots_Info->spots->image_transform[3] = -0.5 * Spots_Info->spots->image_scale;
  Spots_Info->spots->image_transform[4] = 0.0;
  Spots_Info->spots->image_transform[5] = Spots_Info->spots->image_scale;

  /*
    Pupil display transformation
  */
  Spots_Info->spots->pupil_transform[0] = -0.5 * Spots_Info->spots->pupil_scale;
  Spots_Info->spots->pupil_transform[1] = Spots_Info->spots->pupil_scale;
  Spots_Info->spots->pupil_transform[2] = 0.0;
  Spots_Info->spots->pupil_transform[3] = -0.5 * Spots_Info->spots->pupil_scale;
  Spots_Info->spots->pupil_transform[4] = 0.0;
  Spots_Info->spots->pupil_transform[5] = Spots_Info->spots->pupil_scale;

  if ( debug ) {
    printf("  PGplot_SpotsSetup: Scale and transform set for %dx%d spots image\n",
	   Spots_Info->spots->values, Spots_Info->spots->values);
  }

  return(0);
}
