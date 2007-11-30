/*
  PCR_GetWFSC.c

  This routine connects to the PC-Reconstructor (PCR), request a
    WFSC frame data stream and puts these data in the the proper
    memory.

  **Note: The data recieved from the PCR are in "network" format, and here are stored
    in the DServ memory in this "network" format, so no ntoh*() routines are not needed
    for storage and no hton*() routines are needed in DServ_GetSingle.c

  Written  20nov05  DLM  Started with DServ.c

  Altered  11may06  DLM  Changed name from DServ_StartPCR.c

  Altered  17jul06  DLM  Removed conversion of data to "host" format
*/

#include "PCR.h"
#include "DServ.h"

/*================================================================================*
 * Start server to manage data servers
 *================================================================================*/
void *PCR_GetWFSC( void *Passed_Info )
{
  int status, i;
  long nBytes;
  char request[STRING_LENGTH];
  char *longPtr;
  long tempLong;
  char skipPtr[640];  // 4*80*2=640  Rows*columns*size
  int nread, nsaved, nvalues, nerror;
  int save_bytes, extra_row_bytes, extra_edge_bytes;
  int dimension;
  int local_debug = 0;

  /*
    Local pointers to varible passed from the process which created this thread
  */
  PCR_Thread_Client *Main_Info;
  DServ_Info *WFSC_Info;
  int *continueRunning;
  int *running;
  int *reset;
  int *debug;

  /*
    Variables needed to use the select commands to watch for data on a port
  */
  fd_set rfds;
  struct timeval tv;

  /*
    Variables used to determine the rate at which frames are arriving from
      the PCR
  */
  struct timeval *systime;
  int start_sec, start_usec;
  int end_sec, end_usec;
  float total_time;
  int count;

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (PCR_Thread_Client *)Passed_Info;
  WFSC_Info = Main_Info->info;
  continueRunning = Main_Info->continueRunning;
  running = Main_Info->running;
  reset = Main_Info->reset;
  debug = Main_Info->debug;

  if ( local_debug ) {
    printf("  PCR_GetWFSC: WFSC_Info->socket->name = %s\n", WFSC_Info->socket->name);
    printf("  PCR_GetWFSC: WFSC_Info->data->num_frames = %d\n", WFSC_Info->data->num_frames);
    fflush(stdout);
  }

  while ( *continueRunning ) {
    /*
      Open a socket to the PCR
    */
    while ( !WFSC_Info->socket->connected ) {

      if ( *debug ) {
	printf("  PCR_GetWFSC: Trying to connect to PCR WFSC server\n");
	fflush(stdout);
      }

      status =  Socket_OpenClient ( (Socket_Info *)WFSC_Info->socket, local_debug);
      if ( status ) {
	if ( local_debug ) {
	  printf("  PCR_GetWFSC: Unable to connect to the PCR WFSC port in Socket_OpenClient\n");
	  printf("               Will wait and try again\n");
	  fflush(stdout);
	}
	sleep(1);

      } else {

	/*
	  Send the request to start a WFSC data server
	*/
	strcpy( request, "start_WFSC\n");
	if ( Socket_StringWrite( WFSC_Info->socket->sockfd, request, strlen(request)) != strlen(request) ) {
	  printf("  PCR_GetWFSC: Error writing start_WFSC to PCR WFSC port in Socket_StringWrite\n");
	  printf("               Will wait and try again\n");
	  fflush(stdout);
	  status =  Socket_Close ( (Socket_Info *)WFSC_Info->socket, *debug);
	  sleep(1);
	}
      }

      /*
	Check that this thread has the OK from the main thread to continue running
      */
      if ( !*continueRunning ) {

	/*
	  Set running flag for the PCR_WFSC to 0 so other threads know this thread has stopped
	*/
	*running = 0;

	return( (void *)0);
      }

    }

    /*
      Watch to see when there is data to be read on sockfd
    */
    FD_ZERO(&rfds);
    FD_SET(WFSC_Info->socket->sockfd, &rfds);
    /* Wait up to one seconds. */
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
 
    status = select( (WFSC_Info->socket->sockfd)+1, &rfds, NULL, NULL, &tv);

    if ( !status ) {

      /*
	Jump to the end of the loop to see if a shutdown has been requested.  If
	  not (*continueRunning != 0) then we will be back to check for the number
	  of bytes
      */
      continue;

    }

    /*
      We have reveived data from the PCR server, so the local client was just
        started or reset.  Either way, we will want to now continue to receive
	consecutive frame.
    */
    *reset = 0;

    /*
      Read the number of bytes per frame and convert from network to host
    */
    longPtr = (char *)&tempLong;
    if ( Socket_Read( WFSC_Info->socket->sockfd, longPtr, sizeof(long)) ) {
      if ( *debug ) {
	printf("  PCR_GetWFSC: Error reading nBytes in Socket_Read\n");
	fflush(stdout);
      }
      status =  Socket_Close ( (Socket_Info *)WFSC_Info->socket, *debug);
      continue;
    }
    nBytes = ntohl(tempLong);

    if ( *debug ) {
      printf("  PCR_GetWFSC: Frame size in bytes = %ld\n", nBytes);
      fflush(stdout);
    }

    if ( nBytes == 1159 ) {
      WFSC_Info->data->wfsc_bytes = 1152;
      WFSC_Info->data->total_bytes = nBytes;
      dimension = 24;
      extra_row_bytes = 0;
      extra_edge_bytes = 0;
    } else if ( nBytes == 1359 ) {
      WFSC_Info->data->wfsc_bytes = 1152;
      WFSC_Info->data->total_bytes = nBytes;
      dimension = 24;
      extra_row_bytes = 26*sizeof(short);
      extra_edge_bytes = 1*sizeof(short);
    } else if ( nBytes == 10375 ) {
      WFSC_Info->data->wfsc_bytes = 10368;
      WFSC_Info->data->total_bytes = nBytes;
      dimension = 72;
      extra_row_bytes = 0;
      extra_edge_bytes = 0;
    } else if ( nBytes == 12807 ) {
      WFSC_Info->data->wfsc_bytes = 10368;
      WFSC_Info->data->total_bytes = nBytes;
      dimension = 72;
      extra_row_bytes = 4*80*sizeof(short);
      extra_edge_bytes = 4*sizeof(short);
    } else {
      printf("  PCR_GetWFSC: Incorrect number of bytes recieved from PCR\n");
      printf("       Could be  1159\n");
      printf("             or  1359\n");
      printf("             or 10375\n");
      printf("             or 12807\n");
      printf("       Recieved %ld\n", nBytes);
      fflush(stdout);
      *continueRunning = 0;
      continue;
    }

    if ( local_debug ) {
      printf("  PCR_GetWFSC: WFSC_Info->data->wfsc_bytes = %d\n", WFSC_Info->data->wfsc_bytes);
      printf("  PCR_GetWFSC: WFSC_Info->data->total_bytes = %d\n", WFSC_Info->data->total_bytes);
      fflush(stdout);
    }

    /*
      Counter used to determine the rate at which frame are arriving from
        the PCR (WFSC port)
    */
    count = 0;

    /*
      Get sytem time for the start of receiving data so we can calculate
        the rate.
    */
    systime = (struct timeval *)malloc( sizeof(struct timeval) );
    status = gettimeofday(systime, NULL);
    start_sec = systime->tv_sec;
    start_usec = systime->tv_usec;

    while ( *continueRunning && !(*reset) ) {

      /*
	Watch to see when there is data to be read on sockfd
      */
      FD_ZERO(&rfds);
      FD_SET(WFSC_Info->socket->sockfd, &rfds);
      /* Wait up to one tenth of a seconds. */
      tv.tv_sec = 0;
      tv.tv_usec = 200000;
 
      status = select( (WFSC_Info->socket->sockfd)+1, &rfds, NULL, NULL, &tv);

      if ( !status ) {

	/*
	  If we do not receive an SOF in 1 second (slower than we will ever run
	    the AO loop) then we will assume the PCR server is no long sending data.
	  The next time we will get data from the server, it will be number of bytes
	    in the frame (a start_WFSC has been given)
	*/
	*(WFSC_Info->receiving_data) = 0;
	*(WFSC_Info->rate) = 0;
      
	/*
	  Get sytem time for the start of receiving data so we can calculate
	    the rate.
	*/
	systime = (struct timeval *)malloc( sizeof(struct timeval) );
	status = gettimeofday(systime, NULL);
	start_sec = systime->tv_sec;
	start_usec = systime->tv_usec;

	count = 0;

	continue;

      } else {

	/*
	  Data is ready so set the Info flag so the User Interface will know
	*/
	*(WFSC_Info->receiving_data) = 1;
	
      }


      /*
	Read the Start-Of-Frame (SOF)
      */
      if ( Socket_Read( WFSC_Info->socket->sockfd, WFSC_Info->data->dataPtr, 3*sizeof(char)) ) {
	printf("  PCR_GetWFSC: Connection lost to WFSC port of the PCR (SOF)\n");
	fflush(stdout);
	status =  Socket_Close ( (Socket_Info *)WFSC_Info->socket, *debug);
	*reset = 1;
	continue;
      }

      if ( strncmp (WFSC_Info->data->dataPtr, "SOF", strlen("SOF") ) ) {
	printf("  PCR_GetWFSC: SOF not found\n");
	fflush(stdout);
	if ( (status = PCR_FindSOF( WFSC_Info->data->dataPtr, WFSC_Info->socket->sockfd, *debug)) ) {
	  printf("  PCR_GetWFSC: Connection lost to WFSC port of the PCR (SOF)\n");
	  fflush(stdout);
	  *reset = 1;
	  continue;
	}	    
      }

      /*
	Move past the 'SOF'
      */
      WFSC_Info->data->dataPtr+=3*sizeof(char);

      /*
	Watch to see when there is data to be read on sockfd
      */
      FD_ZERO(&rfds);
      FD_SET(WFSC_Info->socket->sockfd, &rfds);
      /* Wait up to one seconds. */
      tv.tv_sec = 1;
      tv.tv_usec = 0;
 
      status = select( (WFSC_Info->socket->sockfd)+1, &rfds, NULL, NULL, &tv);

      if ( !status ) {

	printf("  PCR_GetWFSC: Error, did not receive frame number from PCR\n");
	fflush(stdout);
	status =  Socket_Close ( (Socket_Info *)WFSC_Info->socket, *debug);
	*reset = 1;
	continue;

      }

      /*
	Read the frame number
      */
      if ( Socket_Read( WFSC_Info->socket->sockfd, WFSC_Info->data->dataPtr, sizeof(long)) ) {
	if ( *debug ) {
	  printf("  PCR_GetWFSC: Error reading frame number in Socket_Read\n");
	  fflush(stdout);
	}
	status =  Socket_Close ( (Socket_Info *)WFSC_Info->socket, *debug);
	*reset = 1;
	continue;
      }

      /*
	Move past the frame number and increment byte counter
      */
      WFSC_Info->data->dataPtr+=sizeof(long);
      nread = WFSC_Info->data->header_bytes;

      /*
	Watch to see when there is data to be read on sockfd
      */
      FD_ZERO(&rfds);
      FD_SET(WFSC_Info->socket->sockfd, &rfds);
      /* Wait up to one seconds. */
      tv.tv_sec = 1;
      tv.tv_usec = 0;
 
      status = select( (WFSC_Info->socket->sockfd)+1, &rfds, NULL, NULL, &tv);

      if ( !status ) {

	printf("  PCR_GetWFSC: Error, did not receive frame from PCR\n");
	fflush(stdout);
	status =  Socket_Close ( (Socket_Info *)WFSC_Info->socket, *debug);
	*reset = 1;
	continue;

      }

      /*
	Read the frame

	=> Skip the first several rows
      */
      nread += extra_row_bytes;
      nread -= Socket_Read( WFSC_Info->socket->sockfd, skipPtr, extra_row_bytes);

      /*
	=> Read a row and save "dimension" values into memory.  Do this "dimension" times.
	   -> Read and throw away edge pixels
	   -> Read "dimension" pixels and put into memory
	   -> Read and throw away edge pixels
      */
      nsaved = 0;
      save_bytes = dimension*sizeof(short);
      for ( i=0; i<dimension; i++) {
	nread += extra_edge_bytes;
	nerror += Socket_Read( WFSC_Info->socket->sockfd, skipPtr, extra_edge_bytes);
	nsaved += save_bytes;
	nerror += Socket_Read( WFSC_Info->socket->sockfd, WFSC_Info->data->dataPtr, save_bytes);
	WFSC_Info->data->dataPtr += save_bytes;
	nread += extra_edge_bytes;
	nerror -= Socket_Read( WFSC_Info->socket->sockfd, skipPtr, extra_edge_bytes);
      }
      nread += nsaved;
      nread -= nvalues;

      /*
	=> Skip the last several rows
      */
      nread += extra_row_bytes;
      nread -= Socket_Read( WFSC_Info->socket->sockfd, skipPtr, extra_row_bytes);

      /*
	Check that we read enough data
      */
      if ( nread != WFSC_Info->data->total_bytes ) {
	printf("  PCR_GetWFSC: Not the correct amount of data read\n");
	printf("           Read %d\n", nread);
	printf("       Expected %d\n", WFSC_Info->data->total_bytes);
	fflush(stdout);
	*continueRunning = 0;
	continue;
      }

      /*
	Check that we saved enough data
      */
      if ( nsaved != WFSC_Info->data->wfsc_bytes ) {
	printf("  PCR_GetWFSC: Not the correct amount of data saved\n");
	printf("          Saved %d\n", nsaved);
	printf("       Expected %d\n", WFSC_Info->data->wfsc_bytes);
	fflush(stdout);
	*continueRunning = 0;
	continue;
      }

      /*
	Move all pointers forward one frame
      */
      WFSC_Info->data->last = WFSC_Info->data->active;
      WFSC_Info->data->active = WFSC_Info->data->nextFrame[WFSC_Info->data->active];
      WFSC_Info->data->dataPtr = WFSC_Info->data->framePtr[WFSC_Info->data->active];

      count++;

      if ( count > 100 ) {

	/*
	  Calculate the data rate
	*/
	status = gettimeofday(systime, NULL);
	end_sec = systime->tv_sec;
	end_usec = systime->tv_usec;
	total_time = (float) (end_sec-start_sec) + (float) (end_usec-start_usec) /1.0e6 ;
	*(WFSC_Info->rate) = (int)((float) count/total_time);
	start_sec = end_sec;
	start_usec = end_usec;
	count = 0;
      }

    }

    if ( WFSC_Info->socket->connected ) {

      if ( *debug ) {
	printf("  PCR_GetWFSC: reset received, flushing the port\n");
	fflush(stdout);
      }

      /*
	Watch to see when there is data to be read on sockfd
      */
      FD_ZERO(&rfds);
      FD_SET(WFSC_Info->socket->sockfd, &rfds);
      /* Wait up to one-tenth of a seconds. */
      tv.tv_sec = 0;
      tv.tv_usec = 100000;

      status = select( (WFSC_Info->socket->sockfd)+1, &rfds, NULL, NULL, &tv);

      if ( status ) {

	status = Socket_Flush( WFSC_Info->socket->sockfd);

      }

    }

    /*
      Either continueRunning was set to FALSE, we lost connection to the PCR WFSC port or a reset
        request from the GUI was received.  In all cases we need to set the receiving_data flag to
	FALSE so the DServ_DM_Single will send a repeat frame flag.
    */
    *(WFSC_Info->receiving_data) = 0;

  }

  if ( WFSC_Info->socket->connected ) {

    status =  Socket_Close ( (Socket_Info *)WFSC_Info->socket, *debug);
    if ( status ) {
      printf("  PCR_GetWFSC: Error in Socket_Close\n");
      fflush(stdout);
    }

  }

  if ( *debug ) {
    printf("  PCR_GetWFSC: Shutdown\n");
    fflush(stdout);
  }

  /*
    Set running flag for the PCR_WFSC to 0 so other threads know this thread has stopped
  */
  *running = 0;
  *(WFSC_Info->receiving_data) = 0;

  return( (void *)0);

}
