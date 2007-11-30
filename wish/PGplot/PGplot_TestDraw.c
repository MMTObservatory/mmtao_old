/*
  PGplot_Open.c

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

int PGplot_TestDraw( PGplot_Info *Info, double x_min, double x_max, int debug)
{

  float x;
  int i;

  /*
    Make sure that a device has already been opened (PGplot_Open)
  */
  if ( Info->id <= 0) {
    printf("  PGplot_Open: Device not opened yet\n");
    return(-1);
  }

  /*
   * Select the PGPLOT device and draw the plot.
   */
  cpgslct( Info->id);
  cpgpage();
  cpgsch(1.0);
  cpgswin( x_min, x_max, 0.0, 1.0);
  cpgsci( 1);
  cpgbox( "BCNST", 0, 0, "BCNST", 0, 0);
  cpgsci( 2);
  cpgmove( 0.0, 0.0);
  for(i=1; i<100; i++) {
    x = i/100.0;
    cpgdraw( x, x*x);
  }
  return(0);
}
