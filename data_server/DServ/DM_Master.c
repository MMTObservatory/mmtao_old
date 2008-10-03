/*
  DM_Master.c

  This routine starts a data server which will serve data from the
    PC-Reconstructor (PCR).  The PCR data is stored in memory by
    another thread, with the location of this memory passed to this
    thread.

  This process will not collect or disperse data, but creeates threads
    to do this.

  This top level server lists to port DSERV_PORT_DM_MASTER.

  When a connection is made from a client, this routine creates a thread
    to deal with requests from the client.  The possible request are:

    1) wfsc: The routine DM_Process is called with DM_Info structure.
       With this routine it is possible to connect to the PCR via port
       PCR_WFSC and request streamed WFSC frames.  These data are stored in
       the appropriate memory (DM_MEM)
    2) quit: Exit only this thread.
    3) exit: Tell the main routine to exit. The mother will properly shutdown
       all created threads.

  Written  19nov05  DLM  Started with the VXWorks routine DServ.c

  Altered  12dec05  DLM  Changed from forking child processes to thread.  This
                           was need to allow common memory for clients and servers
			   so data from the PCR can be passed to clients.

  Altered   8may06  DLM  Started with DServ_Process.c
*/

#include <pthread.h>

#include "PCR.h"
#include "DServ.h"

/*================================================================================*
 * Start server to manage data servers
 *================================================================================*/
void *DM_Master( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  DServ_Thread_Info *Main_Info;
  DServ_Info *DM_Info;
  int *DServ_continueRunning;
  int *debug;

  int status, i;
  int newsockfd, saveflags;

  /*
    Client connection information
  */
  struct sockaddr_in cli_addr;
  int clilen;

  /*
    Structure to pass variable information to threads
  */
  pthread_t wfscThread[NUM_CONNECTIONS];
  DServ_Thread_Info Thread_Info[NUM_CONNECTIONS];

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (DServ_Thread_Info *)Passed_Info;
  DM_Info = Main_Info->info;
  DServ_continueRunning = Main_Info->run_flag;
  debug = Main_Info->debug;

  /*
    Set all Thread_Info[*].sockfd to -1 to indicate they are not in use
  */
  for ( i=0; i<NUM_CONNECTIONS; i++) {
    Thread_Info[i].sockfd = -1;
  }

  /*=====================================================================
    Open a non-blocking Socket to receive requests
    =======================================================================*/

  /*
    Open a TCP socket (an Internet stream socket) and bind
      the local address so a client can connect and send request
  */
  status = Socket_OpenServer( (Socket_Info *)DM_Info->socket, *debug);

  /*
    Set sockfd as non-blocking.  In this way we can see if the motor
      host (ao-topbox.mmto.arizona.edu) connects immediately.  If
      not, the host is most likely not available.  28dec04 DLM

    First, save the current flags for sockfd
  */
  saveflags=fcntl( DM_Info->socket->sockfd,F_GETFL,0);
  if(saveflags<0) {
    printf("  DM_Master: Error saving original configuration of sockfd\n");
    return( (void *)-1);
  }

  /*
    Now set this socket as non-blocking
  */
  if(fcntl( DM_Info->socket->sockfd,F_SETFL,saveflags|O_NONBLOCK)<0) {
    printf("  DM_Master: Error setting sockfd to non-blocking\n");
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
      printf("  DM_Master: Connection number = %d\n", i);

      if ( i<NUM_CONNECTIONS ) {

	/*
	  Restore new socket to blocking connection
	*/
	if(fcntl(newsockfd,F_SETFL,saveflags)<0) {
	  printf("  DM_Master: Error restoring newsockfd to blocking connection\n");
	  *DServ_continueRunning = 0;
	  continue;
	}

	/*
	  Fill in Thread_Info structure with WFSC information and this new connection
	*/
	Thread_Info[i].info = DM_Info;
	Thread_Info[i].sockfd = newsockfd;
	Thread_Info[i].run_flag = Main_Info->run_flag;
	Thread_Info[i].debug = Main_Info->debug;

      } else {

	printf("  DM_Master: Too many connections\n");
	*DServ_continueRunning = 0;
	continue;

      }

      /*
	Start Thread
      */
      status = pthread_create( &wfscThread[i], &attr, DM_Process, (void *)&Thread_Info[i]);
      if ( *debug ) {
	printf("  DM_Master: Starting thread\n");
      }
      if ( status ) {
	printf("  DM_Master: Error starting thread\n");
	close( newsockfd);
	Thread_Info[i].sockfd = -1;
	continue;
      }

    }

  } while ( *DServ_continueRunning );

  if ( *debug ) {
    printf("  DM_Master: Data Server Shutdown\n");
  }

  for ( i=0; i<NUM_CONNECTIONS; i++) {
    if (      Thread_Info[i].sockfd >= 0 ) {
      if ( *debug ) {
	printf("  DM_Master: Canceling thread %d\n", i);
      }
      pthread_cancel( wfscThread[i]);
    }
  }
  Socket_Close( (Socket_Info *)DM_Info->socket, *debug);

  return( (void *)0);
}
