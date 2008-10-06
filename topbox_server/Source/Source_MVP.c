/*
  Source_MVP.c

  This routine starts a test server for a device controlled by an
    MVP controller

  Written  15aug07  DLM  Started with Source_Start.c

*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Source.h"

/*================================================================================*
 * Start MVP server
 *================================================================================*/
void *Source_MVP( void *Passed_Info)
{
  /*
    Local pointers to varible passed from the process which created this thread
   */
  Device_Info *Info;
  Socket_Info *Socket;
  Device_Data *Data;
  int *continueRunning;
  int *debug;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  int position;
  int status;
  int newsockfd;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  int local_debug = 0;

  char *tok_ptr;
  char *device, *action, *parameter;
  int home_trigger = 0;
  int remaining = 0;
  int relative = 0;
  int absolute = 0;
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
  Data->position = 0.0;
  Data->moving = 0;
  Data->in_position = 1;
  Data->error = 0;
  Data->enabled = 1; 
  Data->positive = 0;
  Data->negative = 0;
  Data->event_2 = 1;

  /*
    Device Structures: contains all device information.
  */
  pthread_t mvpThread;
  Source_Info Local_Info;

  Local_Info.socket = Socket;
  Local_Info.data = Data;
  Local_Info.home_trigger = &home_trigger;
  Local_Info.remaining = &remaining;
  Local_Info.relative = &relative;
  Local_Info.absolute = &absolute;
  Local_Info.running = &running;

  /*
    Open a TCP socket (an Internet stream socket) and bind
      the local address so a client can connect and send request
  */
  if ( Socket_OpenServer( Socket, *debug) ) {
    printf("  Source_MVP: Error opening %s socket\n", Socket->name);
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
      printf("  Source_MVP: accept error\n" );
      fflush(stdout);
      *continueRunning = 0;
      continue;
    }

    Socket->connected = 1;

    if ( *debug ) {
      printf("  Source_MVP: %s Connection...\n", Socket->name);
      fflush(stdout);
    }

    do {

      /*
	Read an instruction from the client
      */
      status = Source_ReadMVP( newsockfd, Socket, request);
      if ( status < 0 ) {
	if ( *debug ) {
	  printf("  Source_MVP: Error reading request in Socket_StringRead\n");
	  fflush(stdout);
	}
	Socket->connected = 0;
	continue;
      } else if ( status > 0 ) {
	continue;
      }

      device = strtok_r( request, " ", &tok_ptr);
      action = strtok_r( NULL, " ", &tok_ptr);
      if ( action == NULL ) {
	printf("  Source_MVP: Action parameter not included in request\n");
	fflush(stdout);
	continue;
      }

      //      printf("    device = %s\n", device);
      //      printf("    action = %s\n", action);

      /*
	Process this request
      */
      if ( !strncmp ( action, "ST", strlen("ST") ) ) {
	
	/*
	  Create the status reply string.  The MVP sends out a rubbish string terminated
	    with a NL/LF (\n) and then the needed status string terminated with a CR (\r).
	*/
	status = Source_StatusMVP( Socket, Data, reply, local_debug);

	/*
	  Send the request
	*/
	if ( Socket_StringWrite( newsockfd, reply, strlen(reply)) != strlen(reply) ) {
	  if ( *debug ) {
	    printf("  Source_MVP: Error writing status reply in Socket_StringWrite\n");
	    fflush(stdout);
	  }
	}

      } else if ( !strncmp ( action, "AB", strlen("AB") ) ) {

	remaining = 0;
	if ( *debug ) {
	  printf("  Source_MVP: Request = %s\n", action);
	  fflush(stdout);
	}

      } else if ( !strncmp ( action, "EN", strlen("EN") ) ) {

	Data->enabled = 1;
	if ( *debug ) {
	  printf("  Source_MVP: Request = %s\n", action);
	  fflush(stdout);
	}

      } else if ( !strncmp ( action, "DI", strlen("DI") ) ) {

	remaining = 0;
	Data->enabled = 0;
	if ( *debug ) {
	  printf("  Source_MVP: Request = %s\n", action);
	  fflush(stdout);
	}

      } else if ( !strncmp ( action, "POS", strlen("POS") ) ) {

	if ( local_debug ) {
	  printf("  Source_MVP: Request = %s\n", action);
	  fflush(stdout);
	}

	/*
	  Create the status reply string.  We first have to convert the float position value
	    (because of definition of the Device_Data struture) to integer counts in device
	    units.
	  The MVP sends out a rubbish string terminated with a NL/LF (\n) and then the needed
	    status string terminated with a CR (\r).
	*/
	position = (int) ( Data->position * (double)Data->gain);
	sprintf( reply, "01234567\n  %12x", position);
	if ( local_debug ) {
	  printf("  Source_MVP: Position in user = %f\n", Data->position);
	  printf("                        device = %d\n", position);
	  printf("                   hex (reply) = %x\n", position);
	  fflush(stdout);
	}
	strcat( reply, "\r");

	/*
	  Send the request
	*/
	if ( Socket_StringWrite( newsockfd, reply, strlen(reply)) != strlen(reply) ) {
	  if ( *debug ) {
	    printf("  Source_MVP: Error writing position in Socket_StringWrite\n");
	    fflush(stdout);
	  }
	}

      } else if ( !strncmp ( action, "HO", strlen("HO") ) ) {

	Data->position = 0.0;
	Data->event_2 = 1;
	if ( *debug ) {
	  printf("  Source_MVP: Request = %s\n", action);
	  fflush(stdout);
	}

      } else if ( !strncmp ( action, "ANO", strlen("ANO") ) ) {

	/*
	  See if there is a parameter to the ANO request.  If there is, then
	    this integer is Home flag for the device, so put it
	    in the Data structure as speed.  If not, then the ANO request is just
	    querying the value of the Home flag.
	*/
	parameter = strtok_r( NULL, " ", &tok_ptr);

	if ( parameter == NULL ) {

	  /*
	    Send back the value of the Home flag (in hex format)
	  */
	  sprintf( reply, "01234567\n  %12x\r", Data->home);

	  /*
	    Send the request
	  */
	  if ( Socket_StringWrite( newsockfd, reply, strlen(reply)) != strlen(reply) ) {
	    if ( *debug ) {
	      printf("  Source_MVP: Error writing Home flag in Socket_StringWrite\n");
	      fflush(stdout);
	    }
	  }

	} else {

	  Data->home = atoi(parameter);

	  if ( *debug ) {
	    printf("  Source_MVP: %s home flag set to %d\n", Socket->name, Data->home);
	    fflush(stdout);
	  }

	}

      } else if ( !strncmp ( action, "HA", strlen("HA") ) ) {

	/*
	  Read the parameter to either set or unset the homing trigger
	*/
	parameter = strtok_r( NULL, " ", &tok_ptr);

	if ( parameter == NULL ) {

	  printf("  Source_MVP: No flag sent with Set Home Trigger request\n");
	  fflush(stdout);

	} else {

	  home_trigger = atoi(parameter);

	  if ( *debug ) {
	    printf("  Source_MVP: %s home trigger set to %d\n", Socket->name, home_trigger);
	    fflush(stdout);
	  }

	}

      } else if ( !strncmp ( action, "HF", strlen("HF") ) ) {

	if ( *debug ) {
	  printf("  Source_MVP: %s hard stop flag set\n", Socket->name);
	  fflush(stdout);
	}

      } else if ( !strncmp ( action, "LR", strlen("LR") ) ) {

	/*
	  See if there is a parameter to the LR request.  If there is, then
	    this integer is the request distance to move and should be placed
	    in the variable distance.  If not, then the LR request is just
	    querying the value of the LR distance variable.
	*/
	parameter = strtok_r( NULL, " ", &tok_ptr);

	if ( parameter == NULL ) {

	  /*
	    Send back the value of the LR distance variable (in hex format)
	  */
	  sprintf( reply, "01234567\n  %12x\r", relative);

	  /*
	    Send the request
	  */
	  if ( Socket_StringWrite( newsockfd, reply, strlen(reply)) != strlen(reply) ) {
	    if ( *debug ) {
	      printf("  Source_MVP: Error writing LR relative distance in Socket_StringWrite\n");
	      fflush(stdout);
	    }
	  }

	} else {

	  relative = atoi(parameter);
	  absolute = 0.0;

	  if ( *debug ) {
	    printf("  Source_MVP: Relative move request = %8.2f\n", (double)relative/Data->gain);
	    fflush(stdout);
	  }

	}

      } else if ( !strncmp ( action, "LA", strlen("LA") ) ) {

	/*
	  See if there is a parameter to the LR request.  If there is, then
	    this integer is the request distance to move and should be placed
	    in the variable distance.  If not, then the LR request is just
	    querying the value of the LR distance variable.
	*/
	parameter = strtok_r( NULL, " ", &tok_ptr);

	if ( parameter == NULL ) {

	  /*
	    Send back the value of the LR distance variable (in hex format)
	  */
	  sprintf( reply, "01234567\n  %12x\r", absolute);

	  /*
	    Send the request
	  */
	  if ( Socket_StringWrite( newsockfd, reply, strlen(reply)) != strlen(reply) ) {
	    if ( *debug ) {
	      printf("  Source_MVP: Error writing LA absolute distance in Socket_StringWrite\n");
	      fflush(stdout);
	    }
	  }

	} else {

	  absolute = atoi(parameter);
	  relative = 0.0;

	  if ( *debug ) {
	    printf("  Source_MVP: Absolute move request = %8.2f\n", (double)absolute/Data->gain);
	    fflush(stdout);
	  }

	}

      } else if ( !strncmp ( action, "SP", strlen("SP") ) ) {

	/*
	  See if there is a parameter to the SP request.  If there is, then
	    this integer is the request speed for the device to move, so put it
	    in the Data structure as speed.  If not, then the SP request is just
	    querying the value of the SP speed variable.
	*/
	parameter = strtok_r( NULL, " ", &tok_ptr);

	if ( parameter == NULL ) {

	  /*
	    Send back the value of the SP speed variable (in hex format)
	  */
	  sprintf( reply, "01234567\n  %12x\r", Data->speed);

	  /*
	    Send the request
	  */
	  if ( Socket_StringWrite( newsockfd, reply, strlen(reply)) != strlen(reply) ) {
	    if ( *debug ) {
	      printf("  Source_MVP: Error writing SP speed value in Socket_StringWrite\n");
	      fflush(stdout);
	    }
	  }

	} else {

	  Data->speed = atoi(parameter);

	  if ( *debug ) {
	    printf("  Source_MVP: %s speed set to %d\n", Socket->name, Data->speed);
	    fflush(stdout);
	  }

	}

      } else if ( !strncmp ( action, "AC", strlen("AC") ) ) {

	/*
	  See if there is a parameter to the AC request.  If there is, then
	    this integer is the request acceleration for the device, so put it
	    in the Data structure as accel.  If not, then the AC request is just
	    querying the value of the AC acceleration variable.
	*/
	parameter = strtok_r( NULL, " ", &tok_ptr);

	if ( parameter == NULL ) {

	  /*
	    Send back the value of the AC acceleration variable (in hex format)
	  */
	  sprintf( reply, "01234567\n  %12x\r", Data->accel);

	  /*
	    Send the request
	  */
	  if ( Socket_StringWrite( newsockfd, reply, strlen(reply)) != strlen(reply) ) {
	    if ( *debug ) {
	      printf("  Source_MVP: Error writing AC acceleration value in Socket_StringWrite\n");
	      fflush(stdout);
	    }
	  }

	} else {

	  Data->accel = atoi(parameter);

	  if ( *debug ) {
	    printf("  Source_MVP: %s acceleration set to %d\n", Socket->name, Data->accel);
	    fflush(stdout);
	  }

	}

      } else if ( !strncmp ( action, "M", strlen("M") ) ) {

	if ( *debug ) {
	  printf("  Source_MVP: Request = %s\n", action);
	  fflush(stdout);
	}

	if ( *(Local_Info.running) ) {

	  printf("  Source_MVP: Thread to make a relative move for %s already running\n", Socket->name);
	  fflush(stdout);

	} else {

	  /*
	    Only a relative motion is done, so must convert an absolute motion request to
	      a relative move before Source_RelativeMVP() is called below.
	  */
	  if ( relative != 0 ) {

	    /*
	      Nothing to do; parameters for Source_RelativeMVP() are set
	    */

	  } else if ( absolute != 0 ) {

	    relative = absolute - (int) (Data->position * Data->gain);

	  } else {

	    printf("  Source_MVP: Neither relative nor absolute parameter set\n");
	    printf("              Request to move %s ignored\n", Socket->name);
	    fflush(stdout);
	    continue;

	  }

	  /*=====================================================================
	    Start thread to move the device the given relative distance
	    =======================================================================*/
	  status = pthread_create( &mvpThread, &attr, Source_RelativeMVP, (void *)&Local_Info);
	  if ( local_debug ) {
	    printf("  Source_MVP: Starting thread to make a relative move of %s\n", Socket->name);
	    fflush(stdout);
	  }
	  if ( status ) {
	    printf("  Source_MVP: Error starting thread to make a relative move of %s\n", Socket->name);
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
    printf("  Source_MVP: Data Server Shutdown\n");
    fflush(stdout);
  }

  Socket_Close( (Socket_Info *)&Socket, *debug);
  return( (void *)0);
}
