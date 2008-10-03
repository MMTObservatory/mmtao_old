/*
  Info_Master.c

  This routine starts a Info data server which will serve infomation about
    the status of the AO loop, PCR, DServ and total AO system

  This process will not collect or disperse data, but creeates threads
    to do this.

  This top level server lists to port INFO_PORT.

  Written  19nov06  DLM  Started with the DServ_Master.c

  Altered  28dec07  DLM  Added setsockopt(... SO_LINGER ...) so sockets will be close
                           immediately on a close(sockfd) rather than try to send any
			   final data (socket in TIME_WAIT stat when a % netstat -tcp
			   command is issued).  Needed to fix the problem of no socket
			   resources available to state a new Info thread (error =
			   "Info_Master: Errors starting thread")

*/

#include <pthread.h>

#include "PCR.h"
#include "Info.h"

/*================================================================================*
 * Start server to manage Info servers
 *================================================================================*/
void *Info_Master( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  Info_Thread *Main_Info;
  Info_Struct *Info;
  int *continueRunning;
  int *debug;

  int status, i;
  int newsockfd, saveflags;

  /*
    Client connection information
  */
  struct sockaddr_in cli_addr;
  socklen_t clilen;
  struct linger Linger;

  /*
    Structure to pass variable information to threads
  */
  pthread_t infoThread[INFO_CONNECTIONS];
  Info_Thread Thread_Info[INFO_CONNECTIONS];

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (Info_Thread *)Passed_Info;
  Info = Main_Info->info;
  continueRunning = Main_Info->continueRunning;
  debug = Main_Info->debug;

  /*
    Set all Thread_Info[*].sockfd to -1 to indicate they are not in use
  */
  for ( i=0; i<INFO_CONNECTIONS; i++) {
    Thread_Info[i].sockfd = -1;
  }

  /*=====================================================================
    Open a non-blocking Socket to receive requests
    =======================================================================*/

  /*
    Open a TCP socket (an Internet stream socket) and bind
      the local address so a client can connect and send request
  */
  if ( Socket_OpenServer( (Socket_Info *)Info->socket, *debug) ) {
    printf("  Info_Master: Error opening Info_Master socket\n");
    printf("    Exiting...\n");
    fflush(stdout);
    return( (void *)-1);
  }

  /*
    Set sockfd as non-blocking.  In this way we can see if the motor
      host (ao-topbox.mmto.arizona.edu) connects immediately.  If
      not, the host is most likely not available.  28dec04 DLM

    First, save the current flags for sockfd
  */
  saveflags=fcntl( Info->socket->sockfd,F_GETFL,0);
  if(saveflags<0) {
    printf("  Info_Master: Error saving original configuration of sockfd\n");
    fflush(stdout);
    return( (void *)-1);
  }

  /*
    Now set this socket as non-blocking
  */
  if(fcntl( Info->socket->sockfd,F_SETFL,saveflags|O_NONBLOCK)<0) {
    printf("  Info_Master: Error setting sockfd to non-blocking\n");
    fflush(stdout);
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
    newsockfd = accept( Info->socket->sockfd, (struct sockaddr *) &cli_addr, &clilen );

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
      for ( i=0; i<INFO_CONNECTIONS; i++) {
	if (      Thread_Info[i].sockfd < 0 ) break;
      }

      if ( i<INFO_CONNECTIONS ) {

	/*
	  Restore new socket to blocking connection
	*/
	if(fcntl(newsockfd,F_SETFL,saveflags)<0) {
	  printf("  Info_Master: Error restoring newsockfd to blocking connection\n");
	  fflush(stdout);
	  *continueRunning = 0;
	  continue;
	}

	/*
	  Tell the system not to try to send any leftover data when the socket
	    is closed.  This should free up resources so we don't get a 
	    sockets in a TIME_WAIT state
	*/
	Linger.l_onoff = 1;
	Linger.l_linger = 0;
	setsockopt (newsockfd, SOL_SOCKET, SO_LINGER, (char *)&Linger,  sizeof(struct linger));

	/*
	  Fill in Thread_Info structure with WFSC information and this new connection
	*/
	Thread_Info[i].info = Main_Info->info;
	Thread_Info[i].sockfd = newsockfd;
	Thread_Info[i].continueRunning = Main_Info->continueRunning;
	Thread_Info[i].debug = Main_Info->debug;

      } else {

	printf("  Info_Master: Too many connections\n");
	fflush(stdout);
	continue;

      }

      /*
	Start Thread
      */
      status = pthread_create( &infoThread[i], &attr, Info_Process, (void *)&Thread_Info[i]);
      if ( *debug ) {
	printf("  Info_Master: Starting thread\n");
	fflush(stdout);
      }
      if ( status ) {
	printf("  Info_Master: Error starting thread\n");
	fflush(stdout);
	close( newsockfd);
	Thread_Info[i].sockfd = -1;
	continue;
      }

    }

  } while ( *continueRunning );

  if ( *debug ) {
    printf("  Info_Master: Info Server Shutdown\n");
    fflush(stdout);
  }

  for ( i=0; i<INFO_CONNECTIONS; i++) {
    if (      Thread_Info[i].sockfd >= 0 ) {
      if ( *debug ) {
	printf("  Info_Master: Canceling thread %d\n", i);
	fflush(stdout);
      }
      pthread_cancel( infoThread[i]);
    }
  }
  Socket_Close( (Socket_Info *)Info->socket, *debug);

  return( (void *)0);
}
