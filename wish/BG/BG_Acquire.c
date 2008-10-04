/*
  BG_Acquire.c

  Routine to save streaming WFSC data to a FITS file, determine the
    mean and standard deviation of these data and write these "dark"
    values to file.

  Written  6feb07  DLM  Started with DClient_DM_Save.c

*/

#include "PCR_Wish.h"
#include "DClient.h"

#include <cfitsio/fitsio.h>

#define DEBUG 0

/*================================================================================
 * BG_Acquire: Save streaming data to FITS file
 *================================================================================*/
int BG_Acquire( DClient_Info *BG_Info, char *fname, int frames, int debug, char *errorMsg)
{
  int status;
  int i, j, num;
  int tries = 0;
  int max_tries = 100;
  int frame_counter = 0;
  int continueRunning = 1;

  /*
    Data variables
  */
  short *DM_Vector = NULL;
  double *avgFrame = NULL;
  double *sigFrame = NULL;
  double *quad = NULL;
  int *frameNumbers;
  int *numberPtr;
  int nBytes;
  int fd;
  FILE *fp;
  int type_hdu;

  int *nxtLong;
  short *nxtShort, *nxtArray;

  /* fitsio variables */

  fitsfile *fptr;
  char filename[STRING_LENGTH];
  int blockDimension[3];
  int pixelsPerImage;
  int num_axis = 3;
  int first_pixel = 1;
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

  status = 0;

  /*
    Loop to collect frames streaming from the WFC, create image frame, save in FITS file.
  */
  systime = (struct timeval *)malloc( sizeof(struct timeval) );
  status = gettimeofday(systime, NULL);
  start_sec = systime->tv_sec;
  start_usec = systime->tv_usec;

  /*
    Send request to server on the VME to send DM streaming data.
      These frames will be consecutive, at least until the circular
      buffer has time to catch up.
   */ 
  strcpy( BG_Info->server.out_line, "Stream\n");
  if ( debug ) {
    printf("  BG_Acquire: requested %s", BG_Info->server.out_line);
  }
  status = Server_Write( (Server_Info *)BG_Info);
  if ( status ) {
    strcpy( errorMsg, "BG_Acquire: Error in Server_Write sending Save request");
    printf("  %s\n", errorMsg);
    return(status);
  }

  strcpy( BG_Info->server.out_line, "WFSC\n");
  if ( debug ) {
    printf("                             %s", BG_Info->server.out_line);
  }
  status = Server_Write( (Server_Info *)BG_Info);
  if ( status ) {
    strcpy( errorMsg, "BG_Acquire: Error in Server_Write sending Save request");
    printf("  %s\n", errorMsg);
    return(status);
  }

  /*
    Setup to watch when there are data to be read on sockfd
  */
  FD_ZERO(&rfds);
  FD_SET(BG_Info->server.fd, &rfds);
  /* Wait up to 1 second. */
  tv.tv_sec = 10;
  tv.tv_usec = 0;

  /*
    Get the next frame and write to the fits file
  */
  status = select( (BG_Info->server.fd)+1, &rfds, NULL, NULL, &tv);

  if ( !status ) {

    strcpy( errorMsg, "BG_Acquire Tired of waiting for DServ to send data");
    strcat( errorMsg, "\n    => Is the WFSC running?");
    strcat( errorMsg, "\n    => Is the loop open?");
    if ( debug ) printf("  %s\n", errorMsg);
    return(-1);

  } else {

    status = Server_Read( (Server_Info *)BG_Info, sizeof(int));
    if ( status ) {
      strcpy( errorMsg, "BG_Acquire: Error in Server_Read getting nBytes");
      printf("  %s\n", errorMsg);
      return(status);
    }
    nBytes = ntohl( *(int *)BG_Info->server.in_line);
    if ( debug ) {
      printf("  BG_Acquire: Bytes in each frame %d\n", nBytes);
    }

  }

  /*
    Determine the size of the WFSC image
  */
  pixelsPerImage = nBytes/sizeof(short);

  if ( pixelsPerImage == 576 ) {
    blockDimension[0] = 24;
    blockDimension[1] = 24;
  } else if ( pixelsPerImage == 5184 ) {
    blockDimension[0] = 72;
    blockDimension[1] = 72;
  } else {
    sprintf( errorMsg, "BG_Acquire: Error receiving the number of pixels\n");
    sprintf( errorMsg, "    Should be 576 or 5184\n");
    sprintf( errorMsg, "    Received %d", pixelsPerImage);
    return(1);
  }

  blockDimension[2] = frames+2;

  /*
    Check to see if frame memory has already been allocated.  If so, free it
  */
  if ( DM_Vector != NULL) {
    if ( debug ) printf("  BG_Acquire: Freeing DM_Vector\n");
    free(DM_Vector);
  }

  if (( DM_Vector = (short *) malloc ( nBytes) ) == NULL) {
    sprintf( errorMsg, "BG_Acquire: Error allocating DM_Vector");
    printf("%s\n", errorMsg);
    return(-1);
  }
  memset( DM_Vector, 0, nBytes );

  /*
    Allocate (double *)avgFrame and (double *)sigFrame memory, and deallocate
      it when finished.  First check to see if they are already allocated.
  */

  if ( avgFrame != NULL) {
    if ( debug ) printf("  BG_Acquire: Freeing avgFrame\n");
    free(avgFrame);
  }

  avgFrame = (double *) malloc( sizeof(double)*pixelsPerImage );
  if ( avgFrame == NULL ) {
    sprintf( errorMsg, "BG_Acquire: Error allocating avgFrame");
    printf("%s\n", errorMsg);
    free(DM_Vector);
    return(1);
  }
  memset( avgFrame, 0, sizeof(double)*pixelsPerImage );

  if ( sigFrame != NULL) {
    if ( debug ) printf("  BG_Acquire: Freeing sigFrame\n");
    free(sigFrame);
  }

  sigFrame = (double *) malloc( sizeof(double)*pixelsPerImage );
  if ( sigFrame == NULL ) {
    sprintf( errorMsg, "BG_Acquire: Error allocating sigFrame");
    free(DM_Vector);
    free(avgFrame);
    return(1);
  }
  memset( sigFrame, 0, sizeof(double)*pixelsPerImage );

  /*
    Allocate memory to store frame numbers
  */

  frameNumbers = (int *) malloc( sizeof(int)*frames );
  if ( frameNumbers == NULL ) {
    printf("  BG_Acquire: Error allocating frameNumbers\n");
    free(DM_Vector);
    free(avgFrame);
    free(sigFrame);
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
    Open a fits file to save the data
  */
  fits_create_img( fptr, SHORT_IMG, num_axis, blockDimension, &istatus);
 
  /*
    Insert key words
  */
  fits_update_key( fptr, TSTRING, "OBS_DATE", time_string, "Observation Date", &istatus);

  /*
    The first image (size = pixelsPerImage) will be the mean image
      and the second will hold the standard deviations of the mean image.  Thus,
      must skip 2 x pixelsPerImage pixels before we start
      writing the frames.  We will write these first two images after we have aquired
      and written the num_frames below.
  */
  j = 1;
  fits_update_key( fptr, TSHORT, "FR_AVG", &j, "", &istatus);
  j = 2;
  fits_update_key( fptr, TSHORT, "FR_SIGMA", &j, "", &istatus);
  j = 3;
  fits_update_key( fptr, TSHORT, "FR_FRIST", &j, "", &istatus);
  j = frames;
  fits_update_key( fptr, TSHORT, "FR_NUM", &j, "", &istatus);

  /*
    Puts frames in fits file starting at frame 2.  Frame 0 is reserved for the average
      WFSC image and fame 1 is reserved for the sigma of the average WFSC image
  */
  first_pixel = 2 * pixelsPerImage + 1;

  /*
    Setup to watch when there are data to be read on sockfd
  */
  FD_ZERO(&rfds);
  FD_SET(BG_Info->server.fd, &rfds);
  /* Wait up to 1 second */
  tv.tv_sec = 10;
  tv.tv_usec = 0;

  frame_counter = 0;

  printf("  =>      ");

  while ( continueRunning ) {

    if ( nBytes > 0 ){

      //      printf( "number of bytes %d\n", nBytes);
      /*
	A frame is ready to be read, so:
	  - Read the frame number
	  - Read the frame data
	  - Save these data
          - Increment the frame counter
	  - Read the number of bytes for the next frame
      */

      /*
	We have a new frame, so we reset the (no new data) counter
      */
      tries = 0;

      /*
	Get the next frame and write to the fits file
      */
      num = Server_Read( (Server_Info *)BG_Info, sizeof(int));
      if ( status ) {
	strcpy( errorMsg, "BG_Acquire: Error in Server_Read getting frameNumber");
	printf("  %s\n", errorMsg);
	return(status);
      }
      *numberPtr++ = ntohl( *(int *)BG_Info->server.in_line);
      nxtLong = (int *)DM_Vector;
      *nxtLong = *(numberPtr - 1);
      //      printf("  BG_Acquire: Frame Number = %d\n", *(numberPtr - 1));

#if( DEBUG )
      printf("  BG_Acquire: Frame Number = %d\n", *(numberPtr - 1));
#endif

      num = Server_Read( (Server_Info *)BG_Info, nBytes);
      if ( status ) {
	strcpy( errorMsg, "BG_Acquire: Error in Server_Read getting frame");
	printf("  %s\n", errorMsg);
	return(status);
      }

      nxtArray = DM_Vector;
      nxtShort = (short *)BG_Info->server.in_line;
      for (j=0; j<nBytes; j+=2) {
	*nxtArray++ = ntohs(*nxtShort++);
      }

#if( DEBUG )
      printf("%d\n", *(numberPtr - 1));
      nxtShort = DM_Vector;
      datacount = 16;
      for ( j=0; j<datacount; j+=4) {
	printf(" %3d = ", j);
	for (k=0; k<4; k++) {
	  printf(" %6d", *nxtShort++);
	}
	printf("\n");
      }
#endif

      for ( i=0; i<pixelsPerImage; i++) {
	avgFrame[i] += (double) DM_Vector[i];
	sigFrame[i] += (double) DM_Vector[i] * (double) DM_Vector[i];
      }

      fits_write_img( fptr, TSHORT, first_pixel, pixelsPerImage, DM_Vector, &istatus);
      first_pixel += pixelsPerImage;

      /*
	Increment the frame counter
      */
      frame_counter++;

      if ( frame_counter%10 == 0) {
	printf("\b\b\b\b\b\b%6ld", frame_counter);
	fflush(stdout);
      }

      if ( frame_counter < frames ) {

	/*
	  Get the nBytes for the next frame
	*/
	status = select( (BG_Info->server.fd)+1, &rfds, NULL, NULL, &tv);
	if ( !status ) {
	  /*
	    DServ has not sent any data for too long so return from this routine
	  */
	  strcpy( errorMsg, "BG_Acquire: Tired of waiting for DServ to send data");
	  strcat( errorMsg, "\n    => Is the WFSC running?");
	  strcat( errorMsg, "\n    => Is the loop open?");
	  if ( debug ) printf("  %s\n", errorMsg);
	  status = -1;
	  nBytes = -2;
	  continueRunning = 0;
	  continue;
	}

	/*
	  nBytes is available, so read it
	*/
	status = Server_Read( (Server_Info *)BG_Info, sizeof(int));
	if ( status ) {
	  strcpy( errorMsg, "BG_Acquire: Error in Server_Read getting nBytes (1)");
	  printf("  %s\n", errorMsg);
	  continueRunning = 0;
	  continue;
	}
	nBytes = ntohl( *(int *)BG_Info->server.in_line);

      } else {

	/*
	  We have read enough frames so send the stop signal to DServ and clean up (outside
	    this while loop)
	*/
	continueRunning = 0;

      }

      /*
	Send the current frame number back to TclTk does not work.  Need TclTk
          to be threaded.  The next project....  19spe04
	  DClient_Current_Frame = i+1;
      */

    } else if ( nBytes == -2 ){

      /*
	Get the nBytes for the next frame
      */
      status = select( (BG_Info->server.fd)+1, &rfds, NULL, NULL, &tv);

      if ( !status ) {

	/*
	  DServ has not sent any data for too long so return from this routine
	*/
	strcpy( errorMsg, "BG_Acquire: Tired of waiting for DServ to send data");
	strcat( errorMsg, "\n    => Is the WFSC running?");
	strcat( errorMsg, "\n    => Is the loop open?");
	if ( debug ) printf("  %s\n", errorMsg);
	status = -1;
	continueRunning = 0;
	break;

      }

      /*
	nBytes is available, so read it
      */
      status = Server_Read( (Server_Info *)BG_Info, sizeof(int));
      if ( status ) {
	strcpy( errorMsg, "BG_Acquire: Error in Server_Read getting nBytes (2)");
	printf("  %s\n", errorMsg);
	continueRunning = 0;
	break;
      }
      nBytes = ntohl( *(int *)BG_Info->server.in_line);

      /*
	Increment the number of times we have read a -2 (new data not available)
	  from the DM Data Server
      */
      tries++;

      if ( tries > max_tries ) {

	printf("  BG_Acquire: Requesting Status\n");
	/*
	  Send a status request to DServ so it will send a flag (nBytes)
	    is important because if the server is writing to this port when we close
	    it on this end we get a broke pipe error on the DServ side and DServ
	    dies a terrible death.
	*/ 
	strcpy( BG_Info->server.out_line, "Status\n");
	if ( debug ) {
	  printf("  BG_Acquire: requested %s", BG_Info->server.out_line);
	}
	status = Server_Write( (Server_Info *)BG_Info);
	if ( status ) {
	  strcpy( errorMsg, "BG_Acquire: Error in Server_Write sending Save request");
	  printf("  %s\n", errorMsg);
	  return(status);
	}

	tries = 0;

      }

    } else {

      printf("\n");
      sprintf( errorMsg, "BG_Acquire: Error reading the correct number of bytes %d", nBytes);
      if ( debug ) printf("  %s\n", errorMsg);
      continueRunning = 0;

    }

  }

  /*
    Calculate time spent retrieving data
  */
  status = gettimeofday(systime, NULL);
  end_sec = systime->tv_sec;
  end_usec = systime->tv_usec;
  total_time = (float) (end_sec-start_sec) + (float) (end_usec-start_usec) /1.0e6 ;

  /*
    Send a stop request to DServ so it can properly shut itself down.  This
      is important because if the server is writing to this port when we close
      it on this end we get a broke pipe error on the DServ side and DServ
      dies a terrible death.
   */ 
  strcpy( BG_Info->server.out_line, "Stop\n");
  if ( debug ) {
    printf("  BG_Acquire: requested %s", BG_Info->server.out_line);
  }
  status = Server_Write( (Server_Info *)BG_Info);
  if ( status ) {
    strcpy( errorMsg, "BG_Acquire: Error in Server_Write sending Save request");
    printf("  %s\n", errorMsg);
    return(status);
  }

  /*
    Flush the port so we know when DServ has properly closed the socket and we can
      close the socket on this side.
  */
  status = Server_Flush( (Server_Info *)BG_Info);
  if ( status ) {
    strcpy( errorMsg, "BG_Acquire: Error in Server_Flush");
    printf("  %s\n", errorMsg);
    return(status);
  }

  printf(" frames saved to %s in %f seconds (%3.0f Hz)\n", filename, total_time,
	 (float) frame_counter/total_time);

  /*
    Find average for each pixel and its standard deviation.  The background which wil
      be use in the SSC is the average background - 3*(standard deviation).  This will
      make 98% of our corrected wavefront image pixel values will be positive.  The
      correct method to guaranttee this is to check for negative values in the SSC
      and set any negative values to 0.  12sep01  DLM
  */

  for ( i=0; i<pixelsPerImage; i++) {
    avgFrame[i] /= (double) frames;
    sigFrame[i] = sqrt( (sigFrame[i] - (double)frames*avgFrame[i]*avgFrame[i])
      /(double)(frames-1));
    //    avgFrame[i] -= 3.0*sigFrame[i];
  }

  /*
    Save the average background frame as the first image in the
      data cube.
  */

  for ( i=0; i<pixelsPerImage; i++) {
    DM_Vector[i] = (short) avgFrame[i];
  }

  j = 0;
  fits_movabs_hdu( fptr, 1, &type_hdu, &istatus);
  first_pixel = 1;
  fits_write_img( fptr, TSHORT, first_pixel, pixelsPerImage, DM_Vector, &istatus);

  /*
    Save the standard deviation of the background frame as the second image in
      the data cube.
  */

  for ( i=0; i<pixelsPerImage; i++) {
    DM_Vector[i] = (short) sigFrame[i];
  }

  first_pixel += pixelsPerImage;
  fits_write_img( fptr, TSHORT, first_pixel, pixelsPerImage, DM_Vector, &istatus);
  fits_close_file( fptr, &istatus);
  fits_report_error( stderr, istatus);

  /*
    Write the average frame to an ASCII file for Don to load into
      the SSC.  The frame must be re-arranged into four separate
      quadrants for Don to load into four separate C40 SSC processors,
      instead of the image format returned by Char_to_spot_array_2 used
      for display.  17May02 DLM
  */

  strcpy(filename, fname);
  //  if( (ptr = strstr(filename, ".fits")) != NULL ) *ptr = '\0';
  //  if( (ptr = strstr(filename, ".FITS")) != NULL ) *ptr = '\0';
  strcat(filename, ".dat");

  fp = fopen(filename, "w");
  if( fd < 0 ) {
    sprintf( errorMsg, "BG_Acquire Error opening %s for ASCII data", filename);
    free(DM_Vector);
    free(avgFrame);
    free(sigFrame);
    return(1);
  };

  /*
    Comment lines start with # and the number of pixel written
  */
  //  fprintf(fp, "#  Background file\n");
  //  fprintf(fp, "%d  Number of pixels\n", pixelsPerImage);

  /*
    This is the full image order needed by IDL for background
      removal for display
  */
  quad = avgFrame;
  for( i=0 ; i<pixelsPerImage ; i++ ) {
    fprintf(fp, " %6d\n", (short)(*(quad++)+0.5));
  }

  fclose(fp);

  status = DClient_SaveFrameNumbers( frames, frameNumbers, fname, time_string);

  free(DM_Vector);
  free(frameNumbers);

  return(status);
}
