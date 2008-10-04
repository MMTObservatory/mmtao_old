/*
  DClient_Slopes_Save.c

  Routine to save streaming Slopes data from the WFC to a FITS file

  Written  19spe04  DLM

*/

#include "PCR_Wish.h"
#include "DClient.h"

#include <cfitsio/fitsio.h>

#define DEBUG 0

/*================================================================================
 * DClient_Slopes_Save: Save streaming data to FITS file
 *================================================================================*/
int DClient_Slopes_Save( DClient_Info *Slopes_Save_Info, char *fname, int frames, 
		       short *Slopes_Array, int debug, char *errorMsg)
{
  int status;
  int i, j, num;

  /*
    Spot variables
  */

  int *frameNumbers;
  int *numberPtr;
  int nBytes;

  short *nxtShort, *nxtArray;

  /* fitsio variables */

  fitsfile *fptr;
  char filename[STRING_LENGTH];
  int blockDimension[2];
  int slopesPerImage;
  int num_axis = 2;
  int first_slope = 1;
  int istatus;

  /* Time variables */

  time_t *tp;
  char *time_string;

  /* Timing variables */

  fd_set rfds;
  struct timeval tv;

  struct timeval *systime;
  int start_sec, start_usec;
  int end_sec, end_usec;
  float total_time;

#if( DEBUG )
  int datacount;
  int k;
#endif

  /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

  /*
    Allocate memory to store frame numbers
  */

  frameNumbers = (int *) malloc( sizeof(int)*frames );
  if ( frameNumbers == NULL )
    {
      printf("  DClient_Slopes_Save: Error allocating frameNumbers\n");
      return(1);
    }
  memset( frameNumbers, 0, sizeof(int)*frames );
  numberPtr = frameNumbers;

  /* Open FITS file */

  strcpy(filename,"!");
  strcat(filename,fname);
  strcat(filename,".fits");
  istatus = 0;
  fits_create_file( &fptr, filename, &istatus);

  /* IDL time...I don't like the format DLM
     fits_get_system_time(time_string, &num_hdu, &istatus);
  */

  /* C time...Much better format  DLM */
  tp = (time_t *)malloc( sizeof(time_t) );
  time( (time_t *)tp );
  time_string = ctime( tp);
  time_string[strlen(time_string)-1] = '\0';

  /*
    Slopes frames are either 24x24 or 72x72.  Note: our frame
      has the frame number (int) at the beginning of the frame,
      thus our frame is 24x24+2 or 72x72+2.
  */

  slopesPerImage = Slopes_Save_Info->server.nBytes/2;

  blockDimension[0] = slopesPerImage;
  blockDimension[1] = frames;

  fits_create_img( fptr, SHORT_IMG, num_axis, blockDimension, &istatus);
 
  /*
    Insert key words
  */

  fits_update_key( fptr, TSTRING, "OBS_DATE", time_string, "Observation Date", &istatus);
  i = frames;
  fits_update_key( fptr, TSHORT, "FRAMES", &i, "", &istatus);

  /*
    Loop to collect frames streaming from the WFC, create image frame, save in FITS file.
  */

  systime = (struct timeval *)malloc( sizeof(struct timeval) );
  status = gettimeofday(systime, NULL);
  start_sec = systime->tv_sec;
  start_usec = systime->tv_usec;

  strcpy( Slopes_Save_Info->server.out_line, "Save\n");

  status = Server_Write( (Server_Info *)Slopes_Save_Info);
  if ( status ) {
    strcpy( errorMsg, "DClient_Slopes_Save: Error in Server_Write sending Save request");
    printf("%s\n", errorMsg);
    return(status);
  }

  /*
    Setup to watch when there are data to be read on sockfd
  */
  FD_ZERO(&rfds);
  FD_SET(Slopes_Save_Info->server.fd, &rfds);
  /* Wait up to one seconds. */
  tv.tv_sec = 10;
  tv.tv_usec = 0;

  /*
    Send request to server on the VME to send Slopes streaming data.
      These frames will be consecutive, at least until the circular
      buffer has time to catch up.

    The server does not pay attention to when data has been read
      from its socket. Thus, the only way to quit reading data is to
      close the socket on the client side (here).  The server sees
      that the connection has been lost and properly exits.
   */ 
  strcpy( Slopes_Save_Info->server.out_line, "Stream\n");
  status = Server_Write( (Server_Info *)Slopes_Save_Info);
  if ( status ) {
    strcpy( errorMsg, "DClient_Slopes_Save: Error in Server_Write sending Save request");
    printf("%s\n", errorMsg);
    return(status);
  }

  printf("Save Status: ");

  for ( i=0; i<frames; i++) {

    /*
      Get the next frame and write to the fits file
    */
    tv.tv_sec = 10;
    status = select( (Slopes_Save_Info->server.fd)+1, &rfds, NULL, NULL, &tv);

    if ( !status ) {
      printf("  DClient_Slopes_Save: Tired of waiting for DServ to send data\n");
      fits_close_file( fptr, &istatus);
      sprintf( filename, "rm -f %s.fits", fname);
      system(filename);
      return(-1);
    }

    status = Server_Read( (Server_Info *)Slopes_Save_Info, sizeof(int));
    if ( status ) {
      strcpy( errorMsg, "DClient_Slopes_Save: Error in Server_Read getting nBytes");
      printf("%s\n", errorMsg);
      fits_close_file( fptr, &istatus);
      sprintf( filename, "rm -f %s.fits", fname);
      system(filename);
      return(status);
    }
    nBytes = ntohl( *(int *)Slopes_Save_Info->server.in_line);

#if( DEBUG )
    printf("  DClient_Slopes_Save: Byte Count = %d\n", nBytes);
#endif

    if ( nBytes == Slopes_Save_Info->server.nBytes ){

      num = Server_Read( (Server_Info *)Slopes_Save_Info, sizeof(int));
      if ( status ) {
	strcpy( errorMsg, "DClient_Slopes_Save: Error in Server_Read getting frameNumber");
	printf("%s\n", errorMsg);
	fits_close_file( fptr, &istatus);
	sprintf( filename, "rm -f %s.fits", fname);
	system(filename);
	return(status);
      }
      *numberPtr++ = ntohl( *(int *)Slopes_Save_Info->server.in_line);

#if( DEBUG )
      printf(" DClient_Slopes_Save: Frame Number = %d\n", *(numberPtr - 1));
#endif

      num = Server_Read( (Server_Info *)Slopes_Save_Info, Slopes_Save_Info->server.nBytes);
      if ( status ) {
	strcpy( errorMsg, "DClient_Slopes_Save: Error in Server_Read getting frame");
	printf("%s\n", errorMsg);
	fits_close_file( fptr, &istatus);
	sprintf( filename, "rm -f %s.fits", fname);
	system(filename);
	return(status);
      }

      nxtArray = Slopes_Array;
      nxtShort = (short *)Slopes_Save_Info->server.in_line;
      for (j=0; j<nBytes; j+=2) {
	*nxtArray++ = ntohs(*nxtShort++);
      }

    } else if ( nBytes < 0 ) {
      /*
	Return (-1) indicates an error
      */
      if ( nBytes == -1 ) {
	strcpy( errorMsg, "DClient_Slopes_Save: Error in server: closing connection");
	if ( debug ) printf("%s\n", errorMsg);
	sprintf( errorMsg, "DClient_Slopes_Save: Unkown error code %d", nBytes);
	if ( debug ) printf("%s\n", errorMsg);
      }
      fits_close_file( fptr, &istatus);
      sprintf( filename, "rm -f %s.fits", fname);
      system(filename);
      return(nBytes);

    } else {

      strcpy( errorMsg, "DClient_Slopes_Save: Error reading the correct number of bytes");
      printf("%s\n", errorMsg);
      fits_close_file( fptr, &istatus);
      sprintf( filename, "rm -f %s.fits", fname);
      system(filename);
      return(-1);
    }


#if( DEBUG )
    printf("%d\n", *(numberPtr - 1));
    nxtShort = Slopes_Array;
    datacount = 16;
    for ( j=0; j<datacount; j+=4) {
      printf(" %3d = ", j);
      for (k=0; k<4; k++) {
	printf(" %6d", *nxtShort++);
      }
      printf("\n");
    }
#endif

    fits_write_img( fptr, TSHORT, first_slope, slopesPerImage, Slopes_Array, &istatus);
    first_slope += slopesPerImage;

    if ( i%250 == 0) {
      printf("%5ld\b\b\b\b\b", i);
      fflush(stdout);
    }

    /*
      Send the current frame number back to TclTk does not work.  Need TclTk
        to be threaded.  The next project....  19spe04
    DClient_Current_Frame = i+1;
    */

  }

  status = gettimeofday(systime, NULL);
  end_sec = systime->tv_sec;
  end_usec = systime->tv_usec;
  total_time = (float) (end_sec-start_sec) + (float) (end_usec-start_usec) /1.0e6 ;

  fits_close_file( fptr, &istatus);
  fits_report_error( stderr, istatus);

  printf("\n   %d frames saved to %s in %f seconds (%3.0f Hz)\n", frames, filename, total_time, (float) frames/total_time);

  status = DClient_SaveFrameNumbers( frames, frameNumbers, fname, time_string);

  free(frameNumbers);

  return(0);
}
