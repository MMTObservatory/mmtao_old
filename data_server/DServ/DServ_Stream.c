/*===============================================================================
 * DServ_Stream         
 *================================================================================

  This routine sends a stream of data (all frames) from the  WFSC or DM DServ
    memory to the client.

  Written  3sep06  DLM  Started with DServ_GetSingle.c

*/

#include "PCR.h"
#include "DServ.h"

#define TIMER 0

/*===============================================================================
 * DServ_Stream 
 *===============================================================================*/

int DServ_Stream( DServ_Info *WFSC_Info, DServ_Info *DM_Info, int sockfd, int *debug)
{
  DServ_Info *Info;
  int status;
  char request[STRING_LENGTH];
  int continueRunning = 1;

  /*
    Variables used to find the next frame
  */
  long nBytes, nBytes_send, wait;
  int first_frame, new_frame;
  long new_number, next_number;
  char *DServ_DataPtr, *DServ_NumberPtr, *dataPtr;
  long count;
  long number_tries = 10000000;

  int j;

  /*
    Variable to store which data to send
  */
  int sets, i;
  int offset[6];
  int bytes[6];

  /*
    Variables needed to use the select commands to watch for data on a port
  */
  fd_set rfds;
  struct timeval tv;

#if( TIMER )
  /*
    Variables used to determine the rate at which frames are arriving from
      the PCR
  */
  struct timeval *systime;
  int start_sec, start_usec;
  int end_sec, end_usec;
  float total_time;
  int counter;
#endif

  /*
    Read request from the client
  */
  if ( Socket_StringRead( sockfd, request) ) {
    if ( *debug ) {
      printf("  DServ_Stream: Error reading request in Socket_StringRead\n");
      fflush(stdout);
    }
    return(-1);
  }

  /*
    Check that a request was sent
  */
  if ( strlen(request) <= 1 ) {
    printf("  DServ_Stream: Warning: request of zero length recieved\n");
    fflush(stdout);
    return(-1);
  }

  if ( *debug ) {
    printf("  DServ_Stream: request = %s\n", request);
    fflush(stdout);
  }

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  status = Socket_StringLowerCase(request);

  /*
    Determine which image the client has requested and set the number of bytes
      to be sent and set the offset in the frame to the correct positin.
  */
  nBytes = 0;
  sets = 0;
  if ( strstr( request, "wfsc")!=NULL ) {

    if ( *debug ) {
      printf("  DServ_Stream: WFSC found\n");
      fflush(stdout);
    }

    /*
      Set the Info pointer to the WFSC_Info structure.
    */
    Info = WFSC_Info;

    offset[sets] = Info->data->wfsc_offset;
    bytes[sets] = Info->data->wfsc_bytes;
    sets++;
    nBytes += Info->data->wfsc_bytes;

  }

  if ( strstr( request, "spots")!=NULL ) {

    if ( *debug ) {
      printf("  DServ_Stream: Spots found\n");
      fflush(stdout);
    }

    /*
      Set the Info pointer to the DM_Info structure.
    */
    Info = DM_Info;

    offset[sets] = Info->data->wfsc_offset;
    bytes[sets] = Info->data->wfsc_bytes;
    sets++;
    nBytes += Info->data->wfsc_bytes;

  }

  if ( strstr( request, "slope")!=NULL ) {

    if ( *debug ) {
      printf("  DServ_Stream: Slope found\n");
      fflush(stdout);
    }

    /*
      Set the Info pointer to the DM_Info structure.
    */
    Info = DM_Info;

    offset[sets] = Info->data->slope_offset;
    bytes[sets] = Info->data->slope_bytes;
    sets++;
    nBytes += Info->data->slope_bytes;

  }

  if ( strstr( request, "rtr")!=NULL ) {

    if ( *debug ) {
      printf("  DServ_Stream: RTR found\n");
      fflush(stdout);
    }

    /*
      Set the Info pointer to the DM_Info structure.
    */
    Info = DM_Info;

    offset[sets] = Info->data->rtr_offset;
    bytes[sets] = Info->data->rtr_bytes;
    sets++;
    nBytes += Info->data->rtr_bytes;

  }

  if ( strstr( request, "cmd")!=NULL ) {

    if ( *debug ) {
      printf("  DServ_Stream: Cmd found\n");
      fflush(stdout);
    }

    /*
      Set the Info pointer to the DM_Info structure.
    */
    Info = DM_Info;

    offset[sets] = Info->data->cmd_offset;
    bytes[sets] = Info->data->cmd_bytes;
    sets++;
    nBytes += Info->data->cmd_bytes;

  }

  if ( strstr( request, "cur")!=NULL ) {

    if ( *debug ) {
      printf("  DServ_Stream: Cur found\n");
      fflush(stdout);
    }

    /*
      Set the Info pointer to the DM_Info structure.
    */
    Info = DM_Info;

    offset[sets] = Info->data->cur_offset;
    bytes[sets] = Info->data->cur_bytes;
    sets++;
    nBytes += Info->data->cur_bytes;

  }

  if ( strstr( request, "pos")!=NULL ) {

    if ( *debug ) {
      printf("  DServ_Stream: Pos found\n");
      fflush(stdout);
    }

    /*
      Set the Info pointer to the DM_Info structure.
    */
    Info = DM_Info;

    offset[sets] = Info->data->pos_offset;
    bytes[sets] = Info->data->pos_bytes;
    sets++;
    nBytes += Info->data->pos_bytes;

  }

  if ( strstr( request, "all")!=NULL ) {

    if ( *debug ) {
      printf("  DServ_Stream: All found\n");
      fflush(stdout);
    }

    /*
      Set the Info pointer to the DM_Info structure.
    */
    Info = DM_Info;

    sets = 0;
    offset[sets] = Info->data->all_offset;
    bytes[sets] = Info->data->all_bytes;
    sets++;
    nBytes = Info->data->all_bytes;

  }

  if ( sets <=0 ) {
    
    printf("  DServ_Stream: Unknown request: %s\n", request);
    fflush(stdout);
    return(-1);

  }

  /*
    Store the current frame and then check to see if the last frame
      filled is different from this value.  If last does not change
      then the loop is not closed. We will continuously send -2 to the
      client to indicate there are no new frames.  The client can decide
      to contiue to wait, or send a stop request.

    When last changes, we will start sending data.
  */
  first_frame = Info->data->last;
  wait = htonl(-2);

#if( TIMER )
  /*
    Get sytem time for the start of receiving data so we can calculate
      the rate.
  */
  systime = (struct timeval *)malloc( sizeof(struct timeval) );
  status = gettimeofday(systime, NULL);
  start_sec = systime->tv_sec;
  start_usec = systime->tv_usec;
  counter = 0;
#endif

  /*
    Check if last has changed
  */
  while ( Info->data->last == first_frame ) {

    /*
      Send the number of bytes to the client.  This long is in "host" format
      so a htonl() must be used.
    printf("Send wait (1)\n");
    status = Socket_StringWrite( sockfd, (char *)&wait, sizeof(long));
    if ( status !=  sizeof(long)) {
      if ( *debug ) {
	printf("  DServ_Stream: Error sending nBytes in Socket_StringWrite\n");
	fflush(stdout);
      }
      return(-1);
    }
    */

    /*
      Set wait time to see if a request has been sent by the client.  This
      is set to 1/10 of a second.
    */
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
 
    status = select( sockfd+1, &rfds, NULL, NULL, &tv);

    if ( status ) {

      /*
	Read request from the client
      */
      if ( Socket_StringRead( sockfd, request) ) {
	if ( *debug ) {
	  printf("  DServ_Stream: Error reading wait request in Socket_StringRead\n");
	  fflush(stdout);
	}
	return(-1);
      }

      /*
	Check that a request was sent
      */
      if ( strlen(request) <= 1 ) {
	printf("  DServ_Stream: Warning: wait request of zero length recieved\n");
	fflush(stdout);
	return(-1);
      }

      if ( *debug ) {
	printf("  DServ_Stream: wait request = %s\n", request);
	fflush(stdout);
      }

      /*
	Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
      */
      status = Socket_StringLowerCase(request);

      /*
	Process this wait request
      */
      if ( !strncmp( request, "stop", strlen(request)) ) {

	printf("  DServ_Stream: Stop request received\n");
	fflush(stdout);
	/*
	  Tell this server to close
	*/
	continueRunning = 0;
	nBytes_send = htonl(-1);
	break;

      } else if ( !strncmp( request, "status", strlen(request)) ) {

	printf("  DServ_Stream: Status request received\n");
	fflush(stdout);
	/*
	  Send a flag (nBytes) to the client
	*/
	status = Socket_StringWrite( sockfd, (char *)&wait, sizeof(long));
	if ( status !=  sizeof(long)) {
	  if ( *debug ) {
	    printf("  DServ_Stream: Error sending nBytes in Socket_StringWrite\n");
	    fflush(stdout);
	  }
	  return(-1);
	}

      } else {

	printf("  DServ_Stream: Unknown request %s\n", request);
	fflush(stdout);

      }

    }

  }

  /*
    Set the frame to start sending to either the next frame, or to
      the very first frame if the buffer was empty when we entered
      the routine.  This was we are guarenteed to start at the very first
      frame when the loop is close.
  */
  if ( (first_frame < 0) || (first_frame >= (NUM_DM_FRAMES - 1)) ) {

    /*
      The buffer was empty or we were at the end of the buffer
    */
    new_frame = 0;

  } else {

    /*
      The buffer was being filled, or was just started
    */
    new_frame = first_frame + 1;

  }

  /*
    Find pointer to the last filled frame.  This is where we start sending
     data.
  */
  DServ_DataPtr = Info->data->framePtr[new_frame];
  DServ_NumberPtr = DServ_DataPtr + 3;
  new_number = ntohl(*( (long *)DServ_NumberPtr));
  //  new_number = *( (long *)DServ_NumberPtr);
  //  printf(" first frame number = %ld\n", new_number);

  j = 0;

  request[0] = 0;

  while ( continueRunning ) {

    /*
      Send the number of bytes to the client.  This long is in "host" format
      so a htonl() must be used.
    */
    nBytes_send = htonl(nBytes);
    status = Socket_StringWrite( sockfd, (char *)&nBytes_send, sizeof(long));
    if ( status !=  sizeof(long)) {
      if ( *debug ) {
	printf("  DServ_Stream: Error sending nBytes in Socket_StringWrite\n");
	fflush(stdout);
      }
      return(-1);
    }	

    /*
      Send the frame number to the client.  The frameNumber is stored in the DServ
      memory in "network" format so a conversion is not needed.
    */
    status = Socket_StringWrite( sockfd, DServ_NumberPtr, sizeof(long));
    if ( status !=  sizeof(long)) {
      if ( *debug ) {
	printf("  DServ_Stream: Error sending new_number in Socket_StringWrite\n");
	fflush(stdout);
      }
      return(-1);
    }

    for ( i=0; i<sets; i++) {

      /*
	Set the point to the beginning of the frame, and then offset to the data
	set to send
      */
      dataPtr = DServ_DataPtr + offset[i];
    
      /*
	Send this frame to the client.  The frame data are stored in the DServ
	memory in "network" format so a conversion is not needed.
      */
      status = Socket_StringWrite( sockfd, (char *)dataPtr, bytes[i]);
      if ( status !=  bytes[i]) {
	if ( *debug ) {
	  printf("  DServ_Stream: Error sending dataPtr in Socket_StringWrite\n");
	  fflush(stdout);
	}
	return(-1);
      }	

    }

#if( TIMER )
    counter++;

    if ( counter > 500 ) {

      /*
	Calculate the data rate
      */
      status = gettimeofday(systime, NULL);
      end_sec = systime->tv_sec;
      end_usec = systime->tv_usec;
      total_time = (float) (end_sec-start_sec) + (float) (end_usec-start_usec) /1.0e6 ;
      printf("  DServ_Stream: Rate = %d Hz\n", (int) ((float) counter/total_time));
      fflush(stdout);
      start_sec = end_sec;
      start_usec = end_usec;
      counter = 0;
    }
#endif

    /*
      Increment and save the frame number so we can check when the next frame has
      been stored in memory
    */
    next_number = new_number + 1;

    /*
      Increment the pointer to the beginning of the next frame
    */
    new_frame = Info->data->nextFrame[new_frame];
    DServ_DataPtr = Info->data->framePtr[new_frame];
    DServ_NumberPtr = DServ_DataPtr + 3;
    //    printf(" next frame %d  %ld\n", new_frame, ntohl(*( (long *)DServ_NumberPtr)) );

    /*
      Set wait time to see if a request has been sent by the client.  This
      is set to 1/10 of a second.
    */
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
 
    /*
      Check to see if the next frame has been saved.  Loop the given number of times
      and then give up
    */
    count = 0;

    do {

      /*
	Check to see if the client has sent a request
      */
      status = select( sockfd+1, &rfds, NULL, NULL, &tv);

      if ( status ) {

	/*
	  Read request from the client
	*/
	if ( Socket_StringRead( sockfd, request) ) {
	  if ( *debug ) {
	    printf("  DServ_Stream: Error reading wait request in Socket_StringRead\n");
	    fflush(stdout);
	  }
	  return(-1);
	}

	/*
	  Check that a request was sent
	*/
	if ( strlen(request) < 1 ) {
	  printf("  DServ_Stream: Warning: wait request of zero length recieved\n");
	  fflush(stdout);
	}

	/*
	  Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
	*/
	status = Socket_StringLowerCase(request);

	/*
	  Process this wait request
	*/
	if ( !strncmp( request, "stop", strlen(request)) ) {

	  if ( *debug ) {
	    printf("  DServ_Stream: Stop request received\n");
	    fflush(stdout);
	  }

	  /*
	    Tell this server to close
	  */
	  continueRunning = 0;
	  nBytes_send = htonl(-1);
	  break;

	} else if ( !strncmp( request, "status", strlen(request)) ) {

	  if ( *debug ) {
	    printf("  DServ_Stream: Status request received\n");
	    fflush(stdout);
	  }

	  /*
	    Send a flag (nBytes) to the client
	  */
	  status = Socket_StringWrite( sockfd, (char *)&wait, sizeof(long));
	  if ( status !=  sizeof(long)) {
	    if ( *debug ) {
	      printf("  DServ_Stream: Error sending nBytes in Socket_StringWrite\n");
	      fflush(stdout);
	    }
	    return(-1);
	  }

	} else {

	  printf("  DServ_Stream: Unknown request %s\n", request);
	  fflush(stdout);

	}

      }

      new_number = ntohl(*( (long *)DServ_NumberPtr ));

      //      printf("%ld  %ld\n", new_number, next_number);
      if ( new_number >= next_number ) {

	/*
	  A new frame is available, so break this do loop and send the next frame
	*/
	count = 0;
	break;

      } else {

	/*
	  Increment the number of times we have checked for a new frame and have
	    not found one
	*/
	count++;

	/*
	  If a new frame is not available after the given number of tries, send the
	    client the appropriate flag (nBytes = -3) and then exit.
	*/
	if ( count > number_tries ) {

	  printf("  DServ_Stream: Did not receive new_number in %ld tries\n", number_tries);
	  fflush(stdout);

	  wait = htonl(-3);
	  status = Socket_StringWrite( sockfd, (char *)&wait, sizeof(long));
	  if ( status !=  sizeof(long)) {
	    if ( *debug ) {
	      printf("  DServ_Stream: Error sending nBytes in Socket_StringWrite\n");
	      fflush(stdout);
	    }
	    return(-1);
	  }

	  continueRunning = 0;
	  break;
	}

      }

    } while ( 1 );

  }

  return(0);
}
