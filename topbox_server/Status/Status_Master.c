/*
  Status_Master.c

  This routine starts a Status data server which will serve infomation about
    the status of devices in the Topbox

  This process will not collect or disperse data, but creeates threads
    to do this.

  This top level server lists to port STATUT_PORT.

  Written  13aug07  DLM  Started with the Info_Master.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"

/*================================================================================*
 * Start server to manage Status servers
 *================================================================================*/
void *Status_Master( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  Status_Thread_Info *Main_Info;
  Status_Struct *Status;
  int *continueRunning;
  int *debug;

  int status, i;
  int newsockfd, saveflags;

  /*
    Client connection information
  */
  struct sockaddr_in cli_addr;
  socklen_t clilen;

  /*
    Structure to pass variable information to threads
  */
  pthread_t statusThread[STATUS_CONNECTIONS];
  Status_Thread_Info Thread_Info[STATUS_CONNECTIONS];

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (Status_Thread_Info *)Passed_Info;
  Status = Main_Info->status;
  continueRunning = Main_Info->continueRunning;
  debug = Main_Info->debug;

  /*
    Set all Thread_Info[*].sockfd to -1 to indicate they are not in use
  */
  for ( i=0; i<STATUS_CONNECTIONS; i++) {
    Thread_Info[i].sockfd = -1;
  }

  /*=====================================================================
    Open a non-blocking Socket to receive requests
    =======================================================================*/

  /*
    Open a TCP socket (an Internet stream socket) and bind
      the local address so a client can connect and send request
  */
  if ( Socket_OpenServer( (Socket_Info *)Status->socket, *debug) ) {
    printf("  Status_Master: Error opening Status_Master socket\n");
    printf("    Exiting...\n");
    return( (void *)-1);
  }

  /*
    Set sockfd as non-blocking.  In this way we can see if the motor
      host (ao-topbox.mmto.arizona.edu) connects immediately.  If
      not, the host is most likely not available.  28dec04 DLM

    First, save the current flags for sockfd
  */
  saveflags=fcntl( Status->socket->sockfd,F_GETFL,0);
  if(saveflags<0) {
    printf("  Status_Master: Error saving original configuration of sockfd\n");
    return( (void *)-1);
  }

  /*
    Now set this socket as non-blocking
  */
  if(fcntl( Status->socket->sockfd,F_SETFL,saveflags|O_NONBLOCK)<0) {
    printf("  Status_Master: Error setting sockfd to non-blocking\n");
    return( (void *)-1);
  }

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  do {

    /*
      Listen to this socket.  This function returns when a client
      has connected.  This is an example of a concurrent server.
    */
    clilen = sizeof( cli_addr );
    newsockfd = accept( Status->socket->sockfd, (struct sockaddr *) &cli_addr, &clilen );

    /*
      Check to see if a connection has been made
    */
    if( newsockfd < 0 ) {

      /*
	No connection
      */
      usleep(100000);
      continue;

    } else {

      /*
	Connection:
	  1) Find an unused Thread_Info structure
	  1) Fill in the Thread_Info structure for this new socket
	  2) Start a thread to deal with the client request
      */
      for ( i=0; i<STATUS_CONNECTIONS; i++) {
	if (      Thread_Info[i].sockfd < 0 ) break;
      }

      if ( i<STATUS_CONNECTIONS ) {

	/*
	  Restore new socket to blocking connection
	*/
	if(fcntl(newsockfd,F_SETFL,saveflags)<0) {
	  printf("  Status_Master: Error restoring newsockfd to blocking connection\n");
	  *continueRunning = 0;
	  continue;
	}

	/*
	  Fill in Thread_Info structure with WFSC information and this new connection
	*/
	Thread_Info[i].status = Main_Info->status;
	Thread_Info[i].sockfd = newsockfd;
	Thread_Info[i].continueRunning = Main_Info->continueRunning;
	Thread_Info[i].debug = Main_Info->debug;

      } else {

	printf("  Status_Master: Too many connections\n");
	continue;

      }

      /*
	Start Thread
      */
      status = pthread_create( &statusThread[i], &attr, Status_Process, (void *)&Thread_Info[i]);
      if ( *debug ) {
	printf("  Status_Master: Starting thread\n");
      }
      if ( status ) {
	printf("  Status_Master: Error starting thread\n");
	Thread_Info[i].sockfd = -1;
	close( newsockfd);
	continue;
      }

    }

  } while ( *continueRunning );

  if ( *debug ) {
    printf("  Status_Master: Status Server Shutdown\n");
  }

  for ( i=0; i<STATUS_CONNECTIONS; i++) {
    if (      Thread_Info[i].sockfd >= 0 ) {
      if ( *debug ) {
	printf("  Status_Master: Canceling thread %d\n", i);
      }
      pthread_cancel( statusThread[i]);
    }
  }
  Socket_Close( (Socket_Info *)Status->socket, *debug);

  return( (void *)0);
}
