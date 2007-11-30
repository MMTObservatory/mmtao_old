/*
  DClient_DM_Save.c

  Routine to save streaming DM data from the WFC to a FITS file

  Written  19spe04  DLM

*/

#include "PCR_Wish.h"
#include "DClient.h"

#include <fitsio.h>

#define DEBUG 0

/*================================================================================
 * DClient_DM_Save: Save streaming data to FITS file
 *================================================================================*/
int DClient_DM_Save( DClient_Info *DM_Save_Info, char *request, char *dirname, char *fname, long frames,
		     int debug, char *errorMsg)
{
  int status;
  long i, j, num;
  int tries = 0;
  int max_tries = 100;
  long frame_counter = 0;
  int continueRunning = 1;
  short key;

  /*
    Data variables
  */

  short *DM_Vector = NULL;
  long *frameNumbers;
  long *numberPtr;
  long nBytes;

  long *nxtLong;
  short *nxtShort, *nxtArray;

  /* fitsio variables */
  char filename[STRING_LENGTH];
  long dimension[3];
  long num_axis;
  long offset, totalBytes;
  int istatus;

  /* Added arrays so multiple files could be saved */
  int sets;
  fitsfile *fptr[6];
  long values[6];
  long first_pixel[6];
  short *Data_Ptr[6];

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
  long datacount;
  int k;
#endif

  debug = 1;

  /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

  /*
    Check to see if memory has already been allocated.  If so, free it
  */
  if ( DM_Vector != NULL) {
    if ( debug ) printf("  DClient_DM_Save: Freeing %s Array\n", DM_Save_Info->server.type);
    free(DM_Vector);
  }

  if (( DM_Vector = (short *) malloc (DM_Save_Info->server.nBytes+4) ) == NULL) {
    sprintf( errorMsg, "DClient_DM_Save: Error allocating %s Array", DM_Save_Info->server.type);
    printf("%s\n", errorMsg);
    return(-1);
  }
  memset( DM_Vector, 0, DM_Save_Info->server.nBytes+4 );

  /*
    Allocate memory to store frame numbers
  */

  frameNumbers = (long *) malloc( sizeof(long)*frames );
  if ( frameNumbers == NULL )
    {
      printf("  DClient_DM_Save: Error allocating frameNumbers\n");
      return(1);
    }
  memset( frameNumbers, 0, sizeof(long)*frames );
  numberPtr = frameNumbers;

  /*
    Send request to server on the VME to send DM streaming data.
      These frames will be consecutive, at least until the circular
      buffer has time to catch up.
   */ 
  strcpy( DM_Save_Info->server.out_line, "Stream\n");
  if ( debug ) {
    printf("  DClient_DM_Save: requested %s", DM_Save_Info->server.out_line);
  }
  status = Server_Write( (Server_Info *)DM_Save_Info);
  if ( status ) {
    strcpy( errorMsg, "DClient_DM_Save: Error in Server_Write sending Save request");
    printf("  %s\n", errorMsg);
    return(status);
  }

  strcpy( DM_Save_Info->server.out_line, request);
  strcat( DM_Save_Info->server.out_line, "\n");
  if ( debug ) {
    printf("                             %s", DM_Save_Info->server.out_line);
  }
  status = Server_Write( (Server_Info *)DM_Save_Info);
  if ( status ) {
    strcpy( errorMsg, "DClient_DM_Save: Error in Server_Write sending Save request");
    printf("  %s\n", errorMsg);
    return(status);
  }

  /*
    Setup to watch when there are data to be read on sockfd
  */
  FD_ZERO(&rfds);
  FD_SET(DM_Save_Info->server.fd, &rfds);
  /* Wait up to 10 seconds */
  tv.tv_sec = 10;
  tv.tv_usec = 0;

  /*
    Get the next frame and write to the fits file
  */
  status = select( (DM_Save_Info->server.fd)+1, &rfds, NULL, NULL, &tv);

  if ( !status ) {

    printf("  DClient_DM_Save: First: Tired of waiting for DServ to send data\n");
    continueRunning = 0;

  } else {

    status = Server_Read( (Server_Info *)DM_Save_Info, sizeof(long));
    if ( status ) {
      strcpy( errorMsg, "DClient_DM_Save: Error in Server_Read getting nBytes");
      printf("  %s\n", errorMsg);
      return(status);
    }
    nBytes = ntohl( *(long *)DM_Save_Info->server.in_line);
    if ( debug ) {
      printf("  DClient_DM_Save: Bytes in each frame %ld\n", nBytes);
    }

  }

  /* IDL time...I don't like the format DLM
     fits_get_system_time(time_string, &num_hdu, &istatus);
  */

  /* C time...Much better format  DLM */
  tp = (time_t *)malloc( sizeof(time_t) );
  time( (time_t *)tp );
  time_string = ctime( tp);
  time_string[strlen(time_string)-1] = '\0';

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  status = Server_StringLowerCase(request);

  /*
    Determine what data is requested, open the needed fits files
      and set the parameters used to save the data
  */
  offset = 0;
  totalBytes = 0;
  sets = 0;
  if ( strstr( request, "wfsc")!=NULL ) {

    if ( debug ) {
      printf("  DClient_DM_Save: WFSC found\n");
    }


    if ( nBytes == 1152 ) {

      /*
	Set the Info pointer to the WFSC_Info structure.
      */
      Data_Ptr[sets] = DM_Vector + offset;
      offset += WFSC_VALUES;
      values[sets] = WFSC_VALUES;
      dimension[0] = 24;
      dimension[1] = 24;
      dimension[2] = frames;
      num_axis = 3;
      first_pixel[sets] = 1;
      totalBytes += WFSC_BYTES;

    } else if ( nBytes == 10368 ) {

      /*
	Set the Info pointer to the WFSC_Info structure.
      */
      Data_Ptr[sets] = DM_Vector + offset;
      offset += (72*72);
      values[sets] = (72*72);
      dimension[0] = 72;
      dimension[1] = 72;
      dimension[2] = frames;
      num_axis = 3;
      first_pixel[sets] = 1;
      totalBytes += (72*72)* sizeof(short);

    } else {

      printf("  DClient_DM_Save: Not the correct number of bytes received\n");
      printf("                   Received %ld\n", nBytes);
      printf("                   Could be 1152\n");
      printf("                        or 10368\n");
      return(-1);

    }

    /*
      Open FITS file
    */

    strcpy(filename,"!");
    strcat(filename,dirname);
    strcat(filename,"/dds_");
    strcat(filename,fname);
    strcat(filename,"_wfsc.fits");
    istatus = 0;
    fits_create_file( &fptr[sets], filename, &istatus);

    /*
      Create an image the correct size
    */
    fits_create_img( fptr[sets], SHORT_IMG, num_axis, dimension, &istatus);
 
    /*
      Insert key words
    */
    fits_update_key( fptr[sets], TSTRING, "OBS_DATE", time_string, "Observation Date", &istatus);
    key = frames;
    fits_update_key( fptr[sets], TSHORT, "FRAMES", &key, "", &istatus);

    sets++;

  }

  if ( strstr( request, "spots")!=NULL ) {

    if ( debug ) {
      printf("  DClient_DM_Save: Spots found\n");
    }

    /*
      Set the Info pointer to the WFSC_Info structure.
    */
    Data_Ptr[sets] = DM_Vector + offset;
    offset += WFSC_VALUES;
    values[sets] = WFSC_VALUES;
    dimension[0] = 24;
    dimension[1] = 24;
    dimension[2] = frames;
    num_axis = 3;
    first_pixel[sets] = 1;
    totalBytes += WFSC_BYTES;

    /*
      Open FITS file
    */

    strcpy(filename,"!");
    strcat(filename,dirname);
    strcat(filename,"/dds_");
    strcat(filename,fname);
    strcat(filename,"_spots.fits");
    istatus = 0;
    fits_create_file( &fptr[sets], filename, &istatus);

    /*
      Create an image the correct size
    */
    fits_create_img( fptr[sets], SHORT_IMG, num_axis, dimension, &istatus);
 
    /*
      Insert key words
    */
    fits_update_key( fptr[sets], TSTRING, "OBS_DATE", time_string, "Observation Date", &istatus);
    key = frames;
    fits_update_key( fptr[sets], TSHORT, "FRAMES", &i, "", &istatus);

    sets++;

  }

  if ( strstr( request, "slope")!=NULL ) {

    if ( debug ) {
      printf("  DClient_DM_Save: Slope found\n");
    }

    /*
      Set the Info pointer to the WFSC_Info structure.
    */
    Data_Ptr[sets] = DM_Vector + offset;
    offset += SLOPE_VALUES;
    values[sets] = SLOPE_VALUES;
    dimension[0] = SLOPE_VALUES;
    dimension[1] = frames;
    num_axis = 2;
    first_pixel[sets] = 1;
    totalBytes += SLOPE_BYTES;

    /*
      Open FITS file
    */

    strcpy(filename,"!");
    strcat(filename,dirname);
    strcat(filename,"/dds_");
    strcat(filename,fname);
    strcat(filename,"_slopes.fits");
    istatus = 0;
    fits_create_file( &fptr[sets], filename, &istatus);

    /*
      Create an image the correct size
    */
    fits_create_img( fptr[sets], SHORT_IMG, num_axis, dimension, &istatus);
 
    /*
      Insert key words
    */
    fits_update_key( fptr[sets], TSTRING, "OBS_DATE", time_string, "Observation Date", &istatus);
    key = frames;
    fits_update_key( fptr[sets], TSHORT, "FRAMES", &i, "", &istatus);

    sets++;

  }

  if ( strstr( request, "rtr")!=NULL ) {

    if ( debug ) {
      printf("  DClient_DM_Save: RTR found\n");
    }

    /*
      Set the Info pointer to the WFSC_Info structure.
    */
    Data_Ptr[sets] = DM_Vector + offset;
    offset += RTR_VALUES;
    values[sets] = RTR_VALUES;
    dimension[0] = RTR_VALUES;
    dimension[1] = frames;
    num_axis = 2;
    first_pixel[sets] = 1;
    totalBytes += RTR_BYTES;

    /*
      Open FITS file
    */

    strcpy(filename,"!");
    strcat(filename,dirname);
    strcat(filename,"/dds_");
    strcat(filename,fname);
    strcat(filename,"_rtr.fits");
    istatus = 0;
    fits_create_file( &fptr[sets], filename, &istatus);

    /*
      Create an image the correct size
    */
    fits_create_img( fptr[sets], SHORT_IMG, num_axis, dimension, &istatus);
 
    /*
      Insert key words
    */
    fits_update_key( fptr[sets], TSTRING, "OBS_DATE", time_string, "Observation Date", &istatus);
    key = frames;
    fits_update_key( fptr[sets], TSHORT, "FRAMES", &i, "", &istatus);

    sets++;

  }

  if ( strstr( request, "cmd")!=NULL ) {

    if ( debug ) {
      printf("  DClient_DM_Save: Cmd found\n");
    }

    /*
      Set the Info pointer to the WFSC_Info structure.
    */
    Data_Ptr[sets] = DM_Vector + offset;
    offset += CMD_VALUES;
    values[sets] = CMD_VALUES;
    dimension[0] = CMD_VALUES;
    dimension[1] = frames;
    num_axis = 2;
    first_pixel[sets] = 1;
    totalBytes += CMD_BYTES;

    /*
      Open FITS file
    */

    strcpy(filename,"!");
    strcat(filename,dirname);
    strcat(filename,"/dds_");
    strcat(filename,fname);
    strcat(filename,"_cmd.fits");
    istatus = 0;
    fits_create_file( &fptr[sets], filename, &istatus);

    /*
      Create an image the correct size
    */
    fits_create_img( fptr[sets], SHORT_IMG, num_axis, dimension, &istatus);
 
    /*
      Insert key words
    */
    fits_update_key( fptr[sets], TSTRING, "OBS_DATE", time_string, "Observation Date", &istatus);
    key = frames;
    fits_update_key( fptr[sets], TSHORT, "FRAMES", &i, "", &istatus);

    sets++;

  }

  if ( strstr( request, "cur")!=NULL ) {

    if ( debug ) {
      printf("  DClient_DM_Save: Cur found\n");
    }

    /*
      Set the Info pointer to the WFSC_Info structure.
    */
    Data_Ptr[sets] = DM_Vector + offset;
    offset += CUR_VALUES;
    values[sets] = CUR_VALUES;
    dimension[0] = CUR_VALUES;
    dimension[1] = frames;
    num_axis = 2;
    first_pixel[sets] = 1;
    totalBytes += CUR_BYTES;

    /*
      Open FITS file
    */

    strcpy(filename,"!");
    strcat(filename,dirname);
    strcat(filename,"/dds_");
    strcat(filename,fname);
    strcat(filename,"_cur.fits");
    istatus = 0;
    fits_create_file( &fptr[sets], filename, &istatus);

    /*
      Create an image the correct size
    */
    fits_create_img( fptr[sets], SHORT_IMG, num_axis, dimension, &istatus);
 
    /*
      Insert key words
    */
    fits_update_key( fptr[sets], TSTRING, "OBS_DATE", time_string, "Observation Date", &istatus);
    key = frames;
    fits_update_key( fptr[sets], TSHORT, "FRAMES", &i, "", &istatus);

    sets++;

  }

  if ( strstr( request, "pos")!=NULL ) {

    if ( debug ) {
      printf("  DClient_DM_Save: Pos found\n");
    }

    /*
      Set the Info pointer to the WFSC_Info structure.
    */
    Data_Ptr[sets] = DM_Vector + offset;
    offset += POS_VALUES;
    values[sets] = POS_VALUES;
    dimension[0] = POS_VALUES;
    dimension[1] = frames;
    num_axis = 2;
    first_pixel[sets] = 1;
    totalBytes += POS_BYTES;

    /*
      Open FITS file
    */

    strcpy(filename,"!");
    strcat(filename,dirname);
    strcat(filename,"/dds_");
    strcat(filename,fname);
    strcat(filename,"_pos.fits");
    istatus = 0;
    fits_create_file( &fptr[sets], filename, &istatus);

    /*
      Create an image the correct size
    */
    fits_create_img( fptr[sets], SHORT_IMG, num_axis, dimension, &istatus);
 
    /*
      Insert key words
    */
    fits_update_key( fptr[sets], TSTRING, "OBS_DATE", time_string, "Observation Date", &istatus);
    key = frames;
    fits_update_key( fptr[sets], TSHORT, "FRAMES", &i, "", &istatus);

    sets++;

  }

  if ( sets <=0 ) {
    
    printf("  DClient_DM_Save: Unknown request: %s\n", request);
    return(-1);

  }

  /*
    Check that we are going to save the correct number of bytes
  */
  if ( totalBytes != nBytes ) {

    printf("  DClient_DM_Save: Incorrect number of bytes to save\n");
    printf("                   Recieved %ld\n", nBytes);
    printf("                   To be saved %ld\n", totalBytes);
    printf("  DClient_DM_Save: No data saved!\n");

    for ( i=0; i<sets; i++) {

      /*
	Close all the fits files
      */
      fits_close_file( fptr[i], &istatus);
      fits_report_error( stderr, istatus);
    }

    return (-1);

  }

  /*
    Start timer to determine the rate at which data was saved
  */
  systime = (struct timeval *)malloc( sizeof(struct timeval) );
  status = gettimeofday(systime, NULL);
  start_sec = systime->tv_sec;
  start_usec = systime->tv_usec;

  /*
    Setup to watch when there are data to be read on sockfd
  */
  FD_ZERO(&rfds);
  FD_SET(DM_Save_Info->server.fd, &rfds);
  /* Wait up to 10 seconds */
  tv.tv_sec = 10;
  tv.tv_usec = 0;

  frame_counter = 0;

  printf("  =>      ");

  while ( continueRunning ) {

    if ( nBytes > 0 ){

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
      num = Server_Read( (Server_Info *)DM_Save_Info, sizeof(long));
      if ( status ) {
	strcpy( errorMsg, "DClient_DM_Save: Error in Server_Read getting frameNumber");
	printf("  %s\n", errorMsg);
	return(status);
      }
      *numberPtr++ = ntohl( *(long *)DM_Save_Info->server.in_line);
      nxtLong = (long *)DM_Vector;
      *nxtLong = *(numberPtr - 1);
      //      printf("  DClient_DM_Save: Frame Number = %d\n", *(numberPtr - 1));

#if( DEBUG )
      printf("  DClient_DM_Save: Frame Number = %d\n", *(numberPtr - 1));
#endif

      num = Server_Read( (Server_Info *)DM_Save_Info, nBytes);
      if ( status ) {
	strcpy( errorMsg, "DClient_DM_Save: Error in Server_Read getting frame");
	printf("  %s\n", errorMsg);
	return(status);
      }

      nxtArray = DM_Vector;
      nxtShort = (short *)DM_Save_Info->server.in_line;
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

      /*
	Loop over the types of data requested and put each type in the correct fits file
      */
      for ( i=0; i<sets; i++) {
	fits_write_img( fptr[i], TSHORT, first_pixel[i], values[i], Data_Ptr[i], &istatus);
	first_pixel[i] += values[i];
      }

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
	status = select( (DM_Save_Info->server.fd)+1, &rfds, NULL, NULL, &tv);
	if ( !status ) {
	  /*
	    DServ has not sent any data for too long so return from this routine
	  */
	  printf("  DClient_DM_Save: After: Tired of waiting for DServ to send data\n");
	  nBytes = -2;
	  continueRunning = 0;
	  continue;
	}

	/*
	  nBytes is available, so read it
	*/
	status = Server_Read( (Server_Info *)DM_Save_Info, sizeof(long));
	if ( status ) {
	  strcpy( errorMsg, "DClient_DM_Save: Error in Server_Read getting nBytes (1)");
	  printf("  %s\n", errorMsg);
	  continueRunning = 0;
	  continue;
	}
	nBytes = ntohl( *(long *)DM_Save_Info->server.in_line);

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
      status = select( (DM_Save_Info->server.fd)+1, &rfds, NULL, NULL, &tv);

      if ( !status ) {

	/*
	  DServ has not sent any data for too long so return from this routine
	*/
	printf("  DClient_DM_Save: Loop: Tired of waiting for DServ to send data\n");
	continueRunning = 0;
	break;

      }

      /*
	nBytes is available, so read it
      */
      status = Server_Read( (Server_Info *)DM_Save_Info, sizeof(long));
      if ( status ) {
	strcpy( errorMsg, "DClient_DM_Save: Error in Server_Read getting nBytes (2)");
	printf("  %s\n", errorMsg);
	continueRunning = 0;
	break;
      }
      nBytes = ntohl( *(long *)DM_Save_Info->server.in_line);

      /*
	Increment the number of times we have read a -2 (new data not available)
	  from the DM Data Server
      */
      tries++;

      if ( tries > max_tries ) {

	printf("  DClient_DM_Save: Requesting Status\n");
	/*
	  Send a status request to DServ so it will send a flag (nBytes)
	    is important because if the server is writing to this port when we close
	    it on this end we get a broke pipe error on the DServ side and DServ
	    dies a terrible death.
	*/ 
	strcpy( DM_Save_Info->server.out_line, "Status\n");
	if ( debug ) {
	  printf("  DClient_DM_Save: requested %s", DM_Save_Info->server.out_line);
	}
	status = Server_Write( (Server_Info *)DM_Save_Info);
	if ( status ) {
	  strcpy( errorMsg, "DClient_DM_Save: Error in Server_Write sending Save request");
	  printf("  %s\n", errorMsg);
	  return(status);
	}

	tries = 0;

      }

    } else {

      printf("\n");
      strcpy( errorMsg, "DClient_DM_Save: Error reading the correct number of bytes");
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
  strcpy( DM_Save_Info->server.out_line, "Stop\n");
  if ( debug ) {
    printf("  DClient_DM_Save: requested %s", DM_Save_Info->server.out_line);
  }
  status = Server_Write( (Server_Info *)DM_Save_Info);
  if ( status ) {
    strcpy( errorMsg, "DClient_DM_Save: Error in Server_Write sending Save request");
    printf("  %s\n", errorMsg);
    return(status);
  }

  usleep(100000);

  /*
    Flush the port so we know when DServ has properly closed the socket and we can
      close the socket on this side.
  */
  status = Server_Flush( (Server_Info *)DM_Save_Info);
  if ( status ) {
    strcpy( errorMsg, "DClient_DM_Save: Error in Server_Flush");
    printf("  %s\n", errorMsg);
    return(status);
  }

  /*
    Finish with .fits file
  */
  for ( i=0; i<sets; i++) {

    /*
      Close all the fits files
    */
    fits_close_file( fptr[i], &istatus);
    fits_report_error( stderr, istatus);
  }

  /* Open FITS file */

  strcpy(filename,dirname);
  strcat(filename,"/dds_");
  strcat(filename,fname);

  printf(" frames saved to %s_*.fits in %f seconds (%3.0f Hz)\n", filename, total_time,
	 (float) frame_counter/total_time);

  status = DClient_SaveFrameNumbers( frames, frameNumbers, filename, time_string);

  free(DM_Vector);
  free(frameNumbers);

  return(0);
}
