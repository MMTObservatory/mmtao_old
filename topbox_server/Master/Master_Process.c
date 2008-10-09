/*
  Master_Process.c

  This routine starts a data server for either WFSC or DM Data

  Written  13aug07  DLM  Started with PCR_Process.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Master.h"
#include "MVP.h"
#include "Smart.h"
#include "Stage.h"
#include "Fwheel.h"
#include "Power.h"

/*================================================================================*
 * Accepts requests to start a data server
 *================================================================================*/
void *Master_Process( void *Passed_Info)
{

  /*
    Local pointers to varible passed from the process which created this thread
   */
  Master_Thread_Info *Main_Info;
  Device_Info *FSM_Tip_Info;
  Device_Info *FSM_Tilt_Info;
  Device_Info *OAP_Tip_Info;
  Device_Info *OAP_Tilt_Info;
  Device_Info *Indigo_Info;
  Device_Info *F15_Info;
  Device_Info *Fwheel_Info;
  Device_Info *Power_Info;
  Device_Info *Smart_Info;
  Status_Struct *Status;
  Status_Entry *List;
  int list_length;

  int sockfd;
  int *Master_continueRunning;
  int *Master_debug;

  /*
    Local variables
  */
  int status;
  int continueRunning;
  char request[STRING_LENGTH];
  char Request[STRING_LENGTH];
  char *device, *parameter;
  char *save_ptr;

  /*
    Pointers to the needed Info structures
  */
  int FSM_Tip_Connected;

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (Master_Thread_Info *)Passed_Info;
  FSM_Tip_Info = Main_Info->fsm_tip;
  FSM_Tilt_Info = Main_Info->fsm_tilt;
  OAP_Tip_Info = Main_Info->oap_tip;
  OAP_Tilt_Info = Main_Info->oap_tilt;
  Indigo_Info = Main_Info->indigo;
  F15_Info = Main_Info->f15;
  Fwheel_Info = Main_Info->fwheel;
  Power_Info = Main_Info->power;
  Smart_Info = Main_Info->smart;
  Status = Main_Info->status;
  List = Status->list;
  list_length = Status->length;
  sockfd = Main_Info->sockfd;
  Master_continueRunning = Main_Info->continueRunning;
  Master_debug = Main_Info->debug;

  /*********************************************************************
    Determine the positions of the needed InfoList structures.
   *********************************************************************/

  /*
    Only need the postion of the first offset value, Static_Astig_45, because the other
      three offsets at the next three variables in InfoList.
  */
  status = Status_FindParameter( List, list_length, "FSM_Tip_Connected", &FSM_Tip_Connected, 1);
  if ( status ) {
    printf("  Master_Process: Error in Info_FindParamter\n");
    continueRunning = 0;
  }

  continueRunning = 1;

  while ( continueRunning ) {

    /*
      Read an instruction from the client
    */

    if ( Socket_StringRead( sockfd, request) ) {
      if ( *Master_debug ) printf("  Master_Process: Error reading request in Socket_StringRead\n");
      continueRunning = 0;
      continue;
    }

    /*
      Check that a request was sent
    */
    if ( strlen(request) <= 1 ) {
      printf("  Master_Process: Warning: request of zero length recieved\n");
      continue;
    }

    if ( *Master_debug ) {
      printf("  Master_Process: Request = %s\n", request);
    }

    /*
      Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
    */
    status = Socket_StringLowerCase(request);

    strcpy( Request, request);

    device = strtok_r( Request, " ", &save_ptr);

    if ( !strncmp ( device, "?", strlen("?") ) ||
	 !strncmp ( device, "help", strlen("help") ) ) {

      status = Master_SendHelp( sockfd);
      if ( status ) {
	printf("  Master_Process: Error returning help options\n");
	fflush(stdout);
	continue;
      }

    } else if ( !strncmp( device, "debug", strlen("debug") ) ) {

      /*
	Parse the debug value to set
      */
      parameter = strtok_r( NULL, " ", &save_ptr);
      if ( parameter == NULL ) {
	printf("  Master_Process: Debug parameter not included in request\n");
	printf("                  Request = %s\n", request);
	fflush(stdout);
	continue;
      }

      /*
	Set the appropriate debug parameter to the requested value
      */
      if ( !strncmp( parameter, "on", strlen("on")) ) {

	*Master_debug = 1;

      } else if ( !strncmp( parameter, "off", strlen("off")) ) {

	*Master_debug = 0;

      } else if ( !strncmp( parameter, "all", strlen("all")) ) {

	*Master_debug = 1;
	*(FSM_Tip_Info->cmd_debug) = 1;
	*(FSM_Tilt_Info->cmd_debug) = 1;
	*(OAP_Tip_Info->cmd_debug) = 1;
	*(OAP_Tilt_Info->cmd_debug) = 1;
	*(Indigo_Info->cmd_debug) = 1;
	*(F15_Info->cmd_debug) = 1;
	*(Fwheel_Info->cmd_debug) = 1;
	*(Power_Info->cmd_debug) = 1;
	*(Smart_Info->cmd_debug) = 1;

      } else if ( !strncmp( parameter, "none", strlen("none")) ) {

	*Master_debug = 0;
	*(FSM_Tip_Info->cmd_debug) = 0;
	*(FSM_Tilt_Info->cmd_debug) = 0;
	*(OAP_Tip_Info->cmd_debug) = 0;
	*(OAP_Tilt_Info->cmd_debug) = 0;
	*(Indigo_Info->cmd_debug) = 0;
	*(F15_Info->cmd_debug) = 0;
	*(Fwheel_Info->cmd_debug) = 0;
	*(Power_Info->cmd_debug) = 0;
	*(Smart_Info->cmd_debug) = 0;

      } else {

	printf("  Master_Process: Unknown debug paramter = %s\n", parameter);
	fflush(stdout);

      }

    } else if ( !strncmp( device, "fsm_tip", strlen("fsm_tip") ) ) {

      /*
	Call the MVP process routine and send the appropriate device strutures
      */
      status = MVP_Process( FSM_Tip_Info, request);

    } else if ( !strncmp( device, "fsm_tilt", strlen("fsm_tilt") ) ) {

      /*
	Call the MVP process routine and send the appropriate device strutures
      */
      status = MVP_Process( FSM_Tilt_Info, request);

    } else if ( !strncmp( device, "oap_tip", strlen("oap_tip") ) ) {

      /*
	Call the MVP process routine and send the appropriate device strutures
      */
      status = MVP_Process( OAP_Tip_Info, request);

    } else if ( !strncmp( device, "oap_tilt", strlen("oap_tilt") ) ) {

      /*
	Call the MVP process routine and send the appropriate device strutures
      */
      status = MVP_Process( OAP_Tilt_Info, request);

    } else if ( !strncmp( device, "indigo", strlen("indigo") ) ) {

      /*
	Call the Stage process routine and send the appropriate device strutures
      */
      status = Stage_Process( Indigo_Info, request);

    } else if ( !strncmp( device, "f15", strlen("f15") ) ) {

      /*
	Call the Stage process routine and send the appropriate device strutures
      */
      status = Stage_Process( F15_Info, request);

    } else if ( !strncmp( device, "fwheel", strlen("fwheel") ) ) {

      /*
	Call the Filter Wheel process routine and send the appropriate device strutures
      */
      status = Fwheel_Process( Fwheel_Info, request);

    } else if ( !strncmp( device, "power", strlen("power") ) ) {

      /*
	Call the Power process routine and send the appropriate device strutures
      */
      status = Power_Process( Power_Info, request);

    } else if ( !strncmp( device, "focus", strlen("focus") ) ) {

      /*
	Call the Focus process routine and send the appropriate device strutures
      */
      status = Smart_Process( Smart_Info, request);

    } else if ( !strncmp( device, "rotation", strlen("rotation") ) ) {

      /*
	Call the Focus process routine and send the appropriate device strutures
      */
      status = Smart_Process( Smart_Info, request);

    } else if ( !strncmp( device, "quit", strlen("quit") ) ) {

      /*
        Stop this server
      */
      continueRunning = 0;

    } else if ( !strncmp ( device, "exit", strlen("exit") ) ) {

      /*
        Stop this all threads
      */
      *Master_continueRunning = 0;

    } else {

      printf("  Master_Process: Unknown request = %s\n", request);

    }

    request[0] = 0;

  }

  if ( *Master_debug ) {
    printf("  Master_Process: Closing connection\n");
  }

  /*
    Set sockfd to -1 to indicate to mother process that this Thread_Info
      structure can be reused
  */
  close( Main_Info->sockfd);
  Main_Info->sockfd = -1;
  pthread_exit(NULL);

  return( (void *)0);
}

/* THE END */
