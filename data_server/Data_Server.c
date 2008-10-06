/*
  PCR_Server.c

  This routine starts a data server which will serve data from the
    PC-Reconstructor (PCR).  All data from data (WFSC, Slopes, Commands,
    Positions, Current) from the PCR will be stored in memory and
    passed on to clients as requested.

  This process will not collect or disperse data, but fork processed
    to do this.  This routine sets up memory and passed addressed to
    its child processes to all have access to parameters and data.

  This top level server lists to port DSERV_MASTER.

  When a connection is made from a client, this routine creates a thread
    to deal with requests from the client.  The possible request are:

    1) wfsc: The routine DServ_StartPCR is called with WFSC_Info structure.
       With this routine it is possible to connect to the PCR via port
       PCR_WFSC and request streamed WFSC frames.  These data are stored in
       the appropriate memory (WFSC_MEM)
    2) dm: The routine DServ_StartPCR is called with DM_Info structure.
       With this routine it is possible to connect to the PCR via port
       PCR_WFSC and request streamed DM frames.  These data are stored in
       the appropriate memory (DM_MEM)
    3) quit: Tell the main routine to exit. The mother will properly shutdown
       all created threads.

  Written  19nov05  DLM  Started with the VXWorks routine DServ.c

  Altered  12dec05  DLM  Changed from forking child processes to thread.  This
                           was need to allow common memory for clients and servers
			   so data from the PCR can be passed to clients.

  Altered  19nov06  DLM  Added a thread for the Info Server

  Altered  28dec06  DLM  Changed name from DServ.c.  Changed directory structure

  Altered   2apr07  DLM  Added mutex lock and unlock to keep from having
                           collisions between different threads trying to
			   use the PCR_CMD port.
*/

#include "PCR.h"
#include "Server_Ports.h"

#include "DServ.h"
#include "Status.h"
#include "Error.h"

#include "InfoList.h"

/*
  Make the mutex variable PCR_CMD_Lock a global variable so all threads
    can see it.
*/
pthread_mutex_t PCR_CMD_Lock = PTHREAD_MUTEX_INITIALIZER;

/*================================================================================*
 * Start the PCR Server to manage data servers
 *================================================================================*/
int main( )
{
  int debug = 0;
  int start_debug = 1;
  int status, i;
  int newsockfd, saveflags;
  int local_debug = 0;
  char filename[STRING_LENGTH];

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  /*
    Client connection information
  */
  struct sockaddr_in cli_addr;
  socklen_t clilen;

  /*
    Master Socket Structures (listen for clients requests)
  */
  Socket_Info Master_Socket;

  /*
    PCR Socket Structures (connect to PCR as a client)
  */
  Socket_Info PCR_CMD_Socket;
  Socket_Info PCR_WFSC_Socket;
  Socket_Info PCR_DM_Socket;
  Socket_Info PCR_Error_Socket;

  /*
    DServ Socket Structures (serve data to clients)
  */
  Socket_Info WFSC_Socket;
  Socket_Info DM_Socket;

  /*
    Info Socket Structures (serve information to clients)
  */
  Socket_Info Info_Socket;

  /*
    WFSC, and DM buffer variables
  */
  DServ_Data WFSC_Data;
  DServ_Data DM_Data;

  /*
    Full PCR information structures
  */
  DServ_Info PCR_CMD_Info;
  DServ_Info PCR_WFSC_Info;
  DServ_Info PCR_DM_Info;
  DServ_Info PCR_Error_Info;

  /*
    Full Info information structures.
  */
  Info_Struct Info_Info;

  /*
    Full DServ information structures
  */
  DServ_Info WFSC_Info;
  DServ_Info DM_Info;

  /*
    Global flag to send shutdown signal to all threads, plus
      run flags for individual threads
  */
  int *DServ_continueRunning;
  int *PCR_CMD_continueRunning;
  int *PCR_WFSC_continueRunning;
  int *PCR_DM_continueRunning;
  int *PCR_Error_continueRunning;
  int *DServ_WFSC_continueRunning;
  int *DServ_DM_continueRunning;
  int *Info_continueRunning;
  int *Status_continueRunning;

  /*
    Old Global flags that should be removed (still used in PCR_Process.c)
  */
  int *PCR_WFSC_running;
  int *PCR_DM_running;

  /*
    Global flags that data is being received form the given PCR port at greater than
      10 Hz
  */
  int *PCR_WFSC_receiving_data;
  int *PCR_DM_receiving_data;

  /*
    Global value that containing the rate at which we are receiving date from the given PCR port
  */
  int *PCR_WFSC_rate;
  int *PCR_DM_rate;

  /*
    Global flags indicating the specific Threads are running
  */
  int *PCR_Client_running;
  int *DServ_WFSC_running;
  int *DServ_DDS_running;
  int *Info_running;

  /*
    Global flag to send reset signal to specific threads
  */
  int *PCR_WFSC_reset;
  int *PCR_DM_reset;

  /*
    Global flag to signal all threads to output debug information,
      plus debug flags for individual threads.
  */
  int *DServ_debug;
  int *PCR_CMD_debug;
  int *PCR_WFSC_debug;
  int *PCR_DM_debug;
  int *PCR_Error_debug;
  int *DServ_WFSC_debug;
  int *DServ_DM_debug;
  int *Info_debug;
  int *Status_debug;

  /*
    Structure to pass variable information to threads
  */
  pthread_t pcrThread[NUM_CONNECTIONS];
  PCR_Thread_Info PCR_Info[NUM_CONNECTIONS];
  pthread_t errorThread;
  Status_Thread Error;
  pthread_t wfscThread;
  DServ_Thread_Info DServ_WFSC_Info;
  pthread_t dservThread;
  DServ_Thread_Info DServ_DM_Info;
  pthread_t infoThread;
  Info_Thread Info;
  pthread_t statusThread;
  Status_Thread Status;
  PCR_Thread_Client WFSC_Thread_Client, DM_Thread_Client;
  pthread_t pcr_wfscThread, pcr_dmThread;

  /*
    Fill in information about the Master PCR Server

  strcpy ( Master_Socket.name, "DServ_Cmd");
  Master_Socket.port = DSERV_CMD_PORT;
  Master_Socket.sockfd = -1;
  Master_Socket.connected = 0;
  Master_Socket.error = 0;
  */
  /*
    Fill in information for PCR clients
  */
  status = Socket_Setup( (Socket_Info *)&PCR_CMD_Socket, "PCR_CMD", debug);
  if ( status ) {
    printf("  PCR_Server: Error in Socket_Setup for PCR_CMD\n");
    fflush(stdout);
    return(-1);
  }

  status = Socket_Setup( (Socket_Info *)&PCR_WFSC_Socket, "PCR_WFSC", debug);
  if ( status ) {
    printf("  PCR_Server: Error in Socket_Setup for PCR_WFSC\n");
    fflush(stdout);
    return(-1);
  }

  status = Socket_Setup( (Socket_Info *)&PCR_DM_Socket, "PCR_DM", debug);
  if ( status ) {
    printf("  PCR_Server: Error in Socket_Setup for PCR_DM\n");
    fflush(stdout);
    return(-1);
  }

  status = Socket_Setup( (Socket_Info *)&PCR_Error_Socket, "PCR_Error", debug);
  if ( status ) {
    printf("  PCR_Server: Error in Socket_Setup for PCR_Error\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Fill in information for Master server
  */
  status = Socket_Setup( (Socket_Info *)&Master_Socket, "Master", debug);
  if ( status ) {
    printf("  PCR_Server: Error in Socket_Setup for Master\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Fill in information for DServ servers
  */
  status = Socket_Setup( (Socket_Info *)&WFSC_Socket, "WFSC", debug);
  if ( status ) {
    printf("  PCR_Server: Error in Socket_Setup for WFSC\n");
    fflush(stdout);
    return(-1);
  }

  status = Socket_Setup( (Socket_Info *)&DM_Socket, "DM", debug);
  if ( status ) {
    printf("  PCR_Server: Error in Socket_Setup for DM\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Fill in information for Info servers
  */
  status = Socket_Setup( (Socket_Info *)&Info_Socket, "Info", debug);
  if ( status ) {
    printf("  PCR_Server: Error in Socket_Setup for Info\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Put PCR Socket and Data structures together
  */
  PCR_CMD_Info.socket = &PCR_CMD_Socket;
  PCR_WFSC_Info.socket = &PCR_WFSC_Socket;
  PCR_WFSC_Info.data = &WFSC_Data;
  PCR_DM_Info.socket = &PCR_DM_Socket;
  PCR_DM_Info.data = &DM_Data;
  PCR_Error_Info.socket = &PCR_Error_Socket;

  /*
    Put DServ Socket and Data structures together
  */
  WFSC_Info.socket = &WFSC_Socket;
  WFSC_Info.data = &WFSC_Data;
  DM_Info.socket = &DM_Socket;
  DM_Info.data = &DM_Data;

  /*
    Put Info Socket and List structures together
  */
  Info_Info.socket = &Info_Socket;
  Info_Info.list = Info_List;
  Info_Info.length = Info_ListLength;
  Info_Info.seeing = &Seeing;
  Info_Info.strehl = &Strehl;

  /*
    Create buffer memory and fill in DServ_Data structure
  */
  status = DServ_InitWFSC( WFSC_Info.data, local_debug);
  status = DServ_InitDM( DM_Info.data, local_debug);

  /*
    Allocate and set flags that are passed, used and change by threads
  */
  DServ_continueRunning = (int *)malloc( sizeof(int));
  *DServ_continueRunning = -1;
  PCR_CMD_continueRunning = (int *)malloc( sizeof(int));
  *PCR_CMD_continueRunning = -1;
  PCR_WFSC_continueRunning = (int *)malloc( sizeof(int));
  *PCR_WFSC_continueRunning = -1;
  PCR_DM_continueRunning = (int *)malloc( sizeof(int));
  *PCR_DM_continueRunning = -1;
  PCR_Error_continueRunning = (int *)malloc( sizeof(int));
  *PCR_Error_continueRunning = -1;
  DServ_WFSC_continueRunning = (int *)malloc( sizeof(int));
  *DServ_WFSC_continueRunning = -1;
  DServ_DM_continueRunning = (int *)malloc( sizeof(int));
  *DServ_DM_continueRunning = -1;
  Info_continueRunning = (int *)malloc( sizeof(int));
  *Info_continueRunning = -1;
  Status_continueRunning = (int *)malloc( sizeof(int));
  *Status_continueRunning = -1;

  /* Should be removed  (still used in PCR_Process.c)*/
  PCR_WFSC_running = (int *)malloc( sizeof(int));
  *PCR_WFSC_running = 0;
  PCR_DM_running = (int *)malloc( sizeof(int));
  *PCR_DM_running = 0;

  PCR_WFSC_receiving_data = (int *)malloc( sizeof(int));
  *PCR_WFSC_receiving_data = 0;
  PCR_WFSC_Info.receiving_data = PCR_WFSC_receiving_data;
  WFSC_Info.receiving_data = PCR_WFSC_receiving_data;
  PCR_DM_receiving_data = (int *)malloc( sizeof(int));
  *PCR_DM_receiving_data = 0;
  PCR_DM_Info.receiving_data = PCR_DM_receiving_data;
  DM_Info.receiving_data = PCR_DM_receiving_data;

  PCR_WFSC_rate = (int *)malloc( sizeof(int));
  *PCR_WFSC_rate = 0;
  PCR_WFSC_Info.rate = PCR_WFSC_rate;
  WFSC_Info.rate = PCR_WFSC_rate;
  PCR_DM_rate = (int *)malloc( sizeof(int));
  *PCR_DM_rate = 0;
  PCR_DM_Info.rate = PCR_DM_rate;
  DM_Info.rate = PCR_DM_rate;

  PCR_Client_running = (int *)malloc( sizeof(int));
  *PCR_Client_running = 0;
  DServ_WFSC_running = (int *)malloc( sizeof(int));
  *DServ_WFSC_running = 0;
  DServ_DDS_running = (int *)malloc( sizeof(int));
  *DServ_DDS_running = 0;
  Info_running = (int *)malloc( sizeof(int));
  *Info_running = 0;

  PCR_WFSC_reset = (int *)malloc( sizeof(int));
  *PCR_WFSC_reset = 0;
  PCR_DM_reset = (int *)malloc( sizeof(int));
  *PCR_DM_reset = 0;

  DServ_debug = (int *)malloc( sizeof(int));
  *DServ_debug = debug;
  PCR_CMD_debug = (int *)malloc( sizeof(int));
  *PCR_CMD_debug = debug;
  PCR_WFSC_debug = (int *)malloc( sizeof(int));
  *PCR_WFSC_debug = debug;
  PCR_DM_debug = (int *)malloc( sizeof(int));
  *PCR_DM_debug = debug;
  PCR_Error_debug = (int *)malloc( sizeof(int));
  *PCR_Error_debug = debug;
  DServ_WFSC_debug = (int *)malloc( sizeof(int));
  *DServ_WFSC_debug = debug;
  DServ_DM_debug = (int *)malloc( sizeof(int));
  *DServ_DM_debug = debug;
  Info_debug = (int *)malloc( sizeof(int));
  *Info_debug = debug;
  Status_debug = (int *)malloc( sizeof(int));
  *Status_debug = debug;

  /*
    Fill in DServ_Thread_Info structures with WFSC and DM information and this new connection
  */
  DServ_WFSC_Info.wfsc = &WFSC_Info;
  DServ_WFSC_Info.dm = &DM_Info;
  DServ_WFSC_Info.sockfd = newsockfd;
  DServ_WFSC_Info.DServ_WFSC_continueRunning = DServ_WFSC_continueRunning;
  DServ_WFSC_Info.DServ_DM_continueRunning = DServ_DM_continueRunning;
  DServ_WFSC_Info.DServ_WFSC_debug = DServ_WFSC_debug;
  DServ_WFSC_Info.DServ_DM_debug = DServ_DM_debug;

  DServ_DM_Info.wfsc = &WFSC_Info;
  DServ_DM_Info.dm = &DM_Info;
  DServ_DM_Info.sockfd = newsockfd;
  DServ_DM_Info.DServ_WFSC_continueRunning = DServ_WFSC_continueRunning;
  DServ_DM_Info.DServ_DM_continueRunning = DServ_DM_continueRunning;
  DServ_DM_Info.DServ_WFSC_debug = DServ_WFSC_debug;
  DServ_DM_Info.DServ_DM_debug = DServ_DM_debug;

  /*
    Fill in Info structures with flag variables
  */
  Info.info = &Info_Info;
  Info.continueRunning = Info_continueRunning;
  Info.debug = Info_debug;

  /*
    Set the pointers to flag for specific Info Variables to the proper
      memory
  */
  status = Info_SetFlagPointers( Info_List, Info_ListLength, PCR_Client_running,
				 DServ_WFSC_running, DServ_DDS_running, Info_running,
				 PCR_WFSC_receiving_data, PCR_DM_receiving_data,
				 PCR_WFSC_rate, PCR_DM_rate,
				 &PCR_CMD_Socket, &PCR_WFSC_Socket, &PCR_DM_Socket,
				 &PCR_Error_Socket, local_debug);
  if ( status ) {
    printf("  PCR_Server: Error setting flag pointers in Info_SetFlagPointers\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Read the default values and place them in the Info_List structure
  */
  strcpy(filename, "default.info");
  status = Info_ReadFile( Info_List, Info_ListLength, filename, local_debug);
  if ( status ) {
    printf("  PCR_Server: Error reading default file in Info_ReadFile\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Set all PCR_Info[*].sockfd to -1 to indicate they are not in use
  */
  for ( i=0; i<NUM_CONNECTIONS; i++) {
    PCR_Info[i].sockfd = -1;
  }

  /*=====================================================================
    Start thread receive interrupt Errors from the PCR
    =======================================================================*/

  /*
    Setup the Status structure to send information to the Error thread
  */
  Error.socket = &PCR_Error_Socket;
  Error.info = &Info_Info;
  Error.continueRunning = PCR_Error_continueRunning;
  Error.debug = PCR_Error_debug;

  /*
    Start Error Thread
  */
  status = pthread_create( &errorThread, &attr, Error_Master, (void *)&Error);
  if ( start_debug ) {
    printf("  PCR_Server: Starting PCR Error thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  PCR_Server: Error starting PCR Error thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=====================================================================
    Start thread receive WFSC Data from the PCR
    =======================================================================*/

  /*
    Setup the Status structure to send information to the PCR_GetWFSC thread
  */

  /*
    Create the required pointer to data memory
  */
  WFSC_Thread_Client.info = &PCR_WFSC_Info;
  WFSC_Thread_Client.continueRunning = PCR_WFSC_continueRunning;
  WFSC_Thread_Client.running = PCR_WFSC_running;
  WFSC_Thread_Client.reset = PCR_WFSC_reset;
  WFSC_Thread_Client.debug = PCR_WFSC_debug;

  /*
    Set running flags
  */
  *PCR_WFSC_running = 1;
  *PCR_WFSC_continueRunning = 1;

  /*
    Start PCR_WFSC Thread
  */
  status = pthread_create( &pcr_wfscThread, &attr, PCR_GetWFSC, (void *)&WFSC_Thread_Client);
  if ( debug ) {
    printf("  PCR_Server: Starting PCR_GetWFSC thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  PCR_Server: Error starting PCR_GetWFSC thread\n");
    fflush(stdout);
  }

  /*=====================================================================
    Start thread receive DM Data from the PCR
    =======================================================================*/

  /*
    Setup the Status structure to send information to the PCR_GetDM thread
  */

  /*
    Create the required pointer to data memory
  */
  DM_Thread_Client.info = &PCR_DM_Info;
  DM_Thread_Client.continueRunning = PCR_DM_continueRunning;
  DM_Thread_Client.running = PCR_DM_running;
  DM_Thread_Client.reset = PCR_DM_reset;
  DM_Thread_Client.debug = PCR_DM_debug;

  /*
    Set running flags
  */
  *PCR_DM_running = 1;
  *PCR_DM_continueRunning = 1;

  /*
    Start PCR_DM Thread
  */
  status = pthread_create( &pcr_dmThread, &attr, PCR_GetDM, (void *)&DM_Thread_Client);
  if ( debug ) {
    printf("  PCR_Server: Starting PCR_GetDM thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  PCR_Server: Error starting PCR_GetDM thread\n");
    fflush(stdout);
  }

  /*=====================================================================
    Start threads to serve WFSC and DM information
    =======================================================================*/

  /*
    Start WFSC DServ Thread
  */
  status = pthread_create( &wfscThread, &attr, WFSC_Master, (void *)&DServ_WFSC_Info);
  if ( start_debug ) {
    printf("  PCR_Server: Starting WFSC thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  PCR_Server: Error starting WFSC thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*
    Start DServ data server Thread
  */
  status = pthread_create( &dservThread, &attr, DServ_Master, (void *)&DServ_DM_Info);
  if ( start_debug ) {
    printf("  PCR_Server: Starting DM thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  PCR_Server: Error starting DM thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=====================================================================
    Start thread to get Status (all) from the PCR
    =======================================================================*/

  /*
    Setup the Status structure to send information to the Status thread
  */
  Status.socket = &PCR_CMD_Socket;
  Status.info = &Info_Info;
  Status.continueRunning = Status_continueRunning;
  Status.debug = Status_debug;

  /*
    Start Status Thread
  */

  status = pthread_create( &statusThread, &attr, Status_Master, (void *)&Status);
  if ( start_debug ) {
    printf("  PCR_Server: Starting Status thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  PCR_Server: Error starting Status thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=====================================================================
    Start thread to serve Info data
    =======================================================================*/

  /*
    Start Info Server Thread
  */
  status = pthread_create( &infoThread, &attr, Info_Master, (void *)&Info);
  if ( start_debug ) {
    printf("  PCR_Server: Starting Info thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  PCR_Server: Error starting Info thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=====================================================================
    Open a non-blocking Socket to receive requests
    =======================================================================*/

  /*
    Open a TCP socket (an Internet stream socket) and bind
      the local address so a client can connect and send request
  */
  if ( Socket_OpenServer( (Socket_Info *)&Master_Socket, debug) ) {
    printf("  PCR_Server: Error opening DServ_Cmd socket\n");
    printf("    Exiting...\n");
    fflush(stdout);
    return(-1);
  }

  if ( start_debug ) {
    printf("  PCR_Server: Starting PCR_Master thread\n");
    fflush(stdout);
  }

  /*
    Set sockfd as non-blocking.  In this way we can see if the motor
      host (ao-topbox.mmto.arizona.edu) connects immediately.  If
      not, the host is most likely not available.  28dec04 DLM

    First, save the current flags for sockfd
  */
  saveflags=fcntl(Master_Socket.sockfd,F_GETFL,0);
  if(saveflags<0) {
    printf("  PCR_Server: Error saving original configuration of sockfd\n");
    printf("    Exiting...\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Now set this socket as non-blocking
  */
  if(fcntl(Master_Socket.sockfd,F_SETFL,saveflags|O_NONBLOCK)<0) {
    printf("  DServe: Error setting sockfd to non-blocking\n");
    printf("    Exiting...\n");
    fflush(stdout);
    return(-1);
  }

  do {

    /*
      Listen to this socket.  This function returns when a client
      has connected.  This is an example of a concurrent server.
    */
    clilen = sizeof( cli_addr );
    newsockfd = accept( Master_Socket.sockfd, (struct sockaddr *) &cli_addr, &clilen );

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
	if (      PCR_Info[i].sockfd < 0 ) break;
      }

      if ( i<NUM_CONNECTIONS ) {

	/*
	  Restore new socket to blocking connection
	*/
	if(fcntl(newsockfd,F_SETFL,saveflags)<0) {
	  printf("  PCR_Server: Error restoring newsockfd to blocking connection\n");
	  fflush(stdout);
	  *DServ_continueRunning = 0;
	  continue;
	}

	/*
	  Fill in Thread_Info structure with PCR information and this new connection
	*/
	PCR_Info[i].cmd = &PCR_CMD_Info;
	PCR_Info[i].wfsc = &PCR_WFSC_Info;
	PCR_Info[i].dm = &PCR_DM_Info;
	PCR_Info[i].error = &PCR_Error_Info;
	PCR_Info[i].info = &Info_Info;
	PCR_Info[i].sockfd = newsockfd;
	PCR_Info[i].DServ_continueRunning = DServ_continueRunning;
	PCR_Info[i].PCR_CMD_continueRunning = PCR_CMD_continueRunning;
	PCR_Info[i].PCR_WFSC_continueRunning = PCR_WFSC_continueRunning;
	PCR_Info[i].PCR_DM_continueRunning = PCR_DM_continueRunning;
	PCR_Info[i].PCR_Error_continueRunning = PCR_Error_continueRunning;
	PCR_Info[i].PCR_WFSC_running = PCR_WFSC_running;
	PCR_Info[i].PCR_DM_running = PCR_DM_running;
	PCR_Info[i].PCR_WFSC_reset = PCR_WFSC_reset;
	PCR_Info[i].PCR_DM_reset = PCR_DM_reset;
	PCR_Info[i].DServ_debug = DServ_debug;
	PCR_Info[i].PCR_CMD_debug = PCR_CMD_debug;
	PCR_Info[i].PCR_WFSC_debug = PCR_WFSC_debug;
	PCR_Info[i].PCR_DM_debug = PCR_DM_debug;
	PCR_Info[i].PCR_Error_debug = PCR_Error_debug;
	//	*DServ_debug = 0;

      } else {

	printf("  PCR_Server: Too many connections\n");
	fflush(stdout);
	continue;

      }

      /*
	Start Thread
      */
      status = pthread_create( &pcrThread[i], &attr, PCR_Process, (void *)&PCR_Info[i]);
      if ( start_debug ) {
	printf("  PCR_Server: Starting CMD thread\n");
	fflush(stdout);
      }
      if ( status ) {
	printf("  PCR_Server: Error starting CMD thread\n");
	fflush(stdout);
	*DServ_continueRunning = 0;
	continue;
      }

    }

  } while ( *DServ_continueRunning );

  if ( debug ) {
    printf("  PCR_Server: Data Server Shutdown\n");
    fflush(stdout);
  }

  for ( i=0; i<NUM_CONNECTIONS; i++) {
    if (      PCR_Info[i].sockfd >= 0 ) {
      if ( debug ) {
	printf("  PCR_Server: Canceling thread %d\n", i);
	fflush(stdout);
      }
      pthread_cancel( pcrThread[i]);
    }
  }
  Socket_Close( (Socket_Info *)&Master_Socket, debug);

  /*
    Wait for a bit until the other master threads have time
     to shutdown
  */
  sleep(1);

  return(0);
}
