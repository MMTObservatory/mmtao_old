/*
  Source_Stream_DM.c

  This routine starts a test data server which streams data

  Written  20nov05  DLM  Started with DServ.c

*/

#include "PCR.h"
#include "DServ.h"

#include <fitsio.h>

/*================================================================================*
 * Start server to manage data servers
 *================================================================================*/
int Source_Stream_DM( DServ_Info *Info, long debug )
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
  short **DM_Vector = NULL;
  short *host_data;
  short *data;
  char *dataPtr;
  short nullvalue = 0;
  int anynull;
  char string_value[80];
  int num_values;
  int num_frames;
  static char File_Base[STRING_LENGTH];
  int offset;
  int local_debug = 0;

  /*
    Set the base name all the files (wfsc, slopes, rtr, cmd, cur, pos) to be read
  */
  strcpy( File_Base, "dds_09.42.30_");
  if ( debug ) {
    printf("  Source_Stream_DM: Reading files = %s*.fits\n", File_Base);
    fflush(stdout);
  }

  /*
    Read a fits file with "real" Spots data
  */
  strcpy(filename, File_Base);
  strcat(filename,"spots.fits");
  if ( local_debug ) {
    printf("  Source_Stream_DM: Spots file = %s\n", filename);
    fflush(stdout);
  }
  istatus = 0;
  fits_open_file( &fptr, filename, READONLY, &istatus);
  if ( istatus ) {
    printf("  Source_Stream_DM: Error opening file %s: istatus = %d\n", filename, istatus);
    fflush(stdout);
    return(-1);
  }

  /*
    Read Spots header information.  Once we know the number of frames, we ca
      can allocate the needed memory
  */
  fits_read_card( fptr, "NAXIS1", string_value, &istatus);
  num_values = atoi(&string_value[10]);

  fits_read_card( fptr, "NAXIS2", string_value, &istatus);
  num_values = atoi(&string_value[10]);

  fits_read_card( fptr, "NAXIS3", string_value, &istatus);
  num_frames = atoi(&string_value[10]);

  num_values = num_values*num_values;

  if ( local_debug ) {
    printf("                     num_values = %d\n", num_values);
    printf("                     num_frames = %d\n", num_frames);
    fflush(stdout);
  }

  /*
    Put appropriate values into the Info structure
  */
  Info->data->num_frames = num_frames;
  Info->data->all_bytes = TOTAL_DM_VALUES;  // *NOTE* This hold number of values rather than bytes
  Info->data->total_bytes = TOTAL_DM_VALUES*sizeof(short);
  
  if ( num_values != WFSC_VALUES ) {
    printf("  Source_Stream: Wrong number of values in %s: %d\n", filename, num_values);
    printf("                 Expected %d\n", WFSC_VALUES);
    fflush(stdout);
    return(-1);
  }

  /*
    Allocate needed memory
  */
  DM_Vector = (short **) malloc (num_frames*sizeof(short *));
  DM_Vector[0] = (short *) malloc (num_frames*Info->data->total_bytes);
  host_data = (short *) malloc (num_values*sizeof(short));

  memset( DM_Vector[0], 0,  Info->data->total_bytes);
  for ( i=1; i<num_frames; i++) {
    DM_Vector[i] = DM_Vector[i-1] + Info->data->all_bytes;
    memset( DM_Vector[i], 0, Info->data->total_bytes );
  }

  first_pixel = 1;
  for ( i=0; i<num_frames; i++) {
    data = DM_Vector[i];
    fits_read_img( fptr, TSHORT, first_pixel, num_values, &nullvalue, data, &anynull, &istatus);
    first_pixel += num_values;
  }  

  fits_close_file( fptr, &istatus);

  /*
    Increment offset so the next data are put in the correct position of DM_Vector
  */
  offset = num_values;

  /*
    Read a fits file with "real" Slopes data
  */
  strcpy(filename, File_Base);
  strcat(filename,"slopes.fits");
  if ( local_debug ) {
    printf("  Source_Stream_DM: Slopes file = %s\n", filename);
    fflush(stdout);
  }
  istatus = 0;
  fits_open_file( &fptr, filename, READONLY, &istatus);
  if ( istatus ) {
    printf("  Source_Stream_DM: Error opening file %s: istatus = %d\n", filename, istatus);
    fflush(stdout);
    return(-1);
  }

  /*
    Read Slopes header information.
  */
  fits_read_card( fptr, "NAXIS1", string_value, &istatus);
  num_values = atoi(&string_value[10]);

  fits_read_card( fptr, "NAXIS2", string_value, &istatus);
  num_frames = atoi(&string_value[10]);

  if ( local_debug ) {
    printf("                     num_values = %d\n", num_values);
    printf("                     num_frames = %d\n", num_frames);
    fflush(stdout);
  }

  if ( num_values != SLOPE_VALUES ) {
    printf("  Source_Stream: Wrong number of values in %s: %d\n", filename, num_values);
    printf("                 Expected %d\n", SLOPE_VALUES);
    fflush(stdout);
    return(-1);
  }

  if ( num_frames != Info->data->num_frames ) {
    printf("  Source_Stream: Wrong number of frames in %s: %d\n", filename, num_frames);
    printf("                 Expected %d\n", Info->data->num_frames);
    fflush(stdout);
    return(-1);
  }

  first_pixel = 1;
  for ( i=0; i<num_frames; i++) {
    data = DM_Vector[i] + offset;
    fits_read_img( fptr, TSHORT, first_pixel, num_values, &nullvalue, data, &anynull, &istatus);
    first_pixel += num_values;
  }  

  fits_close_file( fptr, &istatus);

  offset += num_values;

  /*
    Read a fits file with "real" RTR data
  */
  strcpy(filename, File_Base);
  strcat(filename,"rtr.fits");
  if ( local_debug ) {
    printf("  Source_Stream_DM: RTR file = %s\n", filename);
    fflush(stdout);
  }
  istatus = 0;
  fits_open_file( &fptr, filename, READONLY, &istatus);
  if ( istatus ) {
    printf("  Source_Stream_DM: Error opening file %s: istatus = %d\n", filename, istatus);
    fflush(stdout);
    return(-1);
  }

  /*
    Read RTR header information.
  */
  fits_read_card( fptr, "NAXIS1", string_value, &istatus);
  num_values = atoi(&string_value[10]);

  fits_read_card( fptr, "NAXIS2", string_value, &istatus);
  num_frames = atoi(&string_value[10]);

  if ( local_debug ) {
    printf("                     num_values = %d\n", num_values);
    printf("                     num_frames = %d\n", num_frames);
    fflush(stdout);
  }

  if ( num_values != RTR_VALUES ) {
    printf("  Source_Stream: Wrong number of values in %s: %d\n", filename, num_values);
    printf("                 Expected %d\n", RTR_VALUES);
    fflush(stdout);
    return(-1);
  }

  if ( num_frames != Info->data->num_frames ) {
    printf("  Source_Stream: Wrong number of frames in %s: %d\n", filename, num_frames);
    printf("                 Expected %d\n", Info->data->num_frames);
    fflush(stdout);
    return(-1);
  }

  first_pixel = 1;
  for ( i=0; i<num_frames; i++) {
    data = DM_Vector[i] + offset;
    fits_read_img( fptr, TSHORT, first_pixel, num_values, &nullvalue, data, &anynull, &istatus);
    first_pixel += num_values;
  }  

  fits_close_file( fptr, &istatus);

  offset += num_values;

  /*
    Read a fits file with "real" Command data
  */
  strcpy(filename, File_Base);
  strcat(filename,"cmd.fits");
  if ( local_debug ) {
    printf("  Source_Stream_DM: Cmd file = %s\n", filename);
    fflush(stdout);
  }
  istatus = 0;
  fits_open_file( &fptr, filename, READONLY, &istatus);
  if ( istatus ) {
    printf("  Source_Stream_DM: Error opening file %s: istatus = %d\n", filename, istatus);
    fflush(stdout);
    return(-1);
  }

  /*
    Read Command header information.
  */
  fits_read_card( fptr, "NAXIS1", string_value, &istatus);
  num_values = atoi(&string_value[10]);

  fits_read_card( fptr, "NAXIS2", string_value, &istatus);
  num_frames = atoi(&string_value[10]);

  if ( local_debug ) {
    printf("                     num_values = %d\n", num_values);
    printf("                     num_frames = %d\n", num_frames);
    fflush(stdout);
  }

  if ( num_values != CMD_VALUES ) {
    printf("  Source_Stream: Wrong number of values in %s: %d\n", filename, num_values);
    printf("                 Expected %d\n", CMD_VALUES);
    fflush(stdout);
    return(-1);
  }

  if ( num_frames != Info->data->num_frames ) {
    printf("  Source_Stream: Wrong number of frames in %s: %d\n", filename, num_frames);
    printf("                 Expected %d\n", Info->data->num_frames);
    fflush(stdout);
    return(-1);
  }

  first_pixel = 1;
  for ( i=0; i<num_frames; i++) {
    data = DM_Vector[i] + offset;
    fits_read_img( fptr, TSHORT, first_pixel, num_values, &nullvalue, data, &anynull, &istatus);
    first_pixel += num_values;
  }  

  fits_close_file( fptr, &istatus);

  offset += num_values;

  /*
    Read a fits file with "real" Current data
  */
  strcpy(filename, File_Base);
  strcat(filename,"cur.fits");
  if ( local_debug ) {
    printf("  Source_Stream_DM: Current file = %s\n", filename);
    fflush(stdout);
  }
  istatus = 0;
  fits_open_file( &fptr, filename, READONLY, &istatus);
  if ( istatus ) {
    printf("  Source_Stream_DM: Error opening file %s: istatus = %d\n", filename, istatus);
    fflush(stdout);
    return(-1);
  }

  /*
    Read Current header information.
  */
  fits_read_card( fptr, "NAXIS1", string_value, &istatus);
  num_values = atoi(&string_value[10]);

  fits_read_card( fptr, "NAXIS2", string_value, &istatus);
  num_frames = atoi(&string_value[10]);

  if ( local_debug ) {
    printf("                     num_values = %d\n", num_values);
    printf("                     num_frames = %d\n", num_frames);
    fflush(stdout);
  }

  if ( num_values != CUR_VALUES ) {
    printf("  Source_Stream: Wrong number of values in %s: %d\n", filename, num_values);
    printf("                 Expected %d\n", CUR_VALUES);
    fflush(stdout);
    return(-1);
  }

  if ( num_frames != Info->data->num_frames ) {
    printf("  Source_Stream: Wrong number of frames in %s: %d\n", filename, num_frames);
    printf("                 Expected %d\n", Info->data->num_frames);
    fflush(stdout);
    return(-1);
  }

  first_pixel = 1;
  for ( i=0; i<num_frames; i++) {
    data = DM_Vector[i] + offset;
    fits_read_img( fptr, TSHORT, first_pixel, num_values, &nullvalue, data, &anynull, &istatus);
    first_pixel += num_values;
  }  

  fits_close_file( fptr, &istatus);

  offset += num_values;

  /*
    Read a fits file with "real" Position data
  */
  strcpy(filename, File_Base);
  strcat(filename,"pos.fits");
  if ( local_debug ) {
    printf("  Source_Stream_DM: Position file = %s\n", filename);
    fflush(stdout);
  }
  istatus = 0;
  fits_open_file( &fptr, filename, READONLY, &istatus);
  if ( istatus ) {
    printf("  Source_Stream_DM: Error opening file %s: istatus = %d\n", filename, istatus);
    fflush(stdout);
    return(-1);
  }

  /*
    Read Position header information.
  */
  fits_read_card( fptr, "NAXIS1", string_value, &istatus);
  num_values = atoi(&string_value[10]);

  fits_read_card( fptr, "NAXIS2", string_value, &istatus);
  num_frames = atoi(&string_value[10]);

  if ( local_debug ) {
    printf("                     num_values = %d\n", num_values);
    printf("                     num_frames = %d\n", num_frames);
    fflush(stdout);
  }

  if ( num_values != POS_VALUES ) {
    printf("  Source_Stream: Wrong number of values in %s: %d\n", filename, num_values);
    printf("                 Expected %d\n", POS_VALUES);
    fflush(stdout);
    return(-1);
  }

  if ( num_frames != Info->data->num_frames ) {
    printf("  Source_Stream: Wrong number of frames in %s: %d\n", filename, num_frames);
    printf("                 Expected %d\n", Info->data->num_frames);
    fflush(stdout);
    return(-1);
  }

  first_pixel = 1;
  for ( i=0; i<num_frames; i++) {
    data = DM_Vector[i] + offset;
    fits_read_img( fptr, TSHORT, first_pixel, num_values, &nullvalue, data, &anynull, &istatus);
    first_pixel += num_values;
  }  

  fits_close_file( fptr, &istatus);

  offset += num_values;

  /*
    Convert ALL data to host format
  */
  for ( i=0; i<num_frames; i++) {
    for ( j=0; j<TOTAL_DM_VALUES; j++) {
      DM_Vector[i][j] = htons(DM_Vector[i][j]);
    }
  }

  /*
    Send the full number of bytes in each frame
  */
  nBytes = htonl(Info->data->total_bytes + HEADER_BYTES);
  longPtr = (char *)&nBytes;
  if ( Socket_Write( Info->socket->sockfd, longPtr, sizeof(long)) ) {
    if ( debug ) {
      printf("  Source_Stream_DM: Error writing nBytes in Socket_Write\n");
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
	printf("  Source_Stream_DM: Error writing SOF in Socket_Write\n");
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
	printf("  Source_Stream_DM: Error writing frame number in Socket_Write\n");
	fflush(stdout);
      }
      close( Info->socket->sockfd);
      return(-1);
    }

    /*
      Send the frame
    */
    dataPtr = (char *)DM_Vector[j];
    if ( Socket_Write( Info->socket->sockfd, dataPtr, Info->data->total_bytes ) ) {
      if ( debug ) {
	printf("  Source_Stream_DM: Error writing frame in Socket_Write\n");
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
      printf("  Source_Stream_DM: Recived request %s\n", request);
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
      j = 0;
    }

    //    k++;

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
	  if ( debug ) printf("  Source_Stream_DM: Error writing nBytes in Socket_Write\n");
	  close( Info->socket->sockfd);
	  return(-1);
	}
      }
      */
    }

  } while ( 1 );

  if ( debug ) {
    printf("  Source_Stream_DM: Data Server Shutdown\n");
    fflush(stdout);
  }

  close( Info->socket->sockfd);
  return(0);
}
