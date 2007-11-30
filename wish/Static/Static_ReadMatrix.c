/*
  =========================
  Static_ReadMatrix()
  =========================

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

  This routine reads in the slope offset matrix and stores it into
    a static variable.

  **Note: this matrix depends on the reconstructor (RTR) used in
    the realtime loop.  Thus, this matrix must be reread whenever
    a new reconstructor is loaded.

  Written 27nov06  DLM  Started with AbRm_SlopeOffsetMatRead.c

*/

#include "PCR_Wish.h"

#include "Client.h"
#include "Server.h"
#include "Static.h"

/*================================================================================*
 * Read slope offset matrix (7x288, modes-slopes)
 *================================================================================*/
int Static_ReadMatrix( float *soff_matrix, char *filename, int debug, char *replyData)
{
  long status;
  char ascii[120];
  int i;
  int num_slopes, num_modes;
  float mode_scale_factor;
  float *data_ptr;
  float flt0, flt1, flt2, flt3, flt4, flt5, flt6;
  FILE *fptr;

  if((fptr = fopen(filename, "r"))){

    if(!fgets(ascii, 120, fptr)){		    /* read line, up to 120 characters. */
      return 1;
    }

    /* Read any number of comment lines */
    while(ascii[0] == '#'){
      if(!fgets(ascii, 120, fptr)){
	return 2;
      }
    }

    /* Read two integer (number of slope, number of modes) and a float (Scale factor) */

    if(sscanf(ascii, "%d", &num_slopes) != 1){
      return 5;
    }
    if ( debug ) {
      printf("\n Static_ReadMatrix\n");
      printf("---------------------------\n");
      printf(" num_slopes = %d\n", num_slopes);
    }

    if(!fgets(ascii, 120, fptr)){		    /* read line, up to 120 characters. */
      return 2;
    }
    if(sscanf(ascii, "%d", &num_modes) != 1){
      return 5;
    }
    if ( debug ) printf(" num_modes = %d\n", num_modes);

    if(!fgets(ascii, 120, fptr)){		    /* read line, up to 120 characters. */
      return 2;
    }
    if(sscanf(ascii, "%f", &mode_scale_factor) != 1){
      return 5;
    }
    if ( debug ) printf(" Scale_factor in file = %e\n", mode_scale_factor);
    /*
      Scale factor converts from radians to meters in terms of the redius of the telescope pupil.
        The MMT is diameter is 6.5m , but the pupil is 6.4 m.  The slopes need to be in nanometers,
	thus the factor of 1.0e-9

      Added another factor of 1.0e-9 (1.0e-18 total) for the PCR.  Need to check with Guido
        and Vidhya that this is the correct scaling.  12jan07  DLM
    */
    mode_scale_factor *= 1.0e-18;
    if ( debug ) {
      printf(" mode_scale_factor %e\n",mode_scale_factor );
    }

    data_ptr = soff_matrix;

    for(i = 0; i < num_slopes; i++){

      if(!fgets(ascii, 120, fptr)){		    /* read line, up to 120 characters. */
	return 1;
      }
 
      if(sscanf(ascii, "%f %f %f %f %f %f %f", &flt0, &flt1, &flt2, &flt3, &flt4, &flt5, &flt6) != 7){
	printf("  Error reading Slope Offset Matrix\n  Last line read = %s\n", ascii);
	return 5;
      }
      *data_ptr++ = flt0*mode_scale_factor;
      *data_ptr++ = flt1*mode_scale_factor;
      *data_ptr++ = flt2*mode_scale_factor;
      *data_ptr++ = flt3*mode_scale_factor;
      *data_ptr++ = flt4*mode_scale_factor;
      *data_ptr++ = flt5*mode_scale_factor;
      *data_ptr++ = flt6*mode_scale_factor;
	
    }

  }

  if ( debug ) {
    printf(" Should be 0.0 and first three non-zero values in matrix\n");
    data_ptr = soff_matrix+5*num_modes-1;
    printf(" %f\n", (*data_ptr)/mode_scale_factor);
    data_ptr++;
    printf(" %f\n", (*data_ptr)/mode_scale_factor);
    data_ptr++;
    printf(" %f\n", (*data_ptr)/mode_scale_factor);
    data_ptr++;
    printf(" %f\n\n", (*data_ptr)/mode_scale_factor);
    data_ptr++;
  }

  fclose(fptr);
  status = 0;

  return(status);
}

