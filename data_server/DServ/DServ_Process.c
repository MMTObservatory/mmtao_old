/*=======================================================================
 * DServ_Process.c
 *=======================================================================

  This routine processes the request for a WFSC spot image made by a client.

  The possible requests are:

    1) re-init: re-initialize the arrays need to hold raw and re-ordered data
    2) debug: turn debug output on or off (1/0)
    3) size: send the client the number of bytes in the WFSC frame
    4) single: send a single WFSC frame.  A long is sent to the client first with
               either the number of bytes of data to follow (positive value) or
	       an error number (negative value).
    5) stream: send a continuous stream of WFSC frames.  For each frame, a long is
               sent to the client first with either the number of bytes of data to
	       follow (positive value) or an error number (negative value).
    6) help, h, ?: print possible requests

  Written  24apr03  DLC

  Altered  12aug03  DLM Changed original server for WFSC counts and made it into
                          a WFSC spot image server

  Altered   8mar05  DLM  Cleaned up
                         Key words changed from Re-Init, Size, Spot
			   and Steam to Re-Init, Size, Single, Stream

  Altered  24feb06  DLM  Changed to check if a request was received by WFSC.c.
                           The change allows a "quit" or "debug" request to be
			   caught by WFSC.c.  Any other request is passed to
			   this routine for processing.
*/

#include <pthread.h>

#include "PCR.h"
#include "DServ.h"

/*=======================================================================
 * DServ_Process
 *=======================================================================*/
void *DServ_Process( void *Passed_Info)
{

  /*
    Local pointers to varible passed from the process which created this thread
   */
  DServ_Thread_Info *Main_Info;
  DServ_Info *WFSC_Info, *DM_Info;
  int *old_frameNumber;
  int *continueRunning;
  int *debug;

  /* Local variables */
  int sockfd;
  int status;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  int nBytes;

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (DServ_Thread_Info *)Passed_Info;
  WFSC_Info = Main_Info->wfsc;
  DM_Info = Main_Info->dm;
  sockfd = Main_Info->sockfd;
  old_frameNumber = Main_Info->frameNumber;
  continueRunning = Main_Info->DServ_DM_continueRunning;
  debug = Main_Info->DServ_DM_debug;

  /*********************************************************
   * Continue reading requests from client until the client
   *   send a quit command or client socket is lost.
   *********************************************************/
  do {

    /*
      Read request from the client
    */
    if ( Socket_StringRead( sockfd, request) ) {
      if ( *debug ) {
	printf("  DServ_Process: Error reading request in Socket_StringRead\n");
	fflush(stdout);
      }
      continueRunning = 0;
    }

    /*
      Check that a request was sent
    */
    if ( strlen(request) <= 1 ) {
      printf("  DServ_Process: Warning: request of zero length recieved\n");
      fflush(stdout);
      nBytes = htonl(-1);
      status = Socket_StringWrite( sockfd, (char *)&nBytes, sizeof(nBytes));
      continue;
    }

    if ( *debug ) {
      printf("  DServ_Process: request = %s\n", request);
      fflush(stdout);
    }

    /*
      Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
    */
    status = Socket_StringLowerCase(request);

    /*
      Check the request and do the proper thing
    */

    if ( !strncmp( request, "debug", strlen(request)) ) {

	/*
	  Read debug flag from client
	*/
      if ( Socket_StringRead( sockfd, request) ) {
	if ( *debug ) {
	  printf("  DServ_Process: Error reading debug flag in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }

      /*
	Trim control characters from the end (\n, \r, \t etc)
      */
      status = Socket_StringTrim(request);

      *debug = atol(request);

      printf("  DServ_Process: Debug turned %s\n", *debug ? "ON":"OFF");
      fflush(stdout);

    } else if ( !strncmp( request, "size", strlen(request)) ) {

      /*
	Read request from the client
      */
      if ( Socket_StringRead( sockfd, request) ) {
	if ( *debug ) {
	  printf("  DServ_Process: Error reading request in Socket_StringRead\n");
	  fflush(stdout);
	}
	continueRunning = 0;
	continue;
      }

      /*
	Check that a request was sent
      */
      if ( strlen(request) <= 1 ) {
	printf("  DServ_Process: Warning: request of zero length recieved\n");
	fflush(stdout);
	continue;
      }

      if ( *debug ) {
	printf("                           %s\n", request);
	fflush(stdout);
      }

      /*
	Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
      */
      status = Socket_StringLowerCase(request);

      /*
        Send the client the number of bytes that will be sent
      */
      if ( !strncmp( request, "spots", strlen(request)) ) {

	nBytes = htonl(DM_Info->data->wfsc_bytes);
	status = Socket_StringWrite( sockfd, (char *)&nBytes, sizeof(nBytes));
	if ( status != sizeof(nBytes) ) {
	  printf("  DServ_Process: Error in Socket_StringWrite sending number of WFSC bytes\n");
	  fflush(stdout);
	  continueRunning = 0;
	  continue;
	}

      } else if ( !strncmp( request, "slopes", strlen(request)) ) {

	nBytes = htonl(DM_Info->data->slope_bytes);
	status = Socket_StringWrite( sockfd, (char *)&nBytes, sizeof(nBytes));
	if ( status != sizeof(nBytes) ) {
	  printf("  DServ_Process: Error in Socket_StringWrite sending number of Slope bytes\n");
	  fflush(stdout);
	  continueRunning = 0;
	  continue;
	}

      } else if ( !strncmp( request, "rtr", strlen(request)) ) {

	nBytes = htonl(DM_Info->data->rtr_bytes);
	status = Socket_StringWrite( sockfd, (char *)&nBytes, sizeof(nBytes));
	if ( status != sizeof(nBytes) ) {
	  printf("  DServ_Process: Error in Socket_StringWrite sending number of RTR_Delta bytes\n");
	  fflush(stdout);
	  continueRunning = 0;
	  continue;
	}

      } else if ( !strncmp( request, "cmd", strlen(request)) ) {

	nBytes = htonl(DM_Info->data->cmd_bytes);
	status = Socket_StringWrite( sockfd, (char *)&nBytes, sizeof(nBytes));
	if ( status != sizeof(nBytes) ) {
	  printf("  DServ_Process: Error in Socket_StringWrite sending number of Command bytes\n");
	  fflush(stdout);
	  continueRunning = 0;
	  continue;
	}

      } else if ( !strncmp( request, "cur", strlen(request)) ) {

	nBytes = htonl(DM_Info->data->cur_bytes);
	status = Socket_StringWrite( sockfd, (char *)&nBytes, sizeof(nBytes));
	if ( status != sizeof(nBytes) ) {
	  printf("  DServ_Process: Error in Socket_StringWrite sending number of Current bytes\n");
	  fflush(stdout);
	  continueRunning = 0;
	  continue;
	}

      } else if ( !strncmp( request, "pos", strlen(request)) ) {

	nBytes = htonl(DM_Info->data->pos_bytes);
	status = Socket_StringWrite( sockfd, (char *)&nBytes, sizeof(nBytes));
	if ( status != sizeof(nBytes) ) {
	  printf("  DServ_Process: Error in Socket_StringWrite sending number of Position bytes\n");
	  fflush(stdout);
	  continueRunning = 0;
	  continue;
	}

      } else {

	printf("  DServ_Process: Unknown type of data %s\n", request);
	fflush(stdout);
	nBytes = htonl(-1);
	status = Socket_StringWrite( sockfd, (char *)&nBytes, sizeof(nBytes));
	if ( status != sizeof(nBytes) ) {
	  printf("  DServ_Process: Error in Socket_StringWrite sending -1 bytes\n");
	  fflush(stdout);
	  continueRunning = 0;
	  continue;
	}


      }

    } else if ( !strncmp( request, "single", strlen(request)) ) {

      /*
        Retrive one frame from either the WFSC or DM DServ memory and send it to the client.

	If an error ocurrs in the DServ_GetSingle routine it does not send anything to the
	  client.  Thus, here we must inform the client there was a problem with a return
	  value "status".

	=> right now it's only the DM buffer  16jul06  DLM
      */
      status = DServ_GetSingle( WFSC_Info, DM_Info, sockfd, old_frameNumber, debug);
      if ( status ) {
	nBytes = status;
	status = Socket_StringWrite( sockfd, (char *)&nBytes, sizeof(int));
	if ( status !=  sizeof(int)) {
	  if ( debug ) {
	    printf("  DServ_Process: Error sending nBytes in Socket_StringWrite\n");
	    fflush(stdout);
	  }
	}
      }

    } else if ( !strncmp( request, "stream", strlen(request)) ) {

      /*
        Send a stream of data (all frames) from the  WFSC or DM DServ memory to the client.

	At the moment, the only way to stop the flow of data is for the client to close the
	  connection.  In this case DServ_Stream returns an error and this server process is
	  stopped.  We should be able to send a stop request from the client to this server
	  and not have to close the connection.  3sep06 DLM

	=> right now it's only the DM buffer  16jul06  DLM
      */
      status = DServ_Stream( WFSC_Info, DM_Info, sockfd, debug);
      /*
      if ( status ) {
	nBytes = status;
	status = Socket_StringWrite( sockfd, (char *)&nBytes, sizeof(int));
	if ( status !=  sizeof(int)) {
	  if ( debug ) {
	    printf("  DServ_Process: Error sending nBytes in Socket_StringWrite\n");
	    fflush(stdout);
	  }
	}
      }
      */
      continueRunning = 0;
      continue;

    } else if ( (!strncmp( request, "help", strlen(request))) ||
		(!strncmp( request, "h", strlen(request))) ||
		(!strncmp( request, "?", strlen(request))) ) {

      status = DServ_SendHelp( sockfd);
      if ( status != strlen(reply) ) {
	printf("  DServ_Process: Error returning help options\n");
	fflush(stdout);
	continueRunning = 0;
	continue;
      }

    } else if ( !strncmp( request, "quit", strlen(request)) ) {

      continueRunning = 0;
      continue;

    } else {

      if ( *debug ) {
	printf ("  DServ_Process: unkown request = %s\n", request);
	fflush(stdout);
      }

    }

    request[0] = 0;

  } while ( continueRunning );

  /*
    Signal DServ_Master that this thread has exited
  */
  close( Main_Info->sockfd);
  Main_Info->sockfd = -1;
  
  pthread_exit(NULL);

  return( (void *)0);

}
