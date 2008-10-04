/*
  DServ_Master.c

  This routine starts a data server which will serve data from the
    PC-Reconstructor (PCR).  The PCR data is stored in memory by
    another thread, with the location of this memory passed to this
    thread.

  This process will not collect or disperse data, but creeates threads
    to do this.

  This top level server lists to port DSERV_PORT_WFSC_MASTER.

  When a connection is made from a client, this routine creates a thread
    to deal with requests from the client.  The possible request are:

    1) wfsc: The routine DServ_StartPCR is called with WFSC_Info structure.
       With this routine it is possible to connect to the PCR via port
       PCR_WFSC and request streamed WFSC frames.  These data are stored in
       the appropriate memory (WFSC_MEM)
    2) quit: Exit only this thread.
    3) exit: Tell the main routine to exit. The mother will properly shutdown
       all created threads.

  Written  19nov05  DLM  Started with the VXWorks routine DServ.c

  Altered  12dec05  DLM  Changed from forking child processes to thread.  This
                           was need to allow common memory for clients and servers
			   so data from the PCR can be passed to clients.

  Altered   8may06  DLM  Started with DServ_Process.c

  Altered  28dec07  DLM  Added setsockopt(... SO_LINGER ...) so sockets will be close
                           immediately on a close(sockfd) rather than try to send any
			   final data (socket in TIME_WAIT stat when a % netstat -tcp
			   command is issued).  Needed to fix the problem of no socket
			   resources available to state a new Info thread (error =
			   "Info_Master: Errors starting thread")
*/

#include <pthread.h>

#include "PCR.h"
#include "DServ.h"

/*================================================================================*
 * Start server to manage data servers
 *================================================================================*/
void *DServ_Master( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  DServ_Thread_Info *Main_Info;
  DServ_Info *DM_Info;
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
  pthread_t dservThread[NUM_CONNECTIONS];
  DServ_Thread_Info Thread_Info[NUM_CONNECTIONS];

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (DServ_Thread_Info *)Passed_Info;
  DM_Info = Main_Info->dm;
  continueRunning = Main_Info->DServ_DM_continueRunning;
  debug = Main_Info->DServ_DM_debug;

  /*
    Set all Thread_Info[*].sockfd to -1 to indicate they are not in use
  */
  for ( i=0; i<NUM_CONNECTIONS; i++) {
    Thread_Info[i].sockfd = -1;
    Thread_Info[i].frameNumber = (int *)malloc(sizeof(int));
  }

  /*=====================================================================
    Open a non-blocking Socket to receive requests
    =======================================================================*/

  /*
    Open a TCP socket (an Internet stream socket) and bind
      the local address so a client can connect and send request
  */
  if ( Socket_OpenServer( (Socket_Info *)DM_Info->socket, *debug) ) {
    printf("  DServ_Master: Error opening DServ_Master socket\n");
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
  saveflags=fcntl( DM_Info->socket->sockfd,F_GETFL,0);
  if(saveflags<0) {
    printf("  DServ_Master: Error saving original configuration of sockfd\n");
    fflush(stdout);
    return( (void *)-1);
  }

  /*
    Now set this socket as non-blocking
  */
  if(fcntl( DM_Info->socket->sockfd,F_SETFL,saveflags|O_NONBLOCK)<0) {
    printf("  DServ_Master: Error setting sockfd to non-blocking\n");
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
    newsockfd = accept( DM_Info->socket->sockfd, (struct sockaddr *) &cli_addr, &clilen );

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
      for ( i=0; i<NUM_CONNECTIONS; i++) {
	if (      Thread_Info[i].sockfd < 0 ) break;
      }

      if ( *debug ) {
	printf("  DServ_Master: Connection number = %d\n", i);
	fflush(stdout);
      }

      if ( i<NUM_CONNECTIONS ) {

	/*
	  Restore new socket to blocking connection
	*/
	if(fcntl(newsockfd,F_SETFL,saveflags)<0) {
	  printf("  DServ_Master: Error restoring newsockfd to blocking connection\n");
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
	Thread_Info[i].wfsc = Main_Info->wfsc;
	Thread_Info[i].dm = Main_Info->dm;
	Thread_Info[i].sockfd = newsockfd;
	*(Thread_Info[i].frameNumber) = -1;
	Thread_Info[i].DServ_WFSC_continueRunning = Main_Info->DServ_WFSC_continueRunning;
	Thread_Info[i].DServ_DM_continueRunning = Main_Info->DServ_DM_continueRunning;
	Thread_Info[i].DServ_WFSC_debug = Main_Info->DServ_WFSC_debug;
	Thread_Info[i].DServ_DM_debug = Main_Info->DServ_DM_debug;

      } else {

	printf("  DServ_Master: Too many connections\n");
	fflush(stdout);
	continue;

      }

      /*
	Start Thread
      */
      status = pthread_create( &dservThread[i], &attr, DServ_Process, (void *)&Thread_Info[i]);
      if ( *debug ) {
	printf("  DServ_Master: Starting thread\n");
	fflush(stdout);
      }
      if ( status ) {
	printf("  DServ_Master: Error starting thread\n");
	fflush(stdout);
	close( newsockfd);
	Thread_Info[i].sockfd = -1;
	continue;
      }

    }

  } while ( *continueRunning );

  if ( *debug ) {
    printf("  DServ_Master: Data Server Shutdown\n");
    fflush(stdout);
  }

  for ( i=0; i<NUM_CONNECTIONS; i++) {
    if (      Thread_Info[i].sockfd >= 0 ) {
      if ( *debug ) {
	printf("  DServ_Master: Canceling thread %d\n", i);
	fflush(stdout);
      }
      pthread_cancel( dservThread[i]);
    }
  }
  Socket_Close( (Socket_Info *)DM_Info->socket, *debug);

  return( (void *)0);
}
