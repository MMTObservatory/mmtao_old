/*
  ==========================
  DClient_SaveFrameNumbers()
  ==========================

  Created 15jan04  DLM

  Altered 28mar06  DLM  Copied DDS_SaveFrameNumbers.c
*/

#include "PCR_Wish.h"

#include <cfitsio/fitsio.h>

/*================================================================================
 * DClient_SaveFrameNumbers: Save the frame tags from the DDS
 *================================================================================*/
int DClient_SaveFrameNumbers( int framesReceived, int *frameNumbers, char *fname, char *time_string)
{

  char filename[STRING_LENGTH];

  /* fitsio variables */

  fitsfile *fptr;
  long num_axis = 3;
  int first_pixel = 1;
  int istatus;
  long dataDimension[1];

  /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

  strcpy(filename,"!");
  strcat(filename,fname);
  strcat(filename,"_num.fits");
  istatus = 0;
  fits_create_file( &fptr, filename, &istatus);

  num_axis = 1;
  dataDimension[0] = framesReceived;
  first_pixel = 1;

  fits_create_img( fptr, LONG_IMG, num_axis, dataDimension, &istatus);
  fits_update_key( fptr, TSTRING, "OBS_DATE", time_string, "Observation Date", &istatus);
  fits_write_img( fptr, TLONG, first_pixel, framesReceived, frameNumbers, &istatus);

  fits_close_file( fptr, &istatus);
  fits_report_error( stderr, istatus);

  return(0);
}
