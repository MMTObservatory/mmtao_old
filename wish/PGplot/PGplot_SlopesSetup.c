/*
  PGplot_SlopesSetup.c

  This routine creates a 2-D plot with the given parameters.  PGlot_Open
    must be called before this routine to create a device.  The device id
    is stored in the PGplot_Info structure.

  Written  28mar06  Copied example program from
                      http://www.astro.caltech.edu/~tjp/pgplot/tkdriv.html

  Altered  29mar06  Modified so PGplot_TestDraw is no long called directly from tcl
                      but a C routine that is called by PGplot_Cmd.

  Altered  13jun07  Started with PGplot_SpotsSetup.c

*/

#include "PCR_Wish.h"

#include "PGplot.h"

int PGplot_SlopesSetup( PGplot_Info *Slopes_Info, int debug)
{

  /*
    Make sure that a device has already been opened (PGplot_Open)
  */
  if ( Slopes_Info->id <= 0) {
    printf("  PGplot_SlopesSetup: Device not opened yet\n");
    return(-1);
  }

  /*
   * Select the PGPLOT device and draw the plot.
   */
  cpgslct( Slopes_Info->id);

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
    Set the scale needed to stretch the 12x12 pupil image
      onto the plot area requested
  */
  Slopes_Info->slopes->scale = 1.0;

  /*
    Set up transformation parameters.  cpgimag uses the equation

      x = Spots_Info->spots->transform[0] + Spots_Info->spots->transform[1]*I + Spots_Info->spots->transform[2]*J
      x = Spots_Info->spots->transform[3] + Spots_Info->spots->transform[4]*I + Spots_Info->spots->transform[5]*J

    where I and J are the indices of the array image[I,J], to
    take the 2-D array and create an image the size requested in
    the original

      pgplot arg1 arg2 ....

    call in tcl
  */
  Slopes_Info->slopes->transform[0] = -0.5 * Slopes_Info->slopes->scale;
  Slopes_Info->slopes->transform[1] = Slopes_Info->slopes->scale;
  Slopes_Info->slopes->transform[2] = 0.0;
  Slopes_Info->slopes->transform[3] = -0.5 * Slopes_Info->slopes->scale;
  Slopes_Info->slopes->transform[4] = 0.0;
  Slopes_Info->slopes->transform[5] = Slopes_Info->slopes->scale;

  if ( debug ) {
    printf("  PGplot_SlopesSetup: Scale and transform set for %d slopes\n",
	   Slopes_Info->slopes->values);
  }

  return(0);
}
