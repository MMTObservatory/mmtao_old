/*=======================================================================
 * WFSC_Process.c
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
 * WFSC_Process
 *=======================================================================*/
void *WFSC_Process( void *Passed_Info)
{

  /*
    Local pointers to varible passed from the process which created this thread
   */
  DServ_Thread_Info *Main_Info;
  DServ_Info *WFSC_Info;
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
  sockfd = Main_Info->sockfd;
  continueRunning = Main_Info->DServ_WFSC_continueRunning;
  debug = Main_Info->DServ_WFSC_debug;

  /*********************************************************
   * Continue reading requests from client until the client
   *   send a quit command or client socket is lost.
   *********************************************************/
  do {

    /*
      Read request from the client
    */
    if ( Socket_StringRead( sockfd, request) ) {
      if ( *debug ) printf("  WFSC_Process: Error reading request in Socket_StringRead\n");
      continueRunning = 0;
      continue;
    }

    /*
      Check that a request was sent
    */
    if ( strlen(request) <= 1 ) {
      printf("  WFSC_Process: Warning: request of zero length recieved\n");
      continue;
    }

    if ( *debug ) {
      printf("  WFSC_Process: request = %s\n", request);
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
	if ( *debug ) printf("  WFSC_Process: Error reading debug flag in Socket_StringRead\n");
	continue;
      }

      /*
	Trim control characters from the end (\n, \r, \t etc)
      */
      status = Socket_StringTrim(request);

      *debug = atol(request);

      printf("  WFSC_Process: Debug turned %s\n", *debug ? "ON":"OFF");

    } else if ( !strncmp( request, "size", strlen(request)) ) {

      /*
        Send the client the number of bytes that will be sent
      */
      nBytes = htonl(WFSC_Info->data->wfsc_bytes);
      status = Socket_StringWrite( sockfd, (char *)&nBytes, sizeof(nBytes));
      if ( status != sizeof(nBytes) ) {
	printf("  WFSC_Process: Error in Socket_StringWrite sending number of bytes\n");
	continueRunning = 0;
	continue;
      }

    } else if ( !strncmp( request, "single", strlen(request)) ) {

      /*
        Retrive one WFSC frame from the DDS and send it to the client
      */
      status = WFSC_GetSingle( WFSC_Info, sockfd, *debug);

    } else if ( !strncmp( request, "stream", strlen(request)) ) {

      //      status = WFSC_Stream( WFSC_Info, *debug);
      continueRunning = 0;

    } else if ( (!strncmp( request, "help", strlen(request))) ||
		(!strncmp( request, "h", strlen(request))) ||
		(!strncmp( request, "?", strlen(request))) ) {

      strcpy( reply, "Possible requests:\n");
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  WFSC_Process: Error returning help options\n");
	continueRunning = 0;
	continue;
      }

      strcpy( reply, "  re_init\\n\n");
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  WFSC_Process: Error returning help options\n");
	continueRunning = 0;
	continue;
      }

      strcpy( reply, "  debug\\n<0/1>\\n\n");
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  WFSC_Process: Error returning help options\n");
	continueRunning = 0;
	continue;
      }

      strcpy( reply, "  size\\n\n");
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  WFSC_Process: Error returning help options\n");
	continueRunning = 0;
	continue;
      }

      strcpy( reply, "  single\\n\n");
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  WFSC_Process: Error returning help options\n");
	continueRunning = 0;
	continue;
      }

      strcpy( reply, "  stream\\n\n");
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  WFSC_Process: Error returning help options\n");
	continueRunning = 0;
	continue;
      }

      strcpy( reply, "  help\\n\n");
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  WFSC_Process: Error returning help options\n");
	continueRunning = 0;
	continue;
      }

      strcpy( reply, "  h\\n\n");
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  WFSC_Process: Error returning help options\n");
	continueRunning = 0;
	continue;
      }

      strcpy( reply, "  ?\\n\n");
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  WFSC_Process: Error returning help options\n");
	continueRunning = 0;
	continue;
      }

      strcpy( reply, "  quit\\n\n");
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  WFSC_Process: Error returning help options\n");
	continueRunning = 0;
	continue;
      }

    } else if ( !strncmp( request, "quit", strlen(request)) ) {

      continueRunning = 0;
      continue;

    } else {

      if ( *debug ) {
	printf ("  WFSC_Process: unkown request = %s\n", request);
      }

    }

    request[0] = 0;

  } while ( continueRunning );

  /*
    Signal WFSC_Master that this thread has exited
  */
  close( Main_Info->sockfd);
  Main_Info->sockfd = -1;
  
  pthread_exit(NULL);

  return( (void *)0);

}
