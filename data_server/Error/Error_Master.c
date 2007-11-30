/*
  Error_Master.c

  This routine connects to the error port of the PCR and waits for
    interrupt error messaged (integers) from the PCR.

  When this thread connects to the PCR Error Port it expects an OK
    from the PCR to confirm the connection.  After that, this client
    waits for an integer to sent.  Once an error number has been received,
    the appropriate error message is found in the Error_Structure and
    the appropriate steps are taken by the PCR_Server.  These steps are
    usually accomplished by setting Info Variables to the correct values.
    Specifically, this thread does not change the state of the PCR_Server
    of the PCR GUI, but simply sets Info variables.

  This server listens to port PCR_ERROR_PORT.

  Written  16mar07  DLM  Started with the Status_Master.c

*/

#include <pthread.h>

#include "PCR.h"
#include "Info.h"
#include "Status.h"
#include "PCR_Error.h"
#include "Error.h"

/*================================================================================*
 * Start server to manage Error servers
 *================================================================================*/
void *Error_Master( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  Status_Thread *Main_Info;
  Socket_Info *PCR_Error_Socket;
  Info_Struct *Info;
  Info_Entry *List;
  long list_length;
  int *continueRunning;
  int *debug;
  int local_debug = 0;

  int status;
  int tempInt, errorNumber;
  char errorMessage[STRING_LENGTH];
  char *intPtr;

  /*
    Pointers to the needed Info structures
  */
  long PCR_ERROR_Connected;

  /*
    Variables needed to use the select commands to watch for data on a port
  */
  fd_set rfds;
  struct timeval tv;

  /*
    Pointers to the needed Info structures
  */
  long Loop_Running_Position;
  long Loop_Gain_Position;

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (Status_Thread *)Passed_Info;
  PCR_Error_Socket = Main_Info->socket;
  Info = Main_Info->info;
  List = Info->list;
  list_length = Info->length;
  continueRunning = Main_Info->continueRunning;
  debug = Main_Info->debug;

  //  *debug = 1;

  /*********************************************************************
    Determine the positions of the needed InfoList structures.
   *********************************************************************/

  /*
    Find the position of PCR_ERROR_Connected flag in the InfoList so we
      can watch for the connection to be made
  */
  status = Info_FindParameter( List, list_length, "PCR_ERROR_Connected", &PCR_ERROR_Connected, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    *continueRunning = 0;
  }

  /*
    Find the position of Loop information so if and error is received and the loop is opened
      the Info variables can be updated.
  */
  status = Info_FindParameter( List, list_length, "Loop_Running", &Loop_Running_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "Loop_Gain", &Loop_Gain_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  /*
    Loop that keeps this thread either check to see if it is connect to the PCR Error port, or
      if so, then check to see if an error has been sent
  */
  while ( *continueRunning ) {

    /*=====================================================================
      Start loop to wait for PCR_Server to connect to the PCR Error port
      =======================================================================*/


    while ( !PCR_Error_Socket->connected ) {

      if ( *debug ) {
	printf("  Error_Master: Trying to connect to the PCR Error server\n");
	fflush(stdout);
      }

      /*
	We are not connected, so try to open a socket to the PCR Error server
      */
      status =  Socket_OpenClient ( PCR_Error_Socket, local_debug);
      if ( status ) {

	/*
	  No connection so wait a bit to try again
	*/
	if ( local_debug ) {
	  printf("  Error_Master: Error connecting to the PCR Error port in Socket_OpenClient\n");
	  fflush(stdout);
	}
	sleep(1);

      }

    }

    /*
      Watch to see when there is data to be read on sockfd
    */
    FD_ZERO(&rfds);
    FD_SET(PCR_Error_Socket->sockfd, &rfds);
    /* Wait up to one seconds. */
    tv.tv_sec = 1;
    tv.tv_usec = 0;
 
    status = select( (PCR_Error_Socket->sockfd)+1, &rfds, NULL, NULL, &tv);

    if ( !status ) {

	printf("  Error_Master: Error, did not receive PCR_OK from PCR.\n        Closing connection\n");
	fflush(stdout);

	/*
	  Close the connection to the PCR Error server and continue waiting for a proper connection
	*/
	status =  Socket_Close ( PCR_Error_Socket, *debug);
	if ( status ) {
	  printf("  Error_Master: Error in Socket_Close\n");
	  fflush(stdout);
	}

    } else {

      /*
	Read PCR_OK (integer 0) from the Error Socket, which indicates the connection
          has been properly made.
      */
      intPtr = (char *)&tempInt;
      if ( Socket_Read( PCR_Error_Socket->sockfd, intPtr, sizeof(int)) ) {
	if ( *debug ) {
	  printf("  Error_Master: Error reading PCR_OK in Socket_Read\n");
	  fflush(stdout);
	}
	continue;
      }
      errorNumber = htons(tempInt);

      if ( errorNumber != PCR_OK ) {

	/*
	  Did not get the correct response from the PCR Error server
	*/
	printf("  Error_Master: Expected %d, received %d from the PCR Error server\n        Closing connection\n",
	       PCR_OK, errorNumber);
	fflush(stdout);

	/*
	  Close the connection to the PCR Error server and continue waiting for a proper connection
	*/
	status =  Socket_Close ( PCR_Error_Socket, *debug);
	if ( status ) {
	  printf("  Error_Master: Error in Socket_Close\n");
	  fflush(stdout);
	}

      } else {

	if ( *debug ) {
	  printf("  Error_Master: Connected to the PCR Error server\n");
	  fflush(stdout);
	}

      }

    }

    /*=====================================================================
      Start loop to check for error messages from the PCR.
    =======================================================================*/

    while ( *continueRunning && PCR_Error_Socket->connected ) {

      /*
	Watch to see when there is data to be read on sockfd
      */
      FD_ZERO(&rfds);
      FD_SET(PCR_Error_Socket->sockfd, &rfds);
      /* Wait up to one seconds. */
      tv.tv_sec = 1;
      tv.tv_usec = 0;
 
      status = select( (PCR_Error_Socket->sockfd)+1, &rfds, NULL, NULL, &tv);

      if ( !status ) {

	/*
	  Jump to the end of the loop to see if a shutdown has been requested.  If
	    not (*continueRunning != 0) then we will be back to check for another error
	    number from the PCR
	*/
	continue;

      }

      /*
	Read the error from the Error Socket
      */
      intPtr = (char *)&tempInt;
      if ( Socket_Read( PCR_Error_Socket->sockfd, intPtr, sizeof(int)) ) {
	if ( *debug ) {
	  printf("  Error_Master: Connection lost to Error port of the PCR\n");
	  fflush(stdout);
	}
	status =  Socket_Close ( PCR_Error_Socket, *debug);
	if ( status ) {
	  printf("  Error_Master: Error in Socket_Close\n");
	  fflush(stdout);
	}
	continue;
      }
      errorNumber = ntohl(tempInt);

      if ( *debug ) {
	printf("  Error_Master: Error Number = %d\n", errorNumber);
	fflush(stdout);
      }

      if ( errorNumber < 0 ) {

	/*
	  An error has occurred, so loop has been opened.  Let everyone know via the
	    Info variables
	*/
	status = Error_PlaySound( 1);
	List[Loop_Running_Position].i_value = 0;
	List[Loop_Gain_Position].f_value = 0.0;

      }

      /*
	Find and printout the appropriate error message
      */
      status = Error_FindMessage( errorNumber, errorMessage, 1);
      if ( status ) {
	continue;
      }	else {
	printf("  PCR Interrupt Error: %s\n", errorMessage);
	fflush(stdout);
      }

    }

  }

  if ( PCR_Error_Socket->connected ) {
    status =  Socket_Close ( PCR_Error_Socket, *debug);
    if ( status ) {
      printf("  Error_Master: Error in Socket_Close\n");
      fflush(stdout);
    }
  }

  if ( *debug ) {
    printf("  Error_Master: Error Server Shutdown\n");
    fflush(stdout);
  }

  return( (void *)0);
}
