/*
  PGplot_SpotsUpdate.c

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

int PGplot_SpotsUpdate( PGplot_Info *Spots_Info, int Spots_Type, int Spots_Slopes, int Spots_Arrow, int debug)
{

  float tip, tilt, sum;

  /*
    Make sure that a device has already been opened (PGplot_Open)
  */
  if ( Spots_Info->id <= 0) {
    printf("  PGplot_SpotsUpdate: Device not opened yet\n");
    return(-1);
  }
  cpgslct(Spots_Info->id);

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
  // Create the plot area and put the color wedge next to it
  //========================================================
  /*
    Carve out a plot area a bit bigger than the image so the
      color wedge is the correct size.  The color wedge, via
      cpgwedg(), is drawn to the left of this plot area.
  */
  cpgsvp(0.07, 0.87, 0.085, 0.985);

  /*
    Set the scale for color wedge text
  */
  cpgsch(2.5);

  /*
    Choose white for quad cell divisions and labels
  */
  cpgsci( 1);

  /*
    Create color scale wedge and position it (default to the current image)
  */
  if ( Spots_Type == 0 ) {
    cpgwedg( "RI", 0.5, 2.0, Spots_Info->spots->min_lower, Spots_Info->spots->max_upper, " ");
  } else if ( Spots_Type == 1 ) {
    cpgwedg( "RI", 0.5, 2.0, Spots_Info->spots->min_lower, Spots_Info->spots->max_upper, " ");
  } else if ( Spots_Type == 2 ) {
    cpgwedg( "RI", 0.5, 2.0, Spots_Info->spots->min_pupil_lower, Spots_Info->spots->max_pupil_upper, " ");
  } else {
    cpgwedg( "RI", 0.5, 2.0, Spots_Info->spots->min_lower, Spots_Info->spots->max_upper, " ");
  }
  //========================================================
  // Setup and create Spots image
  //========================================================
  /*
    Carve out a plot area in the upper left for the WFSC image.
      Be sure to make the x and y sizes are the same.
  */
  cpgsvp(0.07, 0.87, 0.07, 1.0);

  /*
    Set axis length in units of sub-apperatures
  */
  cpgwnad(0.0, (float) SUB_APPS,
 	  0.0, (float) SUB_APPS);

  /*
    Display the color image according to the user request (default to the current image)
  */
  if ( Spots_Type == 0 ) {
    cpgimag(Spots_Info->spots->current, Spots_Info->spots->values, Spots_Info->spots->values,
	    1, Spots_Info->spots->values, 1, Spots_Info->spots->values, Spots_Info->spots->min_lower,
	    Spots_Info->spots->max_upper, Spots_Info->spots->image_transform);
  } else if ( Spots_Type == 1 ) {
    cpgimag(Spots_Info->spots->average, Spots_Info->spots->values, Spots_Info->spots->values,
	    1, Spots_Info->spots->values, 1, Spots_Info->spots->values, Spots_Info->spots->min_lower,
	    Spots_Info->spots->max_upper, Spots_Info->spots->image_transform);
  } else if ( Spots_Type == 2 ) {
    cpgimag(Spots_Info->spots->pupil, SUB_APPS, SUB_APPS,
	    1, SUB_APPS, 1, SUB_APPS, Spots_Info->spots->min_pupil_lower,
	    Spots_Info->spots->max_pupil_upper, Spots_Info->spots->pupil_transform);
  } else {
    cpgimag(Spots_Info->spots->current, Spots_Info->spots->values, Spots_Info->spots->values,
	    1, Spots_Info->spots->values, 1, Spots_Info->spots->values, Spots_Info->spots->min_lower,
	    Spots_Info->spots->max_upper, Spots_Info->spots->image_transform);
  }

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

  /*
    Choose black for quad cell divisions
  */
  cpgsci( 0);

  /*
    Draw the axis with tick marks and labels
  */
  cpgbox( "G", 1, 0, "G", 1, 0);

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
  if ( Spots_Type != 2 ){
    if ( Spots_Slopes == 1 ) {
      cpgvect(Spots_Info->spots->x_slopes, Spots_Info->spots->y_slopes, SUB_APPS, SUB_APPS,
	      1, SUB_APPS, 1, SUB_APPS, 2.0, 1, Spots_Info->spots->pupil_transform, 0.0);
    } else if ( Spots_Slopes == 2 ) {
      if ( Spots_Type != 2 ){
	cpgvect(Spots_Info->spots->x_focus, Spots_Info->spots->y_focus, SUB_APPS, SUB_APPS,
		1, SUB_APPS, 1, SUB_APPS, 2.0, 1, Spots_Info->spots->pupil_transform, 0.0);
	printf("focus = %f\n", Spots_Info->spots->focus);
      }
    }
  }

  /*
    Put overall tip/tilt arrow on the image
  */
  if ( Spots_Type == 2 ){
    cpgslw( 7);
    cpgsch( 3);
    cpgsci( 3);
    sum = Spots_Info->spots->pupil[65] + Spots_Info->spots->pupil[66]
      + Spots_Info->spots->pupil[77] + Spots_Info->spots->pupil[78];
    tilt = -1.0*((Spots_Info->spots->pupil[66] + Spots_Info->spots->pupil[78])
	    - (Spots_Info->spots->pupil[65] + Spots_Info->spots->pupil[77]))/sum;
    tip = -1.0*((Spots_Info->spots->pupil[77] + Spots_Info->spots->pupil[78])
	   - (Spots_Info->spots->pupil[65] + Spots_Info->spots->pupil[66]))/sum;
    cpgarro(6.0, 6.0, 6.0 + tilt*12.0, 6.0 + tip*12.0);
    cpgsci( 1);
    cpgsch( 1.5);
    cpgslw( 1);
  } else {
    if ( Spots_Arrow ) {
      if ( Spots_Type != 2 ){
	cpgslw( 7);
	cpgsch( 3);
	cpgsci( 3);
	cpgarro(6.0, 6.0, 6.0 + Spots_Info->spots->tilt*6.0, 6.0 + Spots_Info->spots->tip*6.0);
	cpgsci( 1);
	cpgsch( 1.5);
	cpgslw( 1);
      }
    }
  }

  /*
    Update the display
  */
  cpgebuf();
  cpgupdt();

  return(0);
}
