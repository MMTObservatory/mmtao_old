/*
  Status_Master.c

  This routine starts a Status data server which will serve infomation about
    the status of the AO loop, PCR, DServ and total AO system

  This process will not collect or disperse data, but creeates threads
    to do this.

  This top level server lists to port INFO_PORT.

  Written  19nov06  DLM  Started with the DServ_Master.c

  Altered   2apr07  DLM  Added mutex lock and unlock to keep from having
                           collisions between different threads trying to
			   use the PCR_CMD port.
*/

#include <pthread.h>

#include "PCR.h"
#include "DServ.h"
#include "PCR_Cmd.h"
#include "Status.h"

/*
  The mutex variable PCR_CMD_Lock is a global variable set in PCR_Server.c.  We
    must now make it visible in this routine.
*/
extern pthread_mutex_t PCR_CMD_Lock;

/*================================================================================*
 * Start server to manage Status servers
 *================================================================================*/
void *Status_Master( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  Status_Thread *Main_Info;
  Socket_Info *PCR_CMD_Socket;
  Info_Struct *Info;
  int *continueRunning;
  int *debug;

  char Status_Reply[STATUS_REPLY_LENGTH];
  char request[STRING_LENGTH];
  int status;
  int Init_List = 1;

  int *Status_Position;
  int Status_Count;
  int Status_Lines;
  int local_debug = 0;

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (Status_Thread *)Passed_Info;
  PCR_CMD_Socket = Main_Info->socket;
  Info = Main_Info->info;
  continueRunning = Main_Info->continueRunning;
  debug = Main_Info->debug;

  //  *debug = 1;

  /*=====================================================================
    Start loop to retrieve status from the PCR.  Status is retreived only
      if a connect to the PCR CMD socket already exist.
    =======================================================================*/

  do {

    /*
      Check if we are connected to the PCR.  If so, initialize the parameter
        list with the first call to "All" and then loop at the given rate (set
	by sleep()) to update the parameters.
    */
    if ( PCR_CMD_Socket->connected ) {

      if ( Init_List ) {

	/*
	  If we have not connected to the PCR, or the connection was lost
	    and re-established, then we need to get the parameter list from
	    the PCR and setup the list of parameter positions

	    Lock the mutex variable PCR_CMD_Lock so other threads do not try
	      to read or write while this thread is retrieving status information.
	    If another thread has the variable PCR_CMD_Lock locked, this process
	      is blocked until the mutex is unlocked.  Likewise, if the mutex
	      variable is locked here, other processed are blocked until the mutex
	      is unlock by this thread.
	    In all other routines that communication on the PCR_CMD port the
	      same mutex lock function is called.
	*/
	pthread_mutex_lock( &PCR_CMD_Lock);

	/*
	  Send (all) request to PCR for Status
	*/
	strcpy( request, "all\n");
	if ( Socket_StringWrite( PCR_CMD_Socket->sockfd, request, strlen(request)) != strlen(request) ) {
	  if ( *debug ) {
	    printf("  Status_Master: Error writing all in Socket_StringWrite\n");
	    fflush(stdout);
	  }
	  pthread_mutex_unlock( &PCR_CMD_Lock);
	  status =  Socket_Close ( PCR_CMD_Socket, *debug);
	  if ( status ) {
	    printf("  Status_Master: Error in Socket_Close\n");
	    fflush(stdout);
	  }	
	  continue;
	}

	/*
	  Retrieve the PCR status value until an EOF is found
	*/
	status = Status_ReadToEOF( PCR_CMD_Socket, Status_Reply, &Status_Lines, *debug);
	if ( status ) {
	  if ( *debug ) {
	    printf("  Status_Master: Error reading parameter list from PCR\n");
	    fflush(stdout);
	  }
	  pthread_mutex_unlock( &PCR_CMD_Lock);
	  status =  Socket_Close ( PCR_CMD_Socket, *debug);
	  if ( status ) {
	    printf("  Status_Master: Error in Socket_Close\n");
	    fflush(stdout);
	  }	
	  continue;
	}

	/*
	  Unlock the mutex variable PCR_CMD_Lock so other threads may use the
	  PCR_CMD port
	*/
	pthread_mutex_unlock( &PCR_CMD_Lock);

	/*
	  Find the position in the InfoList of all the parameters sent by the PCR
	    and store them in the array, position
	*/
	status = Status_ParameterPositions( Info, Status_Reply, &Status_Position,
					    &Status_Count, *debug);
	if ( status ) {
	  if ( *debug ) {
	    printf("  Status_Master: Error finding all parameters in Status_ParameterPositions\n");
	    fflush(stdout);
	  }
	}

	/*
	  Place the passed values into the InfoList
	*/
	status = Status_UpdateParameters( Info, Status_Reply, Status_Position, Status_Count, *debug);
	if ( status ) {
	  if ( *debug ) {
	    printf("  Status_Master: Error finding all parameters in Status_UpdateParameter\n");
	    fflush(stdout);
	  }
	}

	Init_List = 0;
      }

      /*
	Retrieve the status parameter and place the values into the Info List

	Lock the mutex variable PCR_CMD_Lock so other threads do not try
	  to read or write while this thread is retrieving status information.
	If another thread has the variable PCR_CMD_Lock locked, this process
	  is blocked until the mutex is unlocked.  Likewise, if the mutex
	  variable is locked here, other processed are blocked until the mutex
	  is unlock by this thread.
	In all other routines that communication on the PCR_CMD port the
	  same mutex lock function is called.
      */
      pthread_mutex_lock( &PCR_CMD_Lock);

      /*
	Send (all) request to PCR for Status
      */
      strcpy( request, "all\n");
      if ( Socket_StringWrite( PCR_CMD_Socket->sockfd, request, strlen(request)) != strlen(request) ) {
	if ( *debug ) {
	  printf("  Status_Master: Error writing all in Socket_StringWrite\n");
	  fflush(stdout);
	}
	pthread_mutex_unlock( &PCR_CMD_Lock);
	status =  Socket_Close ( PCR_CMD_Socket, *debug);
	if ( status ) {
	  printf("  Status_Master: Error in Socket_Close\n");
	  fflush(stdout);
	}	
	continue;
      }

      /*
	Retrieve the PCR status value until an EOF is found
      */
      status = Status_ReadToEOF( PCR_CMD_Socket, Status_Reply, &Status_Lines, local_debug);
      if ( status ) {
	if ( *debug ) {
	  printf("  Status_Master: Error reading parameter list from PCR\n");
	  fflush(stdout);
	}
	pthread_mutex_unlock( &PCR_CMD_Lock);
	status =  Socket_Close ( PCR_CMD_Socket, *debug);
	if ( status ) {
	  printf("  Status_Master: Error in Socket_Close\n");
	  fflush(stdout);
	}	
	continue;
      }

      /*
	Unlock the mutex variable PCR_CMD_Lock so other threads may use the
	  PCR_CMD port
      */
      pthread_mutex_unlock( &PCR_CMD_Lock);

      /*
	If we have lost connection to the pcr, then Status_Lines (the number of status
	  parameters returned) is 0 (or at least not equal to Status_Count).  In this
	  case we should close the connection and loop until the connection is re-established
      */
      if ( Status_Lines != Status_Count ) {
	printf("  Status_Master: Connection lost to CMD port of the PCR\n");
	fflush(stdout);
	status =  Socket_Close ( PCR_CMD_Socket, *debug);
	if ( status ) {
	  printf("  Status_Master: Error in Socket_Close\n");
	  fflush(stdout);
	}	
	continue;
      }
	

      /*
	Place the passed values into the InfoList
      */
      status = Status_UpdateParameters( Info, Status_Reply, Status_Position, Status_Count, local_debug);
      if ( status ) {
	if ( *debug ) {
	  printf("  Status_Master: Error finding all parameters in Status_UpdateParameter\n");
	  fflush(stdout);
	}
      }

    } else {

      /*
	Set the flag that we are not connected, or have lost the connection to
          the PCR.  If Init_List is true (1) then when we are re-connected the
	  first call to "All" is to setup the parameter list
      */
      Init_List = 1;

      if ( *debug ) {
	printf("  Status_Master: Not connected to PCR\n");
	fflush(stdout);
      }

      /*
	Open a socket to the PCR server
      */
      status =  Socket_OpenClient ( PCR_CMD_Socket, local_debug);
      if ( status ) {
	if ( local_debug ) {
	  printf("  Status_Master: Error connecting to the PCR Cmd port in Socket_OpenClient\n");
	  fflush(stdout);
	}
	Socket_Close( PCR_CMD_Socket, *debug);
      }

    }

    /*
      Set the refresh rate of the status looper by sleeping this thread
    */
    sleep(1);

  } while ( *continueRunning );

  if ( *debug ) {
    printf("  Status_Master: Status Server Shutdown\n");
    fflush(stdout);
  }

  return( (void *)0);
}
