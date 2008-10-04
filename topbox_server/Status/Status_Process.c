/*=======================================================================
 * Status_Process.c
 *=======================================================================

  This routine processed the request for information made by a client.

  Written  13aug07  DLM Started with Info_Process from pcr_server

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"

/*=======================================================================
 * Status_Process
 *=======================================================================*/
void *Status_Process( void *Passed_Info)
{

  /*
    Local pointers to varible passed from the process which created this thread
   */
  Status_Thread_Info *Main_Info;
  Status_Struct *Status;
  Status_Entry *List;
  int list_length;
  int *continueRunning;
  int *debug;

  /* Local variables */
  int sockfd;
  int status;
  char request[STRING_LENGTH];

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (Status_Thread_Info *)Passed_Info;
  Status = Main_Info->status;
  List = Status->list;
  list_length = Status->length;
  sockfd = Main_Info->sockfd;
  continueRunning = Main_Info->continueRunning;
  debug = Main_Info->debug;

  /*********************************************************
   * Continue reading requests from client until the client
   *   send a quit command or client socket is lost.
   *********************************************************/
  do {

    /*
      Read request from the client
    */
    if ( Socket_StringRead( sockfd, request) ) {
      if ( *debug ) printf("  Status_Process: Error reading request in Socket_StringRead\n");
      continueRunning = 0;
    }

    /*
      Check that a request was sent
    */
    if ( strlen(request) <= 1 ) {
      if ( *debug )
	printf("  Status_Process: Warning: request of zero length recieved\n");
      continue;
    }

    if ( *debug ) {
      printf("  Status_Process: Request = %s\n", request);
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
	if ( *debug ) printf("  Status_Process: Error reading debug flag in Socket_StringRead\n");
	continue;
      }

      /*
	Trim control characters from the end (\n, \r, \t etc)
      */
      status = Socket_StringTrim(request);

      *debug = atol(request);

      printf("  Status_Process: Debug turned %s\n", *debug ? "ON":"OFF");

    } else if ( !strncmp( request, "set", strlen("set")) ) {

      /*
        Set an Status Parameter
      */
      //      status = Status_SetParameter( sockfd, List, list_length, Seeing, Strehl, *debug);
      if ( status ) {
	printf("  Status_Process: Error in Status_SetParameter\n");
	continue;
      }

    } else if ( !strncmp( request, "get", strlen("get")) ) {

      /*
	Send the value of the requested Status Parameter
      */
//      status = Status_GetParameter( sockfd, List, list_length, *debug);
      if ( status ) {
	printf("  Status_Process: Error in Status_GetParameter\n");
	continue;
      }

    } else if ( !strncmp( request, "all", strlen("all")) ) {

      /*
        Send the client all Status Parameters
      */
      status = Status_SendAll( sockfd, List, list_length);
      if ( status ) {
	printf("  Status_Process: Error in Status_SendAll\n");
      }

    } else if ( (!strncmp( request, "help", strlen(request))) ||
		(!strncmp( request, "h", strlen(request))) ||
		(!strncmp( request, "?", strlen(request))) ) {

      status = Status_SendHelp( sockfd);
      if ( status ) {
	printf("  Status_Process: Error returning help options\n");
	continueRunning = 0;
      }

    } else if ( !strncmp( request, "quit", strlen(request)) ) {

      continueRunning = 0;

    } else {

      if ( *debug ) {
	printf ("  Status_Process: Unkown request = %s\n", request);
      }

    }

    request[0] = 0;

  } while ( continueRunning );

  close( sockfd);
  /*
    Signal Status_Master that this thread has exited
  */
  Main_Info->sockfd = -1;
  
  pthread_exit(NULL);

  return( (void *)0);

}
