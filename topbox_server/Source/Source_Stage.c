/*
  Source_Stage.c

  This routine starts a test server for a Lynx stage

  Written  25aug07  DLM  Started with Source_MVP.c

*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Source.h"

/*================================================================================*
 * Start Stage server
 *================================================================================*/
void *Source_Stage( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  Device_Info *Info;
  Socket_Info *Socket;
  Device_Data *Data;
  int *continueRunning;
  int *debug;

  float position;
  int status;
  int newsockfd;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  char temp[STRING_LENGTH];
  int local_debug = 0;

  char *tok_ptr;
  char *action, *parameter, *flag, *vm;
  float distance = 0.0;
  float abs_distance = 0.0;
  int running = 0;

  /*
    Client connection information
  */
  struct sockaddr_in cli_addr;
  socklen_t clilen;

  /*
    Put the passed variables in to the local variables
  */
  Info = (Device_Info *)Passed_Info;
  Socket = Info->socket;
  Data = Info->data;
  continueRunning = Info->continueRunning;
  debug = Info->cmd_debug;

  Data->home = 0;
  Data->position = 100.0;
  Data->moving = 0;
  Data->in_position = 1;
  Data->error = 0;
  Data->enabled = 1;
  Data->positive = 0;
  Data->negative = 0;
  Data->event_2 = 0;

  /*
    Device Structures: contains all device information.
  */
  pthread_t mvpThread;
  Source_Info Local_Info;

  Local_Info.socket = Socket;
  Local_Info.data = Data;
  Local_Info.distance = &distance;
  Local_Info.running = &running;

  /*
    Open a TCP socket (an Internet stream socket) and bind
      the local address so a client can connect and send request
  */
  if ( Socket_OpenServer( Socket, *debug) ) {
    printf("  Source_Stage: Error opening %s socket\n", Socket->name);
    printf("    Exiting...\n");
    fflush(stdout);
    return( (void *)0);
  }

  do {

    /*
      Listen to this socket.  This function returns when a client
      has connected.  This is an example of a concurrent server.
    */
    clilen = sizeof( cli_addr );
    newsockfd = accept( Socket->sockfd, (struct sockaddr *) &cli_addr, &clilen );
    if( newsockfd < 0 ) {
      printf("  Source_Stage: accept error\n" );
      fflush(stdout);
      *continueRunning = 0;
      continue;
    }

    Socket->connected = 1;

    if ( *debug ) {
      printf("  Source_Stage: %s Connection...\n", Socket->name);
      fflush(stdout);
    }

    do {

      /*
	Read an instruction from the client
      */
      status = Source_StageRead( newsockfd, Socket, request);
      if ( status < 0 ) {
	if ( *debug ) {
	  printf("  Source_Stage: Error reading request in Socket_StringRead\n");
	  fflush(stdout);
	}
	Socket->connected = 0;
	continue;
      } else if ( status > 0 ) {
	continue;
      }

      //      printf(" request = %s  %x\n", request, (int) request);
      action = strtok_r( request, " ", &tok_ptr);
      if ( action == NULL ) {
	printf("  Source_Stage: Action parameter not included in request\n");
	fflush(stdout);
	continue;
      }
      //      printf(" action = %s  %x  %x\n", action, (int) action, (int) tok_ptr);

      /*
	Process this request
      */

      if ( !strncmp ( action, "DRVEN=1", strlen("DRVEN=1") ) ) {

	Data->enabled = 1;
	if ( *debug ) {
	  printf("  Source_Stage: Request = %s\n", action);
	  fflush(stdout);
	}

      } else if ( !strncmp ( action, "DRVEN=0", strlen("DRVEN=0") ) ) {

	Data->enabled = 0;
	if ( *debug ) {
	  printf("  Source_Stage: Request = %s\n", action);
	  fflush(stdout);
	}

      } else if ( !strncmp ( action, "PRINT", strlen("PRINT") ) ) {

	parameter = strtok_r( NULL, " \0", &tok_ptr);
	//	printf(" parameter = %s  %x  %x\n", parameter, (int) parameter, (int) tok_ptr);
	if ( parameter == NULL ) {
	  printf("  Source_Stage: PRINT parameter not included in request\n");
	  fflush(stdout);
	  continue;
	}

	if ( !strncmp ( parameter, "POS", strlen("POS") ) ) {

	  if ( local_debug ) {
	    printf("  Source_Stage: Request = %s\n", request);
	    fflush(stdout);
	  }

	  /*
	    Create the position reply string.
	  */
	  position = Data->position * Data->gain;
	  sprintf( reply, "%f", position);
	  if ( local_debug ) {
	    printf("  Source_Stage: Position = %s\n", reply);
	    fflush(stdout);
	  }
	} else if ( !strncmp ( parameter, "UVARS", strlen("UVARS") ) ) {

	  if ( local_debug ) {
	    printf("  Source_Stage: Request = %s\n", request);
	    fflush(stdout);
	  }

	  /*
	    Create the Home flag reply string.
	  */
	  sprintf( reply, "%d.0", Data->home);
	  if ( local_debug ) {
	    printf("  Source_Stage: HOME = %s\n", reply);
	    fflush(stdout);
	  }

	} else if ( !strncmp ( parameter, "DRVEN", strlen("DRVEN") ) ) {

	  if ( local_debug ) {
	    printf("  Source_Stage: Request = %s\n", request);
	    fflush(stdout);
	  }

	  /*
	    Create the Home flag reply string.
	  */
	  sprintf( reply, "%s", Data->enabled ? "TRUE" : "FALSE");
	  if ( local_debug ) {
	    printf("  Source_Stage: DRVEN = %s\n", reply);
	    fflush(stdout);
	  }

	} else if ( !strncmp ( parameter, "MVG", strlen("MVG") ) ) {

	  if ( local_debug ) {
	    printf("  Source_Stage: Request = %s\n", request);
	    fflush(stdout);
	  }

	  /*
	    Create the Home flag reply string.
	  */
	  sprintf( reply, "%s", Data->moving ? "TRUE" : "FALSE");
	  if ( local_debug ) {
	    printf("  Source_Stage: MVG = %s\n", reply);
	    fflush(stdout);
	  }

	} else if ( !strncmp ( parameter, "IO", strlen("IO") ) ) {

	  flag = strtok_r( NULL, " \0", &tok_ptr);
	  //	  printf(" flag = %s  %x  %x\n", flag, (int) flag, (int) tok_ptr);
	  if ( flag == NULL ) {
	    printf("  Source_Stage: PRINT IO flag not included in request\n");
	    fflush(stdout);
	    continue;
	  }

	  if ( !strncmp ( flag, "25", strlen("25") ) ) {

	    if ( local_debug ) {
	      printf("  Source_Stage: Request = %s\n", request);
	      fflush(stdout);
	    }

	    /*
	      Create the Home flag reply string.
	    */
	    sprintf( reply, "%d", Data->negative);
	    if ( local_debug ) {
	      printf("  Source_Stage: NEGATIVE = %s\n", reply);
	      fflush(stdout);
	    }

	  } else if ( !strncmp ( flag, "26", strlen("26") ) ) {

	    if ( local_debug ) {
	      printf("  Source_Stage: Request = %s\n", request);
	      fflush(stdout);
	    }

	    /*
	      Create the Home flag reply string.
	    */
	    sprintf( reply, "%d", Data->positive);
	    if ( local_debug ) {
	      printf("  Source_Stage: POSITIVE = %s\n", reply);
	      fflush(stdout);
	    }

	  } else {

	    printf(" Unknown PRINT IO request = %s\n", flag);
	    fflush(stdout);
	    continue;

	  }

	} else {

	  printf(" Unknown PRINT request = %s\n", parameter);
	  fflush(stdout);
	  continue;

	}

	/*
	  Add a carriage return to the beginning and end of the rely string
	*/
	strcpy( temp, "\n");
	strcat( temp, reply);
	strcat( temp, "\r");

	/*
	  Send the request
	*/
	if ( Socket_StringWrite( newsockfd, temp, strlen(temp)) != strlen(temp) ) {
	  if ( *debug ) {
	    printf("  Source_Stage: Error sending PRINT reply %s in Socket_StringWrite\n", reply);
	    fflush(stdout);
	  }
	}

      } else if ( !strncmp ( action, "POS=0.00", strlen("POS=0.00") ) ) {

	Data->position = 0.0;
	if ( *debug ) {
	  printf("  Source_Stage: Request = %s\n", action);
	  fflush(stdout);
	}

      } else if ( !strncmp ( action, "HOME=0.0", strlen("HOME=0.0") ) ) {

	Data->home = 0;
	if ( *debug ) {
	  printf("  Source_Stage: Request = %s\n", action);
	  fflush(stdout);
	}

      } else if ( !strncmp ( action, "HOME=1.0", strlen("HOME=1.0") ) ) {

	Data->home = 1;
	if ( *debug ) {
	  printf("  Source_Stage: Request = %s\n", action);
	  fflush(stdout);
	}

      } else if ( !strncmp ( action, "VM=", strlen("VM=") ) ) {

	/*
	  The action string contains the value requested to set the velocity so
	    we my chop up "action".
	*/
	vm = strtok_r( request, "=", &tok_ptr);
	parameter = strtok_r( NULL, " ", &tok_ptr);
	if ( parameter == NULL ) {
	  printf("  Source_Stage: Velocity parameter not included in request\n");
	  fflush(stdout);
	  continue;
	}

	Data->speed = (int) atof(parameter);

	if ( *debug ) {
	  printf("  Source_Stage: %s speed set to %d\n", Socket->name, Data->speed);
	  fflush(stdout);
	}

      } else if ( !strncmp ( action, "MOVR", strlen("MOVR") ) ) {

	/*
	  Read the distance to move
	*/
	parameter = strtok_r( NULL, " ", &tok_ptr);
	if ( parameter == NULL ) {
	  printf("  Source_Stage: Distance parameter not included in request\n");
	  fflush(stdout);
	  continue;
	}
	distance = atof( parameter );

	if ( *debug ) {
	  printf("  Source_Stage: Request = %s %.2f\n", action, distance);
	  fflush(stdout);
	}

	if ( *(Local_Info.running) ) {

	  printf("  Source_Stage: Thread to make a relative move for %s already running\n", Socket->name);
	  fflush(stdout);

	} else {


	  /*=====================================================================
	    Start thread to move the stage the given relative distance
	    =======================================================================*/
	  status = pthread_create( &mvpThread, NULL, Source_StageRelativeMove, (void *)&Local_Info);
	  if ( local_debug ) {
	    printf("  Source_Stage: Starting thread to make a relative move of %s\n", Socket->name);
	    fflush(stdout);
	  }
	  if ( status ) {
	    printf("  Source_Stage: Error starting thread to make a relative move of %s\n", Socket->name);
	    fflush(stdout);
	  }

	}

      } else if ( !strncmp ( action, "MOVA", strlen("MOVA") ) ) {

	/*
	  Read the absolute distance to move
	*/
	parameter = strtok_r( NULL, " ", &tok_ptr);
	if ( parameter == NULL ) {
	  printf("  Source_Stage: Distance parameter not included in request\n");
	  fflush(stdout);
	  continue;
	}
	abs_distance = atof( parameter );

	if ( *debug ) {
	  printf("  Source_Stage: Request = %s %.2f\n", action, abs_distance);
	  fflush(stdout);
	}

	if ( *(Local_Info.running) ) {

	  printf("  Source_Stage: Thread to make a relative move for %s already running\n", Socket->name);
	  fflush(stdout);

	} else {

	  /*
	    We only make relative moves in this stage emulator so we must convert an absolute
	      distance to a relative move
	  */
	  distance = abs_distance - Data->position;

	  /*=====================================================================
	    Start thread to move the stage the given relative distance
	    =======================================================================*/
	  status = pthread_create( &mvpThread, NULL, Source_StageRelativeMove, (void *)&Local_Info);
	  if ( local_debug ) {
	    printf("  Source_Stage: Starting thread to make a relative move of %s\n", Socket->name);
	    fflush(stdout);
	  }
	  if ( status ) {
	    printf("  Source_Stage: Error starting thread to make a relative move of %s\n", Socket->name);
	    fflush(stdout);
	  }

	}

      } else {

	printf(" Unknown request= %s\n", action);
	fflush(stdout);

      }
      
    } while ( Socket->connected );

    close(newsockfd);

  } while ( *continueRunning );

  if ( *debug ) {
    printf("  Source_Stage: Data Server Shutdown\n");
    fflush(stdout);
  }

  Socket_Close( (Socket_Info *)&Socket, *debug);
  return( (void *)0);
}
