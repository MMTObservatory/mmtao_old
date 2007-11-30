/*
  Power_Process.c

  This routine starts a data server for either WFSC or DM Data

  Modified  24aug07  tjt  Started with Stage_Process.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Power.h"

static int
Power_Relay ( Socket_Info *Socket, int who, char *what, int debug )
{
    if ( !strncmp( what, "on", strlen("on")) )
      return Power_On ( Socket, who, debug );
    else
      return Power_Off ( Socket, who, debug );
}

int
Power_Process( Device_Info *Info, char *request)
{
  /* Local variables
   */
  int status;
  char *device, *action, *parameter;
  char Request[STRING_LENGTH];

  /* Local pointers to varible passed from the process which created this thread
   */
  Socket_Info *Socket;
  Device_Data *Data;
  int *debug;

  /* Put the passed variables in to the local variables
  */
  Socket = Info->socket;
  Data = Info->data;
  debug = Info->cmd_debug;

  /* Device Structures: contains all device information.
  */
  static Device_Info Local_Info;

  Local_Info.socket = Socket;
  Local_Info.data = Data;
  Local_Info.running = (int *)malloc( sizeof(int));
  *(Local_Info.running) = 0;
  Local_Info.cmd_debug = debug;

  if ( *debug ) {
    printf("  Power_Process: Request = %s\n", request);
  }

  /*
    Check if we are connected to this device.  If not, display an error message
      and return.  We do not attempt to connect to the device here because the
      Power_Looper for this device is continually trying to connect.
  */
  if ( !Socket->connected ) {
    printf("  Power_Process: Not connected to %s\n", Socket->name);
    printf("               Check that the device has power and the Topbox Cyclades server is running\n");
    fflush(stdout);
    return(-1);
  }

  strcpy( Request, request);

  /* We never use device, but we have to get
   * it out of the way.  Also it doesn't hurt
   * to keep asking for stuff that isn't there,
   * you just get NULL back
   */
  device = strtok( Request, " ");
  action = strtok( NULL, " ");
  parameter = strtok( NULL, " ");

  if ( action == NULL ) {
    printf("  Power_Process: Action parameter not included in request\n");
    printf("               Request = %s\n", request);
    fflush(stdout);
    return(-1);
  }

  /*
    Take the appropriate aciton
  */
  if ( !strncmp (action, "?", strlen("?") ) ||
       !strncmp (action, "help", strlen("help") ) ) {

    //    status = Power_SendHelp( sockfd);
    // if ( status ) {
    //   printf("  Power_Process: Error returning help options\n");
    //   fflush(stdout);
    // }

  } else if ( !strncmp( action, "debug", strlen("debug") ) ) {

    /*
      Parse the debug value to set
    */
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
      printf("  Power_Process: Unknown debug parameter = %s\n", parameter);
      fflush(stdout);
    }

  } else if ( !strncmp( action, "wfsc", strlen("wfsc") ) ) {
    status = Power_Relay ( Socket, 0, parameter, *debug );
  } else if ( !strncmp( action, "peltier", strlen("peltier") ) ) {
    status = Power_Relay ( Socket, 1, parameter, *debug );
  } else if ( !strncmp( action, "stella", strlen("stella") ) ) {
    status = Power_Relay ( Socket, 2, parameter, *debug );
  } else if ( !strncmp( action, "alignment_laser", strlen("alignment_laser") ) ) {
    status = Power_Relay ( Socket, 3, parameter, *debug );
  } else if ( !strncmp( action, "f15_laser", strlen("f15_laser") ) ) {
    status = Power_Relay ( Socket, 4, parameter, *debug );
  } else if ( !strncmp( action, "indigo", strlen("indigo") ) ) {
    status = Power_Relay ( Socket, 5, parameter, *debug );
  } else if ( !strncmp( action, "spare1", strlen("spare1") ) ) {
    status = Power_Relay ( Socket, 6, parameter, *debug );
  } else if ( !strncmp( action, "spare2", strlen("spare2") ) ) {
    status = Power_Relay ( Socket, 7, parameter, *debug );

  } else if ( !strncmp (action, "status", strlen("sta") ) ) {

    /*
      Request the current status of the given stage
    */
    status = Power_PrintDevice( Socket, Data);

  } else {

    printf("  Power_Process: Unknown action = %s\n", action);

  }

  return(0);
}

/* THE END */
