/*
  PGplot_ColorTable.c

  This routine loads the requested color table

  Written  29may07  DLM  Started with Matt's dougbar() routine

*/

#include "PCR_Wish.h"

#include "PGplot.h"

int PGplot_ColorTable( PGplot_Info *Spots_Info, int color, int debug)
{

    int num_colors;
    int cir_low, cir_high;
    float i;
    float *l, *r, *g, *b;
    int counter;

/*
 * Iraf "heat" color table.
 */
static float heat_l[] = {0.0, 0.2, 0.4, 0.6, 1.0};
static float heat_r[] = {0.0, 0.5, 1.0, 1.0, 1.0};
static float heat_g[] = {0.0, 0.0, 0.2, 0.6, 1.0};
static float heat_b[] = {0.0, 0.0, 0.0, 0.2, 1.0};
/*
static float heat_l[] = {0.0, 0.2, 0.4, 0.6, 1.0};
static float heat_r[] = {0.0, 0.5, 1.0, 1.0, 1.0};
static float heat_g[] = {0.0, 0.0, 0.5, 1.0, 1.0};
static float heat_b[] = {0.0, 0.0, 0.0, 0.3, 1.0};
*/
  /*
    Make sure that a device has already been opened (PGplot_Open)
  */
  if ( Spots_Info->id <= 0) {
    printf("  PGplot_ColorTable: Device not opened yet\n");
    return(-1);
  }

  /*
    Select the PGPLOT device and draw the plot.
  */
  cpgslct( Spots_Info->id);

  /*
    Query pgplot as to the number of color indices available
  */
  cpgqcir(&cir_low, &cir_high);
  num_colors = cir_high - cir_low + 1;

  /*
    Allocate the need memory for the color table
  */
  l = (float *)malloc( num_colors*sizeof(float));
  r = (float *)malloc( num_colors*sizeof(float));
  g = (float *)malloc( num_colors*sizeof(float));
  b = (float *)malloc( num_colors*sizeof(float));

  /*
    Loop through the colour index list
      i goes from 0 to 1.0 linearly for all colour indexes
      l,r,g,b are then calculated as a function of i, where l
        is the intensity
  */
  for (counter = cir_low; counter <= cir_high; counter++) {
    i = (float) (counter - cir_low) / (num_colors - 1);

    l[counter - cir_low] = i;
    r[counter - cir_low] = i;
    g[counter - cir_low] = pow(i, 4);
    b[counter - cir_low] = pow(i, 8);

  }

  /*
    Load the "Heat" color map into the color table used
      by cpgimag()
  */
  //cpgctab(l, r, g, b, 112, 1.0, 0.5);
  cpgctab(heat_l, heat_r, heat_g, heat_b, 5, 1.0, 0.5);

  /*
    Free up memory used
  */
  free(l);
  free(r);
  free(g);
  free(b);

  return(0);
}
