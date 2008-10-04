/*
  Source.c

  This routine starts a test data server which acts like the PCR

  Written  20nov05  DLM  Started with DServ.c

*/

#include "PCR.h"
#include "DServ.h"
#include "Server_Ports.h"
#include "PCR_Error.h"

#define ERROR_QUIT -9

#define DEBUG 1

int Source_Stream_WFSC( DServ_Info *Info, char *type, int size, int debug );
int Source_Stream_DM( DServ_Info *Info, int debug );

/*================================================================================*
 * Start server to manage data servers
 *================================================================================*/
int Source_Start( char *name, char *type, int size, int debug )
{
  int status;
  int newsockfd;
  int continueRunning = 1;
  int continueListening = 0;
  int notConnected;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  float Slope_Offsets[288];
  float Raw_Offsets[288];
  short BG[576];
  short Raw_BG[576];
  float *Mod_Rate;
  float *Raw_Rate;
  char *charptr;
  int i, j;
  int vec_length;
  int errorNumber;
  char *intPtr;
  char answer[STRING_LENGTH];

  /*
    Socket structures
  */
  DServ_Info Master_Info;

  /*
    Socket structure
  */
  Socket_Info Master_Socket;

  /*
    Data structure
  */
  DServ_Data Master_Data;

  /*
    Variable to fork() server process
  */
  //  int childpid;

  /*
    Client connection information
  */
  struct sockaddr_in cli_addr;
  socklen_t clilen;

  /*
    Variables needed to use the select commands to watch for data on a port
  */
  fd_set rfds;
  struct timeval tv;

  /*
    Put Socket and Data structures together
  */
  Master_Info.socket = &Master_Socket;
  Master_Info.data = &Master_Data;

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  strcpy( request, name);
  status = Socket_StringLowerCase(request);

  /*
    Fill in information about the Master Server
  */
  if ( !strncmp (request, "cmd", strlen(request) ) ) {
    strcpy ( Master_Socket.name, "CMD");
    Master_Socket.port = PCR_CMD_PORT;
  } else if ( !strncmp (request, "wfsc", strlen(request) ) ) {
    strcpy ( Master_Socket.name, "WFSC");
    Master_Socket.port = PCR_WFSC_PORT;
    Master_Data.wfsc_bytes = size*size;
    Master_Data.total_bytes = Master_Data.wfsc_bytes*sizeof(short) + HEADER_BYTES;
  } else if ( !strncmp (request, "dm", strlen(request) ) ) {
    strcpy ( Master_Socket.name, "DM");
    Master_Socket.port = PCR_DM_PORT;
    Master_Data.wfsc_bytes = TOTAL_DM_VALUES;
    Master_Data.total_bytes = TOTAL_DM_BYTES;
  } else if ( !strncmp (request, "error", strlen(request) ) ) {
    strcpy ( Master_Socket.name, "ERROR");
    Master_Socket.port = PCR_ERROR_PORT;
  } else {
    printf("  Source_Start: Unkown data type: %s\n", name);
    fflush(stdout);
    return(0);
  }
  Master_Socket.sockfd = -1;
  Master_Socket.connected = 0;
  Master_Socket.error = 0;

  /*
    Open a TCP socket (an Internet stream socket) and bind
      the local address so a client can connect and send request
  */
  if ( Socket_OpenServer( (Socket_Info *)&Master_Socket, debug) ) {
    printf("  Source_Start: Error opening %s socket\n", Master_Socket.name);
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
    if( newsockfd < 0 ) {
      printf("  Source_Start: accept error\n" );
      fflush(stdout);
      continueRunning = 0;
      continue;
    }

    if ( debug ) {
      printf("  Source_Start: %s Connection...\n", name);
      fflush(stdout);
    }

    if ( !strncmp ( Master_Socket.name, "ERROR", strlen("ERROR") ) ) {

      /*
	Send PCR_OK to the client so it know the connects has been properly
	  make.
      */
      errorNumber = PCR_OK;
      intPtr = (char *)&errorNumber;
      if ( Socket_Write( newsockfd, intPtr, sizeof(int)) ) {
	if ( debug ) {
	  printf("  Source_Start: Error writing PCR_OK in Socket_Write\n");
	  fflush(stdout);
	}
      }

    } else {
      /*
	Read an instruction from the client
      */

      if ( Socket_StringRead( newsockfd, request) ) {
	if ( debug ) {
	  printf("  Source_Start: Error reading request in Socket_StringRead\n");
	  fflush(stdout);
	}
	close(newsockfd);
	continue;
      }

      /*
	Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
      */
      //    status = Socket_StringLowerCase(request);

      if ( debug && strncmp ( request, "all", strlen("all") ) ) {
	printf("  Source_Start: Request = %s\n", request);
	fflush(stdout);
      }

      if ( !strncmp ( request, "all", strlen("all") ) ) {
	printf("  Source_Start: Status looper is running\n");
	fflush(stdout);
      }
    }

    if ( !strncmp ( Master_Socket.name, "CMD", strlen("CMD") ) ) {

      do {

        if ( !strncmp ( request, "all", strlen("all") ) ) {

	  /*
	    Send a reply to the client
	  */
	  strcpy( reply, "Recon_Name 56\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "WFSC_Initialized 1\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "WFSC_Running 1\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "WFSC_Size Bin\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "WFSC_Period 0.0018\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "WFSC_Frequency 550\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "WFSC_Source Camera\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "WFSC_Bias_0 100\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "WFSC_Bias_1 200\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "WFSC_Bias_2 300\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "WFSC_Bias_3 400\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "WFSC_Temp_1 25.0\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "WFSC_Temp_2 12.2\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "Static_Running 0\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "Modulation_Running 0\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "Background_Dynamic 0\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "Background_Static 0\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "Loop_Running 1\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "Loop_Gain 0.0\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );
	  strcpy( reply, "EOF\n");
	  Socket_StringWrite( newsockfd, reply, strlen(reply) );

	} else if ( !strncmp ( request, "cam_bg_set", strlen("cam_bg_set") ) ) {

          /*
            Read pixel background array from the client
          */

          charptr = (char *)Raw_BG;
        
	  if ( Socket_Read( newsockfd, charptr, 576*sizeof(short)) ) {
	    if ( debug ) {
	      printf("  Source_Start: Error reading Background array in Socket_Read\n");
	      fflush(stdout);
	    }
	    strcpy( request, "quit");
  	  }

          if ( debug ) {
            printf("  Source_Start:           576 Background Values Read\n");
	    fflush(stdout);
          }

	  for ( i=0; i<576; i++) {
	    BG[i] = ntohs(Raw_BG[i]);
	  }

          if ( DEBUG ) {
            printf("\n Pixel Background\n");
            printf("---------------------------\n");
            for ( i=0; i<24; i++) {
              for ( j=0; j<24; j++) {
                printf("%5d", BG[i*24+j]);
              }
              printf("\n");
            }
	    fflush(stdout);
          }

	} else if ( !strncmp ( request, "slope_offset_set", strlen("slope_offset_set") ) ) {

          /*
            Read float Slope_Offsets[288] array from the client
          */

          charptr = (char *)Raw_Offsets;
        
	  if ( Socket_Read( newsockfd, charptr, 288*sizeof(float)) ) {
	    if ( debug ) {
	      printf("  Source_Start: Error reading Offsets array in Socket_Read\n");
	      fflush(stdout);
	    }
	    strcpy( request, "quit");
  	  }

          if ( debug ) {
            printf("  Source_Start:           288 Slope Offsets Read\n");
	    fflush(stdout);
          }

#ifdef MAC
	  status = Socket_ConvertFloat( Raw_Offsets, Slope_Offsets, 288);
#else
	 for ( i=0; i<288; i++) {
	   Slope_Offsets[i] = Raw_Offsets[i];
	 }
#endif

          if ( DEBUG ) {
            printf("\n Static_Calc\n");
            printf("---------------------------\n");
            printf("    X-Slopes\n");
            printf("    --------\n");
            for ( i=0; i<12; i++) {
              for ( j=0; j<12; j++) {
                printf("%10.2e", Slope_Offsets[i*12+j]);
              }
              printf("\n");
            }
            printf("\n    Y-Slopes\n");
            printf(  "    --------\n");
            for ( i=0; i<12; i++) {
              for ( j=0; j<12; j++) {
                printf("%10.2e", Slope_Offsets[144+i*12+j]);
              }
              printf("\n");
            }
	    fflush(stdout);
          }

        } else if ( !strncmp ( request, "mod_offset_set", strlen("mod_offset_set") ) ) {

          /*
            Read float Slope_Offsets[288] array from the client
          */

          charptr = (char *)Raw_Offsets;
        
	  if ( Socket_Read( newsockfd, charptr, 288*sizeof(float)) ) {
	    if ( debug ) {
	      printf("  Source_Start: Error reading Offsets array in Socket_Read\n");
	      fflush(stdout);
	    }
	    strcpy( request, "quit");
  	  }

          if ( debug ) {
            printf("  Source_Start:           288 Slope Offsets Read\n");
	    fflush(stdout);
          }

#ifdef MAC
	  status = Socket_ConvertFloat( Raw_Offsets, Slope_Offsets, 288);
#else
	 for ( i=0; i<288; i++) {
	   Slope_Offsets[i] = Raw_Offsets[i];
	 }
#endif

          if ( DEBUG ) {
            printf("\n Mod_Calc\n");
            printf("---------------------------\n");
            printf("    X-Slopes\n");
            printf("    --------\n");
            for ( i=0; i<12; i++) {
              for ( j=0; j<12; j++) {
                printf("%10.2e", Slope_Offsets[i*12+j]);
              }
              printf("\n");
            }
            printf("\n    Y-Slopes\n");
            printf(  "    --------\n");
            for ( i=0; i<12; i++) {
              for ( j=0; j<12; j++) {
                printf("%10.2e", Slope_Offsets[144+i*12+j]);
              }
              printf("\n");
            }
	    fflush(stdout);
          }

	} else if ( !strncmp ( request, "mod_rate_set", strlen("mod_rate_set") ) ) {

	  /*
	    Read an instruction from the client
	  */

	  if ( Socket_StringRead( newsockfd, request) ) {
	    if ( debug ) {
	      printf("  Source_Start: Error reading modulation vector length in Socket_StringRead\n");
	      fflush(stdout);
	    }
	    continue;
	  }
	  vec_length = atol(request);

	  /*
	    Allocate memory needed for Modulation Rate
	  */

	  Mod_Rate = (float *)malloc(vec_length*sizeof(float));
	  Raw_Rate = (float *)malloc(vec_length*sizeof(float));

          /*
            Read float Mod_Rate[vec_length] array from the client
          */

          charptr = (char *)Raw_Rate;
        
	  if ( Socket_Read( newsockfd, charptr, vec_length*sizeof(float)) ) {
	    if ( debug ) {
	      printf("  Source_Start: Error reading Offsets array in Socket_Read\n");
	      fflush(stdout);
	    }
	    strcpy( request, "quit");
  	  }

          if ( debug ) {
            printf("  Source_Start:           %d Modulation Rate values Read\n", vec_length);
	    fflush(stdout);
          }

#ifdef MAC
	  status = Socket_ConvertFloat( Raw_Rate, Mod_Rate, vec_length);
#else
	 for ( i=0; i<vec_length; i++) {
	   Mod_Rate[i] = Raw_Rate[i];
	 }
#endif

          if ( DEBUG ) {
            printf("\n Mod_Rate\n");
            printf("---------------------------\n");
            for ( i=0; i<vec_length; i++) {
	      printf("%8.2f\n", Mod_Rate[i]);
            }
	    fflush(stdout);
          }
	  free(Mod_Rate);
	  free(Raw_Rate);
        }

        if ( strncmp ( request, "all", strlen("all") ) ) {
	  /*
	    Send a reply to the client
	  */
	  if ( Socket_StringWrite( newsockfd, "OK\n", strlen("OK\n") ) != strlen("OK\n") ) {
	    if ( debug ) {
	      printf("  Source_Start: Error sending reply in Socket_StringWrite\n");
	      fflush(stdout);
	    }
	    strcpy( request, "quit");
	  }
	}

	/*
	  Read an instruction from the client
	*/
	if ( Socket_StringRead( newsockfd, request) ) {
	  if ( debug ) {
	    printf("  Source_Start: Error reading request in Socket_StringRead\n");
	    fflush(stdout);
	  }
	  strcpy( request, "quit");
	}

	if ( debug && strncmp ( request, "all", strlen("all") ) ) {
	  printf("  Source_Start: Request = %s\n", request);
	  fflush(stdout);
	}

      } while ( strncmp( request, "quit", strlen("quit")) );
      close(newsockfd);
      continue;

    } else if ( !strncmp ( Master_Socket.name, "ERROR", strlen("ERROR") ) ) {

      /*
	Flag indicating that we still have a connection
      */
      continueListening = 1;

      /*
	Read an integer from the terminal to send to the PCR_Server.  This
	  is a continuous loop until "Quit" is found.
      */
      do {

	printf("Enter an error number (integer) =>");
	fflush(stdout);

	do {
	  /*
	    Watch to see when there is data to be read on sockfd
	  */
	  FD_ZERO(&rfds);
	  FD_SET( 0, &rfds);
	  //	  FD_SET( *(int *)stdin, &rfds);
	  /* Wait up to one seconds. */
	  tv.tv_sec = 1;
	  tv.tv_usec = 0;
 
	  status = select( 1, &rfds, NULL, NULL, &tv);
	  //	  status = select( *(int *)stdin + 1, &rfds, NULL, NULL, &tv);

	  /*
	    Watch to see when there is data to be read on sockfd
	  */
	  FD_ZERO(&rfds);
	  FD_SET( newsockfd, &rfds);
	  /* Wait up to one seconds. */
	  tv.tv_sec = 0;
	  tv.tv_usec = 0;
 
	  notConnected = select( newsockfd+1, &rfds, NULL, NULL, &tv);

	  if ( notConnected ) {
	    continueListening = 0;
	    printf("\n  Source_Start: Connection Closed\n");
	    fflush(stdout);
	  }

	} while ( !status && !notConnected);
	
	if ( status ) {

	  /*
	    A request is read to be read, so read it and send it
	  */
	  fgets(answer, sizeof(answer), stdin);
	  errorNumber = atoi(answer);

	  printf("  Source_Start: Error Number = %d\n", errorNumber);
	  fflush(stdout);

	  if ( errorNumber == ERROR_QUIT ) {

	    printf("  quiting\n");
	    fflush(stdout);

	    continueListening = 0;
	    continueRunning = 0;

	  } else {

	    errorNumber = htonl(errorNumber);

	    /*
	      Send this error number to the PCR_Server
	    */
	    if ( Socket_Write( newsockfd, intPtr, sizeof(int)) ) {
	      if ( debug ) {
		printf("  Source_Start: Error writing error number in Socket_Write\n");
		fflush(stdout);
	      }
	      continueListening = 0;
	    }
	  }
	}

      } while ( continueListening );

      close( newsockfd );

    } else {

      if ( !strncmp (request, "start_WFSC", strlen(request) ) ) {

        /*******************************************************
        * Setup to start a WFSC or DM data server
        *******************************************************/
	/*
        if ( (childpid = fork()) < 0) {
    	  printf("Source_Start: fork error\n");
	  fflush(stdout);
        } else if (childpid == 0) {
	*/
	  /*
	    This is the child process so we can close the socket the server
	      is listening to.  Plus, we need to send the current client
	      socket to the server so we can pass data
	  */
	//	  close(Master_Socket.sockfd);
	printf("  Source Start: starting Source_Steam_WFSC\n");
	  Master_Socket.sockfd = newsockfd;
	  status = Source_Stream_WFSC( &Master_Info, type, size, debug);

	  //       }

        /*
	  Close the client connection because the child process is now
	    interacting with the client.
        */
        close(newsockfd);

      } else if ( !strncmp (request, "start_DM", strlen(request) ) ) {

        /*******************************************************
        * Setup to start a WFSC or DM data server
        *******************************************************/
	/*
        if ( (childpid = fork()) < 0) {
    	  printf("Source_Start: fork error\n");
	  fflush(stdout);
        } else if (childpid == 0) {
	*/
	  /*
	    This is the child process so we can close the socket the server
	      is listening to.  Plus, we need to send the current client
	      socket to the server so we can pass data
	  */
	//	  close(Master_Socket.sockfd);
	  Master_Socket.sockfd = newsockfd;
	  status = Source_Stream_DM( &Master_Info, debug);

	  //        }

        /*
	  Close the client connection because the child process is now
	    interacting with the client.
        */
        close(newsockfd);

      } else if ( !strncmp (request, "Quit", strlen(request) ) ) {

        /*******************************************************
         * Stop this server
         *******************************************************/

        close(newsockfd);
        continueRunning = 0;
        continue;

      }

    }

    /*******************************************************
     * Start the requested server
     *******************************************************/

  } while ( continueRunning );

  if ( debug ) {
    printf("  Source_Start: Data Server Shutdown\n");
    fflush(stdout);
  }

  Socket_Close( (Socket_Info *)&Master_Socket, debug);
  return(0);
}
