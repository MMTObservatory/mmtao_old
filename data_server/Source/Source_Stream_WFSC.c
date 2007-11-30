/*
  Source_Stream_WFSC.c

  This routine starts a test data server which streams data

  Written  20nov05  DLM  Started with DServ.c

*/

#include "PCR.h"
#include "DServ.h"

#include <fitsio.h>

/*================================================================================*
 * Start server to manage data servers
 *================================================================================*/
int Source_Stream_WFSC( DServ_Info *Info, char *type, int size, long debug )
{
  long num = 0;
  int i, j, k, status;
  long nBytes = -1;

  char sof[STRING_LENGTH];
  char request[STRING_LENGTH];
  char *longPtr;
  char *numPtr;

  /*
    Variables needed to use the select commands to watch for data on a port
  */
  fd_set rfds;
  struct timeval tv;

  /* fitsio variables */

  fitsfile *fptr;
  char filename[STRING_LENGTH];
  long first_pixel = 1;
  int istatus;

  /* Array variables */
  short **WFSC_Vector = NULL;
  short *data;
  char *dataPtr;
  short nullvalue = 0;
  int anynull;
  char string_value[80];
  int num_values;
  int num_frames;
  int frame_start;
  int local_debug = 0;
  debug = 1;

  if ( !strncmp( type, "bg", strlen("bg") ) ||
       !strncmp( type, "real", strlen("real") ) ) {

    /*
      Read a fits file with "real" data
    */

    if ( !strncmp( type, "real", strlen("real") ) ) {

      if ( size == 72 ) {
	strcpy(filename,"wfsc_data_72.fits");
      } else {
	strcpy(filename,"wfsc_data_24.fits");
      }
      frame_start = 0;

    } else {

	strcpy(filename,"bg.fits");

	frame_start = 2;

    }

    /*
    Open FITS file
    */
    if ( debug ) {
      printf("  Source_Stream_WFSC: WFSC file = %s\n", filename);
      fflush(stdout);
    }
    istatus = 0;
    fits_open_file( &fptr, filename, READONLY, &istatus);

    /*
      Read header information
    */
    fits_read_card( fptr, "NAXIS1", string_value, &istatus);
    num_values = atoi(&string_value[10]);

    fits_read_card( fptr, "NAXIS2", string_value, &istatus);
    num_values = atoi(&string_value[10]);

    fits_read_card( fptr, "NAXIS3", string_value, &istatus);
    num_frames = atoi(&string_value[10]);

    num_values = num_values*num_values;

    if ( local_debug ) {
      printf("                       num_values = %d\n", num_values);
      printf("                       num_frames = %d\n", num_frames);
      fflush(stdout);
    }

    Info->data->wfsc_bytes = num_values;
    Info->data->total_bytes = num_values * sizeof(short) + HEADER_BYTES;

    /*
      Allocate needed memory
    */
    WFSC_Vector = (short **) malloc (num_frames*sizeof(short *));
    WFSC_Vector[0] = (short *) malloc (num_frames*num_values*sizeof(short));

    memset( WFSC_Vector[0], 0, num_values*sizeof(short) );
    for ( i=1; i<num_frames; i++) {
      WFSC_Vector[i] = WFSC_Vector[i-1] + num_values;
      memset( WFSC_Vector[i], 0, num_values*sizeof(short) );
    }

    first_pixel = 1;
    for ( i=0; i<num_frames; i++) {
      data = WFSC_Vector[i];
      fits_read_img( fptr, TSHORT, first_pixel, num_values, &nullvalue, data, &anynull, &istatus);
      first_pixel += num_values;
    }  

    fits_close_file( fptr, &istatus);

    for ( i=0; i<num_frames; i++) {
      for ( j=0; j<num_values; j++) {
	WFSC_Vector[i][j] = htons(WFSC_Vector[i][j]);
      }
    }

  } else {

    /*
      Create test pattern
    */
    num_frames = 1;
    num_values = size*size;

    /*
      Allocate needed memory
    */
    WFSC_Vector = (short **) malloc (num_frames*sizeof(short *));
    WFSC_Vector[0] = (short *) malloc (num_frames*num_values*sizeof(short));
    memset( WFSC_Vector[0], 0, num_values*sizeof(short) );

    for ( i=1; i<num_values; i++) {
      WFSC_Vector[0][i] =  htons(i);
    }

    frame_start = 0;

  }
  /*
  for ( i=0; i<60; i++) {
    printf("%d  %d\n", i,  WFSC_Vector[200][i]);
  }  
  */
  /*
    Send the full number of bytes in each frame
  */
  nBytes = htonl(Info->data->total_bytes);
  longPtr = (char *)&nBytes;
  if ( Socket_Write( Info->socket->sockfd, longPtr, sizeof(long)) ) {
    if ( debug ) {
      printf("  Source_Stream_WFSC: Error writing nBytes in Socket_Write\n");
      fflush(stdout);
    }
    close( Info->socket->sockfd);
    return(-1);
  }

  sprintf( sof, "SOF");
  numPtr = (char *)&num;

  i = 0;
  j = 0;
  k = 0;

  do {

    /*
      Send Start-Of-Frame (SOF)
    */
    num = htonl(i);
    if ( Socket_Write( Info->socket->sockfd, sof, strlen(sof)) ) {
      if ( debug ) {
	printf("  Source_Stream_WFSC: Error writing SOF in Socket_Write\n");
	fflush(stdout);
      }
      close( Info->socket->sockfd);
      return(-1);
    }

    /*
      Send the frame number
    */
    if ( Socket_Write( Info->socket->sockfd, numPtr, sizeof(long)) ) {
      if ( debug ) {
	printf("  Source_Stream_WFSC: Error writing frame number in Socket_Write\n");
	fflush(stdout);
      }
      close( Info->socket->sockfd);
      return(-1);
    }

    /*
      Send the frame
    */
    dataPtr = (char *)WFSC_Vector[j];
    if ( Socket_Write( Info->socket->sockfd, dataPtr, Info->data->wfsc_bytes*sizeof(short) ) ) {
      if ( debug ) {
	printf("  Source_Stream_WFSC: Error writing frame in Socket_Write\n");
	fflush(stdout);
      }
      close( Info->socket->sockfd);
      return(-1);
    }

    /*
      Watch to see when there is data to be read on sockfd
    */
    FD_ZERO(&rfds);
    FD_SET(Info->socket->sockfd, &rfds);
    /* Wait up to one seconds. */
    tv.tv_sec = 0;
    tv.tv_usec = 0;
 
    status = select( (Info->socket->sockfd)+1, &rfds, NULL, NULL, &tv);

    if ( status ) {

      /*
	Read an instruction from the client
      */
      if ( Socket_StringRead( Info->socket->sockfd, request) ) {
	if ( debug ) {
	  printf("  Source_Start: Error reading request in Socket_StringRead\n");
	  fflush(stdout);
	}
	close( Info->socket->sockfd);
	return(-1);
      }
      printf("  Source_Stream_WFSC: Recived request %s\n", request);
      fflush(stdout);

      /*
	If we do not receive an SOF in 1 second (slower than we will ever run
	  the AO loop) then we will assume the PCR server is no long sending data.
	By calling continue we allow the while loop to see if continueRunning is
	  still true, to allow the user to shutdown this thread.
      */
      continue;

    }
    /*
      On my MAC a usleep(1600) delay gives about a 500 Hz rate on the full DM data
                a  sleep(1)    delay gives about a   1 Hz rate on the full DM data
        7sep06 DLM
    */
    usleep(1600);

    i++;
    j++;

    if ( j >= num_frames ) {
      j = frame_start;
    }

    //k++;

    if ( k > 500 ) {
      k = 0;
      printf("  Pause for 5 seconds\n");
      fflush(stdout);
      sleep(5);
      printf("  Starting again\n");
      fflush(stdout);

      /*
	The PCR_GetWFSC thread has reset now, so we must send the number of bytes
      if ( Info->data->total_bytes == 1359 || Info->data->total_bytes == 12807 ) {
	if ( Socket_Write( Info->socket->sockfd, longPtr, sizeof(long)) ) {
	  if ( debug ) printf("  Source_Stream_WFSC: Error writing nBytes in Socket_Write\n");
	  close( Info->socket->sockfd);
	  return(-1);
	}
      }
      */
    }

  } while ( 1 );

  if ( debug ) {
    printf("  Source_Stream_WFSC: Data Server Shutdown\n");
    fflush(stdout);
  }

  close( Info->socket->sockfd);
  return(0);
}
