/*
  ======================
  Mod_Calc()
  ======================

  Determine a set of Slope Offsets with the requested amount
    of the 7 modes available:
       astig_0
       astig_45
       coma_x
       coma_y
       trefoil_0
       trefoil_30
       spherical

  The total amount of slope offset is determined by multiplying
    the input vector by matrix (provided by Guido Brusa) containing
    a column of slope offsets for 1 micron rms for each of the
    7 modes.  Thus the matrix is 288x7.

  Written 27nov06  DLM  Started with AbRm_SlopeOffsetCalc.c

*/

#include "PCR_Wish.h"

#include "Mod.h"

#define DEBUG 0

/*================================================================================*
 * Calculate a slope offset vector (144 X's & 144 Y's) with the requested almount
 *   of each of seven modes.
 *================================================================================*/
int Mod_Calc( int mode, float *matrix, float *offsets,  int debug, char *replyData)
{
  int i, j;
  float *fltptr;

  fltptr = matrix;
  fltptr += mode;

  for ( i=0; i<NUM_SLOPES; i++) {
    offsets[i] = *fltptr;
    fltptr += NUM_STATIC_ZERN;
  }

  if ( DEBUG ) {
    printf("\n Mod_Calc\n");
    printf("---------------------------\n");
    printf("    X-Slopes\n");
    printf("    --------\n");
    for ( i=0; i<12; i++) {
      for ( j=0; j<12; j++) {
        printf("%8.2f", offsets[i*12+j]);
      }
      printf("\n");
    }
    printf("\n    Y-Slopes\n");
    printf(  "    --------\n");
    for ( i=0; i<12; i++) {
      for ( j=0; j<12; j++) {
        printf("%8.2f", offsets[144+i*12+j]);
      }
      printf("\n");
    }
  }

  return(0);
}
