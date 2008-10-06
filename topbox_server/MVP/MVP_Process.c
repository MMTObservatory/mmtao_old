/*
  MVP_Process.c

  This routine starts a data server for either WFSC or DM Data

  Written  13aug07  DLM  Started with PCR_Process.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*================================================================================*
 * Accepts requests to start a data server
 *================================================================================*/
int MVP_Process( Device_Info *Info, char *request)
{

  /*
    Local variables
  */
  int status;
  char *device, *action, *parameter;
  float distance;
  int speed, accel, home;
  char Request[STRING_LENGTH];

  /*
    Local pointers to varible passed from the process which created this thread
   */
  Socket_Info *Socket;
  Device_Data *Data;
  int *debug;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  /*
    Put the passed variables in to the local variables
  */
  Socket = Info->socket;
  Data = Info->data;
  debug = Info->cmd_debug;

  /*
    Device Structures: contains all device information.
  */
  pthread_t mvpThread;
  static Device_Info Local_Info;

  Local_Info.socket = Socket;
  Local_Info.data = Data;
  Local_Info.running = (int *)malloc( sizeof(int));
  *(Local_Info.running) = 0;
  Local_Info.cmd_debug = debug;

  if ( *debug ) {
    printf("  MVP_Process: Request = %s\n", request);
  }

  /*
    Check if we are connected to this device.  If not, display an error message
      and return.  We do not attempt to connect to the device here because the
      MVP_Looper for this device is continually trying to connect.
  */
  if ( !Socket->connected ) {
    printf("  MVP_Process: Not connected to %s\n", Socket->name);
    printf("               Check that the device has power and the Topbox Cyclades server is running\n");
    fflush(stdout);
    return(-1);
  }

  strcpy( Request, request);

  device = strtok( Request, " ");
  action = strtok( NULL, " ");
  if ( action == NULL ) {
    printf("  MVP_Process: Action parameter not included in request\n");
    printf("               Request = %s\n", request);
    fflush(stdout);
    return(-1);
  }

  /*
    Take the appropriate aciton
  */
  if ( !strncmp (action, "?", strlen("?") ) ||
       !strncmp (action, "help", strlen("help") ) ) {

    //    status = MVP_SendHelp( sockfd);
    if ( status ) {
      printf("  MVP_Process: Error returning help options\n");
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

      printf("  MVP_Process: Unknown debug paramter = %s\n", parameter);
      fflush(stdout);

    }

  } else if ( !strncmp (action, "abort", strlen("abort") ) ) {

    /*
      Abort the current move and disable the device
    */
    status = MVP_Abort( Socket, Data, *debug);

  } else if ( !strncmp (action, "reset", strlen("reset") ) ) {

    /*
      Reset the given MVP controller
    */
    status = MVP_ResetNode( Socket, Data, *debug);

  } else if ( !strncmp (action, "home", strlen("home") ) ) {

    if ( *(Local_Info.running) ) {

      printf("  MVP_Process: Thread to home %s already running\n", Socket->name);
      fflush(stdout);

    } else {

      /*=====================================================================
       * Start thread to home the requested device
       *=====================================================================*/
      status = pthread_create( &mvpThread, &attr, MVP_Home, (void *)&Local_Info);
      if ( debug ) {
	printf("  MVP_Process: Starting thread to home %s\n", Socket->name);
	fflush(stdout);
      }
      if ( status ) {
	printf("  MVP_Process: Error starting thread to home %s\n", Socket->name);
	fflush(stdout);
      }

    }

  } else if ( !strncmp (action, "enable", strlen("en") ) ) {

    /*
      Check to see if the device is already enabled
    */
    if ( Data->enabled ) {

      printf("  MVP_Process: %s is already enabled\n", Socket->name);
      fflush(stdout);

    } else {

      /*
	Request the device to move a relative distance 0.0.  This reset
	  all the move parameter in the MVP controller so it reads that
	  its in position, so the device can be enabled.
      */
      status = MVP_MoveRelative( Socket, Data, 0.0, *debug);

      /*
	Set the enable flag on the request device, allowing motion
      */
      status = MVP_Enable( Socket, Data, *debug);

    }

  } else if ( !strncmp (action, "disable", strlen("di") ) ) {

    /*
      Check to see if the device is already disabled
    */
    if ( !Data->enabled ) {

      printf("  MVP_Process: %s is already disabled\n", Socket->name);
      fflush(stdout);

    } else {

      /*
	Set the enable flag on the request device.
      */
      status = MVP_Disable( Socket, Data, *debug);

    }

  } else if ( !strncmp (action, "position", strlen("pos") ) ) {

    /*
      Request the current position of the given device
    */
    status = MVP_Position( Socket, Data, *debug);

  } else if ( !strncmp (action, "zero", strlen("zero") ) ) {

    /*
      Set the current position to zero (0.0)
    */
    status = MVP_SetZero( Socket, Data, *debug);

  } else if ( !strncmp (action, "relative", strlen("rel") ) ) {

    /*
      Read the requested distance from the request string
    */
    parameter = strtok( NULL, " ");
    if ( parameter == NULL ) {
      printf("  MVP_Process: Distance parameter not included in request\n");
      printf("               Request = %s\n", request);
      fflush(stdout);
      return(-1);
    }

    distance = atof(parameter);

    /*
      Request the device to move a relative distance
    */
    status = MVP_MoveRelative( Socket, Data, distance, *debug);

  } else if ( !strncmp (action, "absolute", strlen("abs") ) ) {

    /*
      Read the requested distance from the request string
    */
    parameter = strtok( NULL, " ");
    if ( parameter == NULL ) {
      printf("  MVP_Process: Distance parameter not included in request\n");
      printf("               Request = %s\n", request);
      fflush(stdout);
      return(-1);
    }

    distance = atof(parameter);

    /*
      Request the device to move an absolute distance
    */
    status = MVP_MoveAbsolute( Socket, Data, distance, *debug);

  } else if ( !strncmp (action, "speed", strlen("speed") ) ) {

    /*
      Read the requested distance from the request string
    */
    parameter = strtok( NULL, " ");
    if ( parameter == NULL ) {
      printf("  MVP_Process: Speed parameter not included in request\n");
      printf("               Request = %s\n", request);
      fflush(stdout);
      return(-1);
    }

    speed = atoi(parameter);

    /*
      Request the device to set the speed of the device
    */
    status = MVP_SetSpeed( Socket, Data, speed, *debug);

  } else if ( !strncmp (action, "accel", strlen("accel") ) ) {

    /*
      Read the requested distance from the request string
    */
    parameter = strtok( NULL, " ");
    if ( parameter == NULL ) {
      printf("  MVP_Process: Acceleration parameter not included in request\n");
      printf("               Request = %s\n", request);
      fflush(stdout);
      return(-1);
    }

    accel = atoi(parameter);

    /*
      Request the device to set the accleration of the device
    */
    status = MVP_SetAccel( Socket, Data, accel, *debug);

  } else if ( !strncmp (action, "hflag", strlen("hflag") ) ) {

    /*
      Read the requested home flag from the request string
    */
    parameter = strtok( NULL, " ");
    if ( parameter == NULL ) {
      printf("  MVP_Process: Home parameter not included in request\n");
      printf("               Request = %s\n", request);
      fflush(stdout);
      return(-1);
    }

    home = atoi(parameter);

    /*
      Request the device to set it home flag
    */
    status = MVP_SetHomeFlag( Socket, Data, home, *debug);

  } else if ( !strncmp (action, "get_hflag", strlen("get") ) ) {

    /*
      Get the home flag for the requested device
    */
    status = MVP_HomeFlag( Socket, Data, *debug);

  } else {

    printf("  MVP_Process: Unknown action = %s\n", action);

  }

  return(0);
}
