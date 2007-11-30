/*
  PGplot_SlopesUpdate.c

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

int PGplot_SlopesUpdate( PGplot_Info *Slopes_Info, int Slopes_Type, int Slopes_Arrow, int debug)
{

  /*
    Make sure that a device has already been opened (PGplot_Open)
  */
  if ( Slopes_Info->id <= 0) {
    printf("  PGplot_SlopesUpdate: Device not opened yet\n");
    return(-1);
  }
  cpgslct(Slopes_Info->id);

  /*
    Buffer all changes to the display and then update only once with
      cpgebuf() later
  */
  cpgbbuf();

  /*
    Clear the screen so changes in scale values will not appear
  */
  cpgeras();

  //========================================================
  // Create the plot area as large as will fit.  By default,
  //   there is an unusabel margin around the plot
  //========================================================

  cpgsvp(0.04, 0.99, 0.05, 0.99);

  //=============================================================
  // Label the sub-apps and put quad cell boundary lines in white
  //=============================================================
  /*
    Set the scale for plot labels
  */
  cpgsch(1.5);

  /*
    Set axis length in units of sub-apperatures
  */
  cpgwnad(0.5, (float) SUB_APPS + 0.5,
 	  0.5, (float) SUB_APPS + 0.5 );

  /*
    Choose white for labels
  */
  cpgsci( 1);

  /*
    Draw the axis with tick marks and labels
  */
  cpgbox( "BCN", 1, 0, "BCN", 1, 0);

  //========================================================
  // Put sub-app divisions in blue
  //========================================================
  /*
    Set axis length in units of sub-apperatures
  */
  cpgwnad(0.0, (float) SUB_APPS,
 	  0.0, (float) SUB_APPS);

  /*
    Choose blue for sub-apperature lines
  */
  cpgsci( 1);

  /*
    Draw the axis with tick marks and labels
  */
  cpgbox( "G", 1, 0, "G", 1, 0);

  /*
    Put slope vectors on the image
  */
  if ( Slopes_Type == 0 ){
    cpgvect(Slopes_Info->slopes->current_x, Slopes_Info->slopes->current_y, SUB_APPS, SUB_APPS,
	    1, SUB_APPS, 1, SUB_APPS, 0.001, 1, Slopes_Info->slopes->transform, 0.0);
  } else if ( Slopes_Type == 1 ) {
    cpgvect(Slopes_Info->slopes->average_x, Slopes_Info->slopes->average_y, SUB_APPS, SUB_APPS,
	    1, SUB_APPS, 1, SUB_APPS, 0.001, 1, Slopes_Info->slopes->transform, 0.0);
  } else {
    printf("  PGplot_SlopesUpdate: Unknow Slopes_Type %d\n", Slopes_Type);
  }

  /*
    Put overall tip/tilt arrow on the image
  */
  if ( Slopes_Arrow ) {
    cpgslw( 7);
    cpgsch( 3);
    cpgsci( 2);
    cpgarro(6.0, 6.0, 6.0 + Slopes_Info->slopes->tilt*0.05, 6.0 + Slopes_Info->slopes->tip*0.05);
    cpgsci( 1);
    cpgsch( 1.5);
    cpgslw( 1);
  }

  /*
    Update the display
  */
  cpgebuf();
  cpgupdt();

  return(0);
}
