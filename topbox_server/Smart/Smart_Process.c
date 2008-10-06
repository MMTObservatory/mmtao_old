/*
  Smart_Process.c

  This routine starts a data server for either WFSC or DM Data

  Modified  24aug07  tjt  Started with Stage_Process.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Smart.h"

/*================================================================================*
 * Accepts requests to start a data server
 *================================================================================*/
int
Smart_Process( Device_Info *Info, char *request )
{
  /* Local variables
   */
  int status;
  char *device, *action, *parameter;
  float distance;
  char Request[STRING_LENGTH];

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  /* Local pointers to varible passed from the process which created this thread
   */
  Socket_Info *Socket;
  Device_Data *Data;
  int *debug;

  /* parse the request string */
  strcpy( Request, request);

  device = strtok( Request, " ");
  action = strtok( NULL, " ");
  if ( action == NULL ) {
    printf("  Smart_Process: Action parameter not included in request\n");
    printf("               Request = %s\n", request);
    fflush(stdout);
    return(-1);
  }


  /* Put the passed variables in to the local variables
   */
  Socket = Info->socket;
  debug = Info->cmd_debug;

  /* Figure out which device we are dorking with */
  if ( strcmp ( device, "focus" ) == 0 )
      Data = Info->data;
  else
      Data = Info->data2;

  /* Device Structures: contains all device information.
  */
  pthread_t mvpThread;
  static Device_Info Local_Info;

  Local_Info.socket = Socket;
  Local_Info.data = Data;
  Local_Info.running = (int *)malloc( sizeof(int));
  *(Local_Info.running) = 0;
  Local_Info.cmd_debug = debug;

  if ( *debug ) {
    printf("  Smart_Process: Request = %s\n", request);
  }

  /*
    Check if we are connected to this device.  If not, display an error message
      and return.  We do not attempt to connect to the device here because the
      Smart_Looper for this device is continually trying to connect.
  */
  if ( !Socket->connected ) {
    printf("  Smart_Process: Not connected to %s\n", Socket->name);
    printf("               Check that the device has power and the Topbox Cyclades server is running\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Take the appropriate aciton
  */
  if ( !strncmp (action, "?", strlen("?") ) ||
       !strncmp (action, "help", strlen("help") ) ) {

    //    status = Smart_SendHelp( sockfd);
    if ( status ) {
      printf("  Smart_Process: Error returning help options\n");
      fflush(stdout);
    }

  } else if ( !strncmp( action, "debug", strlen("debug") ) ) {

    /*
      Parse the debug value to set
    */
    parameter = strtok( NULL, " ");
    if ( parameter == NULL ) {
      printf("  Master_Process: Debug parameter not included in request\n");
      printf("                  Request = %s\n", request);
      fflush(stdout);
      return(-1);
    }

    /*
      Set the appropriate debug parameter to the requested value
    */
    if ( !strncmp( parameter, "on", strlen("on")) ) {

      *debug = 1;

    } else if ( !strncmp( parameter, "off", strlen("off")) ) {

      *debug = 0;

    } else {

      printf("  Smart_Process: Unknown debug paramter = %s\n", parameter);
      fflush(stdout);

    }

  } else if ( !strncmp (action, "home", strlen("home") ) ) {

    if ( *(Local_Info.running) ) {

      printf("  Smart_Process: Thread to home %s already running\n", Socket->name);
      fflush(stdout);

    } else {

      /*=====================================================================
       * Start thread to home the requested device
       *=====================================================================*/
      status = pthread_create( &mvpThread, &attr, Smart_Home, (void *)&Local_Info);
      if ( debug ) {
	printf("  Smart_Process: Starting thread to home %s\n", Socket->name);
	fflush(stdout);
      }
      if ( status ) {
	printf("  Smart_Process: Error starting thread to home %s\n", Socket->name);
	fflush(stdout);
      }

    }

  } else if ( !strncmp (action, "position", strlen("pos") ) ) {

    /*
    status = Smart_Position( Socket, Data, *debug);
    */

  } else if ( !strncmp (action, "status", strlen("sta") ) ) {

    /*
      Request the current status of the given stage
    */
    status = Smart_PrintDevice( Socket, Data);

  } else if ( !strncmp (action, "relative", strlen("rel") ) ) {

    /*
      Read the requested distance from the request string
    */
    parameter = strtok( NULL, " ");
    if ( parameter == NULL ) {
      printf("  Smart_Process: Distance parameter not included in request\n");
      printf("               Request = %s\n", request);
      fflush(stdout);
      return(-1);
    }

    distance = atof(parameter);

    /*
      Request the device to move a relative distance
    */
    status = Smart_MoveRelative( Socket, Data, distance, *debug);

  } else if ( !strncmp (action, "absolute", strlen("abs") ) ) {

    /*
      Read the requested distance from the request string
    */
    parameter = strtok( NULL, " ");
    if ( parameter == NULL ) {
      printf("  Smart_Process: Distance parameter not included in request\n");
      printf("                 Request = %s\n", request);
      fflush(stdout);
      return(-1);
    }

    distance = atof(parameter);

    /*
      Request the device to move an absolute distance
    */
    status = Smart_MoveAbsolute( Socket, Data, distance, *debug);

  } else if ( !strncmp (action, "speed", strlen("speed") ) ) {

    int speed;
    parameter = strtok( NULL, " ");
    if ( parameter == NULL ) {
      printf("  Smart_Process: Speed parameter not included in request\n");
      printf("                 Request = %s\n", request);
      fflush(stdout);
      return(-1);
    }

    speed = atof(parameter);

    /*
      set the speed of the device
    */
    status = Smart_SetSpeed( Socket, Data, speed, *debug);

  } else {

    printf("  Smart_Process: Unknown action = %s\n", action);

  }

  return(0);
}

/* THE END */
