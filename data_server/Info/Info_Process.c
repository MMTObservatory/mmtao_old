/*=======================================================================
 * Info_Process.c
 *=======================================================================

  This routine processed the request for information made by a client.

  Written  24apr03  DLM Started with DServ_Process and used parts from
                          vxworks/Info/Info_Process.c
*/

#include <pthread.h>

#include "PCR.h"
#include "Info.h"

/*=======================================================================
 * Info_Process
 *=======================================================================*/
void *Info_Process( void *Passed_Info)
{

  /*
    Local pointers to varible passed from the process which created this thread
   */
  Info_Thread *Main_Info;
  Info_Struct *Info;
  Info_Entry *List;
  Info_Array *Seeing;
  Info_Array *Strehl;
  long list_length;
  int *continueRunning;
  int *debug;

  /* Local variables */
  int sockfd;
  int status;
  char request[STRING_LENGTH];

  /* Seeing Parameter info */
  long *Seeing_Positions;
  long Seeing_Count;

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (Info_Thread *)Passed_Info;
  Info = Main_Info->info;
  List = Info->list;
  list_length = Info->length;
  Seeing = Info->seeing;
  Strehl = Info->strehl;
  sockfd = Main_Info->sockfd;
  continueRunning = Main_Info->continueRunning;
  debug = Main_Info->debug;

  /*
    Initialize the positions of the Seeing Parameters
  */
  status = Info_SeeingPositions( Info, &Seeing_Positions, &Seeing_Count, *debug);

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
	printf("  Info_Process: Error reading request in Socket_StringRead\n");
	fflush(stdout);
      }
      continueRunning = 0;
      continue;
    }

    /*
      Check that a request was sent
    */
    if ( strlen(request) <= 1 ) {
      if ( *debug ) {
	printf("  Info_Process: Warning: request of zero length recieved\n");
	fflush(stdout);
      }
      continueRunning = 0;
      continue;
    }

    if ( *debug ) {
      printf("  Info_Process: Request = %s\n", request);
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
	  printf("  Info_Process: Error reading debug flag in Socket_StringRead\n");
	  fflush(stdout);
	}
	continueRunning = 0;
	continue;
      }

      /*
	Trim control characters from the end (\n, \r, \t etc)
      */
      status = Socket_StringTrim(request);

      *debug = atol(request);

      printf("  Info_Process: Debug turned %s\n", *debug ? "ON":"OFF");
      fflush(stdout);

    } else if ( !strncmp( request, "set", strlen("set")) ) {

      /*
        Set an Info Parameter
      */
      status = Info_SetParameter( sockfd, List, list_length, Seeing, Strehl, *debug);
      if ( status ) {
	printf("  Info_Process: Error in Info_SetParameter\n");
	fflush(stdout);
	continueRunning = 0;
	continue;
      }

    } else if ( !strncmp( request, "get", strlen("get")) ) {

      /*
	Send the value of the requested Info Parameter
      */
//      status = Info_GetParameter( sockfd, List, list_length, *debug);
      if ( status ) {
	printf("  Info_Process: Error in Info_GetParameter\n");
	fflush(stdout);
	continue;
      }

    } else if ( !strncmp( request, "all", strlen("all")) ) {

      /*
        Send the client all Info Parameters
      */
      status = Info_SendAll( sockfd, List, list_length);
      if ( status ) {
	printf("  Info_Process: Error in Info_SendAll\n");
	fflush(stdout);
	continueRunning = 0;
      }

    } else if ( !strncmp( request, "seeing", strlen("seeing")) ) {

      /*
        Send the client seeing Info Parameters
      */
      status = Info_SendSeeing( sockfd, List, list_length, Seeing_Positions, Seeing_Count);
      if ( status ) {
	printf("  Info_Process: Error in Info_SendSeeing\n");
	fflush(stdout);
	continueRunning = 0;
      }

    } else if ( !strncmp( request, "see_vals", strlen("see_vals")) ) {

      /*
        Send the client all seeing values Info Parameters
      */
//      status = Info_ServerSeeingArray( sockfd, Seeing);
      if ( status ) {
	printf("  Info_Process: Error in Info_ServerSeeing\n");
	fflush(stdout);
      }

    } else if ( !strncmp( request, "strehl_vals", strlen("strehl_vals")) ) {

      /*
        Send the client all strehl values Info Parameters
      */
//      status = Info_ServerSeeingArray( sockfd, Strehl);
      if ( status ) {
	printf("  Info_Process: Error in Info_ServerSeeing\n");
	fflush(stdout);
      }

    } else if ( !strncmp( request, "reset_seeing", strlen("reset_seeing")) ) {

      /*
        Reset the Seeing Array
      */
      Seeing->position = 0;
      Seeing->total_number = 0;

    } else if ( !strncmp( request, "reset_strehl", strlen("reset_strehl")) ) {

      /*
        Reset the Strehl Array
      */
      Strehl->position = 0;
      Strehl->total_number = 0;

    } else if ( (!strncmp( request, "help", strlen(request))) ||
		(!strncmp( request, "h", strlen(request))) ||
		(!strncmp( request, "?", strlen(request))) ) {

      status = Info_SendHelp( sockfd);
      if ( status ) {
	printf("  Info_Process: Error returning help options\n");
	fflush(stdout);
	continueRunning = 0;
      }

    } else if ( !strncmp( request, "quit", strlen(request)) ) {

      continueRunning = 0;

    } else {

      if ( *debug ) {
	printf ("  Info_Process: unkown request = %s\n", request);
	fflush(stdout);
      }

    }

    request[0] = 0;

  } while ( continueRunning );

  /*
    Signal Info_Master that this thread has exited
  */
  close( Main_Info->sockfd);
  Main_Info->sockfd = -1;
  
  pthread_exit(NULL);

  return( (void *)0);

}
