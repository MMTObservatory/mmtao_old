/*
  PCR_GetDM.c

  This routine connects to the PC-Reconstructor (PCR), request a
    DM frame data stream and puts these data in the the proper
    memory.

  Written  20nov05  DLM  Started with DServ.c

  Altered  11may06  DLM  Changed name from DServ_StartPCR.c
*/

#include "PCR.h"
#include "DServ.h"

#define DEBUG 0

/*================================================================================*
 * Start server to manage data servers
 *================================================================================*/
void *PCR_GetDM( void *Passed_Info )
{
  int status;
  long nBytes;
  char request[STRING_LENGTH];
  char *longPtr;
  long tempLong;
  int save_bytes, nerror;
  int local_debug = 0;

  /*
    Local pointers to varible passed from the process which created this thread
  */
  PCR_Thread_Client *Main_Info;
  DServ_Info *DM_Info;
  int *continueRunning;
  int *running;
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

#if( DEBUG )
  short *shortPtr;
  int i, j;
#endif

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (PCR_Thread_Client *)Passed_Info;
  DM_Info = Main_Info->info;
  continueRunning = Main_Info->continueRunning;
  running = Main_Info->running;
  debug = Main_Info->debug;

  if ( local_debug ) {
    printf("  PCR_GetDM: DM_Info->socket->name       = %s\n", DM_Info->socket->name);
    printf("  PCR_GetDM: DM_Info->data->num_frames   = %d\n", DM_Info->data->num_frames);
    fflush(stdout);
  }

  while ( *continueRunning ) {

    /*
      Open a socket to the PCR
    */
    while ( !DM_Info->socket->connected ) {

      if ( *debug ) {
	printf("  PCR_GetDDS: Trying to connect to PCR DDS server\n");
	fflush(stdout);
      }

      status =  Socket_OpenClient ( (Socket_Info *)DM_Info->socket, *debug);
      if ( status ) {
	if ( local_debug ) {
	  printf("  PCR_GetDM: Unable to connect to the PCR DM port in Socket_OpenClient\n");
	  printf("               Will wait and try again\n");
	  fflush(stdout);
	}
	sleep(1);

      } else {

	/*
	  Send start to the DM data server.  Data does not flow until the loop
	    is closed.
	*/
	strcpy( request, "start_DM\n");
	if ( Socket_StringWrite( DM_Info->socket->sockfd, request, strlen(request)) != strlen(request) ) {
	  printf("  PCR_GetDM: Error writing start_DM to PCR DDS port in Socket_StringWrite\n");
	  printf("               Will wait and try again\n");
	  fflush(stdout);
	  status =  Socket_Close ( (Socket_Info *)DM_Info->socket, *debug);
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
    FD_SET(DM_Info->socket->sockfd, &rfds);
    /* Wait up to one seconds. */
    tv.tv_sec = 1;
    tv.tv_usec = 0;
 
    status = select( (DM_Info->socket->sockfd)+1, &rfds, NULL, NULL, &tv);

    if ( !status ) {

      /*
	Jump to the end of the loop to see if a shutdown has been requested.  If
	  not (*continueRunning != 0) then we will be back to check for the number
	  of bytes
      */
      continue;

    }

    /*
      Read the number of bytes per frame
    */
    longPtr = (char *)&tempLong;
    if ( Socket_Read( DM_Info->socket->sockfd, longPtr, sizeof(long)) ) {
      if ( *debug ) {
	printf("  PCR_GetDM: Error reading nBytes in Socket_Read\n");
	fflush(stdout);
      }
      continue;
    }
    nBytes = ntohl(tempLong);

    if ( *debug ) {
      printf("  PCR_GetDM: Frame size in bytes = %ld\n", nBytes);
      fflush(stdout);
    }

    if ( nBytes != DM_Info->data->total_bytes ) {
      printf("  PCR_GetDM: Error with the number of bytes in data frame\n");
      printf("             Read %ld\n", nBytes);
      printf("         Expected %d\n", DM_Info->data->total_bytes);
      fflush(stdout);
      *continueRunning = 0;
    }

    /*
      Counter used to determine the rate at which frame are arriving from
      the PCR (DDS port)
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

    while ( *continueRunning && DM_Info->socket->connected ) {

      /*
	Watch to see when there is data to be read on sockfd
      */
      FD_ZERO(&rfds);
      FD_SET(DM_Info->socket->sockfd, &rfds);
      /* Wait up to one seconds. */
      tv.tv_sec = 0;
      tv.tv_usec = 200000;
 
      status = select( (DM_Info->socket->sockfd)+1, &rfds, NULL, NULL, &tv);

      if ( !status ) {

	/*
	  If we do not receive an SOF in 1 second (slower than we will ever run
	    the AO loop) then we will assume the PCR server is no long sending data.
	  By calling continue we allow the while loop to see if continueRunning is
	    still true, to allow the user to shutdown this thread.
	*/
	*(DM_Info->receiving_data) = 0;
	*(DM_Info->rate) = 0;

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
	*(DM_Info->receiving_data) = 1;

      }

      /*
	Read the Start-Of-Frame (SOF)
      */
      if ( Socket_Read( DM_Info->socket->sockfd, DM_Info->data->dataPtr, 3*sizeof(char)) ) {
	printf("  PCR_GetDM: Connection lost to DM port of PCR (SOF)\n");
	fflush(stdout);
	status =  Socket_Close ( (Socket_Info *)DM_Info->socket, *debug);
	continue;
      }

      if ( strncmp (DM_Info->data->dataPtr, "SOF", strlen(DM_Info->data->dataPtr) ) ) {
	printf("  PCR_GetDM: SOF not found\n");
	fflush(stdout);
	if ( (status = PCR_FindSOF( DM_Info->data->dataPtr, DM_Info->socket->sockfd, *debug)) ) {
	  printf("  PCR_GetDM: Connection lost to DM port of PCR (SOF)\n");
	  fflush(stdout);
	  status =  Socket_Close ( (Socket_Info *)DM_Info->socket, *debug);
	  continue;
	}	    
      }

      /*
	Move past the 'SOF'
      */
      DM_Info->data->dataPtr+=3*sizeof(char);

      /*
	Watch to see when there is data to be read on sockfd
      */
      FD_ZERO(&rfds);
      FD_SET(DM_Info->socket->sockfd, &rfds);
      /* Wait up to one seconds. */
      tv.tv_sec = 1;
      tv.tv_usec = 0;
 
      status = select( (DM_Info->socket->sockfd)+1, &rfds, NULL, NULL, &tv);

      if ( !status ) {

	printf("  PCR_GetDM: Error, did not receive frame number from PCR\n");
	fflush(stdout);
	status =  Socket_Close ( (Socket_Info *)DM_Info->socket, *debug);
	continue;

      }

      /*
	Read the frame number
      */
      if ( Socket_Read( DM_Info->socket->sockfd, DM_Info->data->dataPtr, sizeof(long)) ) {
	printf("  PCR_GetDM: Connection lost to DM port of PCR (frame number)\n");
	fflush(stdout);
	status =  Socket_Close ( (Socket_Info *)DM_Info->socket, *debug);
	continue;
      }

      /*
	Move past the frame number
      */
      DM_Info->data->dataPtr+=sizeof(long);

      /*
	Watch to see when there is data to be read on sockfd
      */
      FD_ZERO(&rfds);
      FD_SET(DM_Info->socket->sockfd, &rfds);
      /* Wait up to one seconds. */
      tv.tv_sec = 1;
      tv.tv_usec = 0;
 
      status = select( (DM_Info->socket->sockfd)+1, &rfds, NULL, NULL, &tv);

      if ( !status ) {

	printf("  PCR_GetDM: Error, did not frame from PCR\n");
	fflush(stdout);
	status =  Socket_Close ( (Socket_Info *)DM_Info->socket, *debug);
	continue;

      }

      /*
	Read the the frame
      */
      save_bytes = DM_Info->data->total_bytes - DM_Info->data->header_bytes;
      nerror = Socket_Read( DM_Info->socket->sockfd, DM_Info->data->dataPtr, save_bytes);
      if ( nerror ) {
	printf("  PCR_GetDM: Connection lost to DM port of PCR (frame)\n");
	fflush(stdout);
	status =  Socket_Close ( (Socket_Info *)DM_Info->socket, *debug);
	continue;
      }

#if( DEBUG )
      DM_Info->data->dataPtr = DM_Info->data->framePtr[DM_Info->data->active] + DM_Info->data->slope_offset;
      shortPtr = (short *)DM_Info->data->dataPtr;
      for ( i=0; i<12; i++) {
	for ( j=0; j<12; j++) {
	  printf(" %4d", *shortPtr++);
	}
	printf("\n");
      }
      printf("\n\n");
      for ( i=0; i<12; i++) {
	for ( j=0; j<12; j++) {
	  printf(" %4d", *shortPtr++);
	}
	printf("\n");
      }
      printf("\n\n");
      fflush(stdout);
#endif

      /*
	Move all pointer forward one frame
      */
      DM_Info->data->last = DM_Info->data->active;
      DM_Info->data->active = DM_Info->data->nextFrame[DM_Info->data->active];
      DM_Info->data->dataPtr = DM_Info->data->framePtr[DM_Info->data->active];

      count++;

      if ( count > 100 ) {

	/*
	  Calculate the data rate
	*/
	status = gettimeofday(systime, NULL);
	end_sec = systime->tv_sec;
	end_usec = systime->tv_usec;
	total_time = (float) (end_sec-start_sec) + (float) (end_usec-start_usec) /1.0e6 ;
	*(DM_Info->rate) = (int) ((float) count/total_time);
	start_sec = end_sec;
	start_usec = end_usec;
	count = 0;
      }

    }

    /*
      Either continueRunning was set to FALSE or we lost connection to the PCR DDS port.  In
        either cases we need to set the receiving_data flag to FALSE so the DServ_DM_Single will
	send the data from the PCR WFSC port to the GUI
    */
    *(DM_Info->receiving_data) = 0;

  }

  if ( DM_Info->socket->connected ) {

    status =  Socket_Close ( (Socket_Info *)DM_Info->socket, *debug);
    if ( status ) {
      printf("  PCR_GetDM: Error in Socket_Close\n");
      fflush(stdout);
    }

    if ( *debug ) {
      printf("  PCR_GetDM: Shutdown\n");
      fflush(stdout);
    }
  }

  /*
    Set running flag for the PCR_DM to 0 so other threads know this thread has stopped
  */
  *running = 0;
  *(DM_Info->receiving_data) = 0;

  return( (void *)0);

}
