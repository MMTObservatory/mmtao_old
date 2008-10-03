/*
  Topbox_Server.c

  This routine starts a data server which will serve data from Topbox
    motors.

  Written  7aug07  DLM  Started with PCR_Server.c

*/

#include "Topbox.h"
#include "Device.h"
#include "MVP.h"
#include "Smart.h"
#include "Stage.h"
#include "Status.h"
#include "Master.h"
#include "Fwheel.h"
#include "Power.h"

/*
  Setup the Status array
*/
#include "StatusList.h"

/*================================================================================*
 * Start the Topbox Server
 *================================================================================*/
int main ( int argc, char **argv )
{

  /* Local variables */
  int debug = 0;
  int start_debug = 1;
  int status;
  int i;

  /*=========================================================
   *=========================================================
   *
   * FSM Tip declarations and flags
   *
   *=========================================================
   *=========================================================

    These structure are used by the status thread for this device
      and by the Master_Process (to send user requests to the device)
  */

  /*
    Socket Structures: contains information about the connection to 
      the specified device.
  */
  Socket_Info FSM_Tip_Socket;

  /*
    Status Structures: contains device data.
  */
  Device_Data FSM_Tip_Data;

  /*
    Fill in information for FSM Tip connection
  */
  status = Socket_Setup( (Socket_Info *)&FSM_Tip_Socket, "FSM_Tip", debug);
  if ( status ) {
    printf("  Topbox_Server: Error in Socket_Setup for FSM_Tip\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Initialize all flags in the Status Info and Data structures
  */
  Status_InitFlags( &FSM_Tip_Socket, &FSM_Tip_Data);

  /*
    Device Structures: contains all device information.
  */
  pthread_t fsm_tipThread;
  Device_Info FSM_Tip_Info;
  FSM_Tip_Info.socket = &FSM_Tip_Socket;
  FSM_Tip_Info.data = &FSM_Tip_Data;

  FSM_Tip_Info.continueRunning = (int *)malloc( sizeof(int));
  *(FSM_Tip_Info.continueRunning) = 1;

  FSM_Tip_Info.running = (int *)malloc( sizeof(int));
  *(FSM_Tip_Info.running) = 0;

  FSM_Tip_Info.status_debug = (int *)malloc( sizeof(int));
  *(FSM_Tip_Info.status_debug) = 0;

  FSM_Tip_Info.cmd_debug = (int *)malloc( sizeof(int));
  *(FSM_Tip_Info.cmd_debug) = 0;

  /*
    Set the Status List flag pointers to the addresses of the appropriate
      flags in each of the Device_Data structure.  If a device does not use
      a flag in the Device_Data structure then there is no need to see the
      value and the Status "All" request.  To ignore a flag, send NULL instead
      of a character string.
  */
  status = Status_SetFlagPointers( Status_List, Status_ListLength,
				   &FSM_Tip_Socket,
				   &FSM_Tip_Data,
				   "FSM_Tip_Connected",
				   "FSM_Tip_Connection_Error",
				   "FSM_Tip_Home",
				   "FSM_Tip_Position",
				   "FSM_Tip_Moving",
				   "FSM_Tip_In_Position",
				   "FSM_Tip_Error",
				   "FSM_Tip_Enabled",
				   "FSM_Tip_Negative",
				   "FSM_Tip_Positive",
				   NULL,
				   debug);

  /*=====================================================================
    Start looping thread to retrieve status data from the given device
    =======================================================================*/
  status = pthread_create( &fsm_tipThread, NULL, MVP_Looper, (void *)&FSM_Tip_Info);
  if ( start_debug ) {
    printf("  Topbox_Server: Starting FSM_Tip_Looper thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Topbox_Server: Error starting  FSM_Tip_Looper thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=========================================================
   *=========================================================
   *
   * FSM Tilt declarations and flags
   *
   *=========================================================
   *=========================================================

    These structure are used by the status thread for this device
      and by the Master_Process (to send user requests to the device)
  */

  /*
    Socket Structures: contains information about the connection to 
      the specified device.
  */
  Socket_Info FSM_Tilt_Socket;

  /*
    Status Structures: contains device data.
  */
  Device_Data FSM_Tilt_Data;

  /*
    Fill in information for FSM Tilt connection
  */
  status = Socket_Setup( (Socket_Info *)&FSM_Tilt_Socket, "FSM_Tilt", debug);
  if ( status ) {
    printf("  Topbox_Server: Error in Socket_Setup for FSM_Tilt\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Initialize all flags in the Status Info and Data structures
  */
  Status_InitFlags( &FSM_Tilt_Socket, &FSM_Tilt_Data);

  /*
    Device Structures: contains all device information.
  */
  pthread_t fsm_tiltThread;
  Device_Info FSM_Tilt_Info;
  FSM_Tilt_Info.socket = &FSM_Tilt_Socket;
  FSM_Tilt_Info.data = &FSM_Tilt_Data;

  FSM_Tilt_Info.continueRunning = (int *)malloc( sizeof(int));
  *(FSM_Tilt_Info.continueRunning) = 1;

  FSM_Tilt_Info.running = (int *)malloc( sizeof(int));
  *(FSM_Tilt_Info.running) = 0;

  FSM_Tilt_Info.status_debug = (int *)malloc( sizeof(int));
  *(FSM_Tilt_Info.status_debug) = 0;

  FSM_Tilt_Info.cmd_debug = (int *)malloc( sizeof(int));
  *(FSM_Tilt_Info.cmd_debug) = 0;

  /*
    Set the Status List flag pointers to the addresses of the appropriate
      flags in each of the Device_Data structure.  If a device does not use
      a flag in the Device_Data structure then there is no need to see the
      value and the Status "All" request.  To ignore a flag, send NULL instead
      of a character string.
  */
  status = Status_SetFlagPointers( Status_List, Status_ListLength,
				   &FSM_Tilt_Socket,
				   &FSM_Tilt_Data,
				   "FSM_Tilt_Connected",
				   "FSM_Tilt_Connection_Error",
				   "FSM_Tilt_Home",
				   "FSM_Tilt_Position",
				   "FSM_Tilt_Moving",
				   "FSM_Tilt_In_Position",
				   "FSM_Tilt_Error",
				   "FSM_Tilt_Enabled",
				   "FSM_Tilt_Negative",
				   "FSM_Tilt_Positive",
				   NULL,
				   debug);

  /*=====================================================================
    Start looping thread to retrieve status data from the given device
    =======================================================================*/
  status = pthread_create( &fsm_tiltThread, NULL, MVP_Looper, (void *)&FSM_Tilt_Info);
  if ( start_debug ) {
    printf("  Topbox_Server: Starting FSM_Tilt_Looper thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Topbox_Server: Error starting  FSM_Tilt_Looper thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=========================================================
   *=========================================================
   *
   * OAP Tip declarations and flags
   *
   *=========================================================
   *=========================================================

    These structure are used by the status thread for this device
      and by the Master_Process (to send user requests to the device)
  */

  /*
    Socket Structures: contains information about the connection to 
      the specified device.
  */
  Socket_Info OAP_Tip_Socket;

  /*
    Status Structures: contains device data.
  */
  Device_Data OAP_Tip_Data;

  /*
    Fill in information for OAP Tip connection
  */
  status = Socket_Setup( (Socket_Info *)&OAP_Tip_Socket, "OAP_Tip", debug);
  if ( status ) {
    printf("  Topbox_Server: Error in Socket_Setup for OAP_Tip\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Initialize all flags in the Status Info and Data structures
  */
  Status_InitFlags( &OAP_Tip_Socket, &OAP_Tip_Data);

  /*
    Device Structures: contains all device information.
  */
  pthread_t oap_tipThread;
  Device_Info OAP_Tip_Info;
  OAP_Tip_Info.socket = &OAP_Tip_Socket;
  OAP_Tip_Info.data = &OAP_Tip_Data;

  OAP_Tip_Info.continueRunning = (int *)malloc( sizeof(int));
  *(OAP_Tip_Info.continueRunning) = 1;

  OAP_Tip_Info.running = (int *)malloc( sizeof(int));
  *(OAP_Tip_Info.running) = 0;

  OAP_Tip_Info.status_debug = (int *)malloc( sizeof(int));
  *(OAP_Tip_Info.status_debug) = 0;

  OAP_Tip_Info.cmd_debug = (int *)malloc( sizeof(int));
  *(OAP_Tip_Info.cmd_debug) = 0;

  /*
    Set the Status List flag pointers to the addresses of the appropriate
      flags in each of the Device_Data structure.  If a device does not use
      a flag in the Device_Data structure then there is no need to see the
      value and the Status "All" request.  To ignore a flag, send NULL instead
      of a character string.
  */
  status = Status_SetFlagPointers( Status_List, Status_ListLength,
				   &OAP_Tip_Socket,
				   &OAP_Tip_Data,
				   "OAP_Tip_Connected",
				   "OAP_Tip_Connection_Error",
				   "OAP_Tip_Home",
				   "OAP_Tip_Position",
				   "OAP_Tip_Moving",
				   "OAP_Tip_In_Position",
				   "OAP_Tip_Error",
				   "OAP_Tip_Enabled",
				   "OAP_Tip_Negative",
				   "OAP_Tip_Positive",
				   "OAP_Tip_Event_2",
				   debug);

  /*=====================================================================
    Start looping thread to retrieve status data from the given device
    =======================================================================*/
  status = pthread_create( &oap_tipThread, NULL, MVP_Looper, (void *)&OAP_Tip_Info);
  if ( start_debug ) {
    printf("  Topbox_Server: Starting OAP_Tip_Looper thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Topbox_Server: Error starting  OAP_Tip_Looper thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=========================================================
   *=========================================================
   *
   * OAP Tilt declarations and flags
   *
   *=========================================================
   *=========================================================

    These structure are used by the status thread for this device
      and by the Master_Process (to send user requests to the device)
  */

  /*
    Socket Structures: contains information about the connection to 
      the specified device.
  */
  Socket_Info OAP_Tilt_Socket;

  /*
    Status Structures: contains device data.
  */
  Device_Data OAP_Tilt_Data;

  /*
    Fill in information for OAP Tilt connection
  */
  status = Socket_Setup( (Socket_Info *)&OAP_Tilt_Socket, "OAP_Tilt", debug);
  if ( status ) {
    printf("  Topbox_Server: Error in Socket_Setup for OAP_Tilt\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Initialize all flags in the Status Info and Data structures
  */
  Status_InitFlags( &OAP_Tilt_Socket, &OAP_Tilt_Data);

  /*
    Device Structures: contains all device information.
  */
  pthread_t oap_tiltThread;
  Device_Info OAP_Tilt_Info;
  OAP_Tilt_Info.socket = &OAP_Tilt_Socket;
  OAP_Tilt_Info.data = &OAP_Tilt_Data;

  OAP_Tilt_Info.continueRunning = (int *)malloc( sizeof(int));
  *(OAP_Tilt_Info.continueRunning) = 1;

  OAP_Tilt_Info.running = (int *)malloc( sizeof(int));
  *(OAP_Tilt_Info.running) = 0;

  OAP_Tilt_Info.status_debug = (int *)malloc( sizeof(int));
  *(OAP_Tilt_Info.status_debug) = 0;

  OAP_Tilt_Info.cmd_debug = (int *)malloc( sizeof(int));
  *(OAP_Tilt_Info.cmd_debug) = 0;

  /*
    Set the Status List flag pointers to the addresses of the appropriate
      flags in each of the Device_Data structure.  If a device does not use
      a flag in the Device_Data structure then there is no need to see the
      value and the Status "All" request.  To ignore a flag, send NULL instead
      of a character string.
  */
  status = Status_SetFlagPointers( Status_List, Status_ListLength,
				   &OAP_Tilt_Socket,
				   &OAP_Tilt_Data,
				   "OAP_Tilt_Connected",
				   "OAP_Tilt_Connection_Error",
				   "OAP_Tilt_Home",
				   "OAP_Tilt_Position",
				   "OAP_Tilt_Moving",
				   "OAP_Tilt_In_Position",
				   "OAP_Tilt_Error",
				   "OAP_Tilt_Enabled",
				   "OAP_Tilt_Negative",
				   "OAP_Tilt_Positive",
				   "OAP_Tilt_Event_2",
				   debug);

  /*=====================================================================
    Start looping thread to retrieve status data from the given device
    =======================================================================*/
  status = pthread_create( &oap_tiltThread, NULL, MVP_Looper, (void *)&OAP_Tilt_Info);
  if ( start_debug ) {
    printf("  Topbox_Server: Starting OAP_Tilt_Looper thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Topbox_Server: Error starting  OAP_Tilt_Looper thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=========================================================
   *=========================================================
   *
   * Indigo Stage declarations and flags
   *
   *=========================================================
   *=========================================================

    These structure are used by the status thread for this device
      and by the Master_Process (to send user requests to the device)
  */

  /*
    Socket Structures: contains information about the connection to 
      the specified device.
  */
  Socket_Info Indigo_Socket;

  /*
    Status Structures: contains device data.
  */
  Device_Data Indigo_Data;

  /*
    Fill in information for Indigo connection
  */
  status = Socket_Setup( (Socket_Info *)&Indigo_Socket, "Indigo", debug);
  if ( status ) {
    printf("  Topbox_Server: Error in Socket_Setup for Indigo\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Initialize all flags in the Status Info and Data structures
  */
  Status_InitFlags( &Indigo_Socket, &Indigo_Data);

  /*
    Device Structures: contains all device information.
  */
  pthread_t indigoThread;
  Device_Info Indigo_Info;
  Indigo_Info.socket = &Indigo_Socket;
  Indigo_Info.data = &Indigo_Data;

  Indigo_Info.continueRunning = (int *)malloc( sizeof(int));
  *(Indigo_Info.continueRunning) = 1;

  Indigo_Info.running = (int *)malloc( sizeof(int));
  *(Indigo_Info.running) = 0;

  Indigo_Info.status_debug = (int *)malloc( sizeof(int));
  *(Indigo_Info.status_debug) = 0;

  Indigo_Info.cmd_debug = (int *)malloc( sizeof(int));
  *(Indigo_Info.cmd_debug) = 0;

  /*
    Set the Status List flag pointers to the addresses of the appropriate
      flags in each of the Device_Data structure.  If a device does not use
      a flag in the Device_Data structure then there is no need to see the
      value and the Status "All" request.  To ignore a flag, send NULL instead
      of a character string.
  */
  status = Status_SetFlagPointers( Status_List, Status_ListLength,
				   &Indigo_Socket,
				   &Indigo_Data,
				   "Indigo_Connected",
				   "Indigo_Connection_Error",
				   "Indigo_Home",
				   "Indigo_Position",
				   "Indigo_Moving",
				   NULL,
				   NULL,
				   "Indigo_Enabled",
				   "Indigo_Negative",
				   "Indigo_Positive",
				   NULL,
				   debug);

  /*=====================================================================
    Start looping thread to retrieve status data from the given device
    =======================================================================*/
  status = pthread_create( &indigoThread, NULL, Stage_Looper, (void *)&Indigo_Info);
  if ( start_debug ) {
    printf("  Topbox_Server: Starting Indigo_Looper thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Topbox_Server: Error starting  Indigo_Looper thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=========================================================
   *=========================================================
   *
   * F15 Stage declarations and flags
   *
   *=========================================================
   *=========================================================

    These structure are used by the status thread for this device
      and by the Master_Process (to send user requests to the device)
  */

  /*
    Socket Structures: contains information about the connection to 
      the specified device.
  */
  Socket_Info F15_Socket;

  /*
    Status Structures: contains device data.
  */
  Device_Data F15_Data;

  /*
    Fill in information for F15 connection
  */
  status = Socket_Setup( (Socket_Info *)&F15_Socket, "F15", debug);
  if ( status ) {
    printf("  Topbox_Server: Error in Socket_Setup for F15\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Initialize all flags in the Status Info and Data structures
  */
  Status_InitFlags( &F15_Socket, &F15_Data);

  /*
    Device Structures: contains all device information.
  */
  pthread_t f15Thread;
  Device_Info F15_Info;
  F15_Info.socket = &F15_Socket;
  F15_Info.data = &F15_Data;

  F15_Info.continueRunning = (int *)malloc( sizeof(int));
  *(F15_Info.continueRunning) = 1;

  F15_Info.running = (int *)malloc( sizeof(int));
  *(F15_Info.running) = 0;

  F15_Info.status_debug = (int *)malloc( sizeof(int));
  *(F15_Info.status_debug) = 0;

  F15_Info.cmd_debug = (int *)malloc( sizeof(int));
  *(F15_Info.cmd_debug) = 0;

  /*
    Set the Status List flag pointers to the addresses of the appropriate
      flags in each of the Device_Data structure.  If a device does not use
      a flag in the Device_Data structure then there is no need to see the
      value and the Status "All" request.  To ignore a flag, send NULL instead
      of a character string.
  */
  status = Status_SetFlagPointers( Status_List, Status_ListLength,
				   &F15_Socket,
				   &F15_Data,
				   "F15_Connected",
				   "F15_Connection_Error",
				   "F15_Home",
				   "F15_Position",
				   "F15_Moving",
				   NULL,
				   NULL,
				   "F15_Enabled",
				   "F15_Negative",
				   "F15_Positive",
				   NULL,
				   debug);

  /*=====================================================================
    Start looping thread to retrieve status data from the given device
    =======================================================================*/
  status = pthread_create( &f15Thread, NULL, Stage_Looper, (void *)&F15_Info);
  if ( start_debug ) {
    printf("  Topbox_Server: Starting F15_Looper thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Topbox_Server: Error starting  F15_Looper thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=========================================================
   *=========================================================
   *
   * Filter Wheel declarations and flags
   *
   *=========================================================
   *=========================================================

    These structure are used by the status thread for this device
      and by the Master_Process (to send user requests to the device)
  */

  /*
    Socket Structures: contains information about the connection to 
      the specified device.
  */
  Socket_Info Fwheel_Socket;

  /*
    Status Structures: contains device data.
  */
  pthread_t fwheelThread;
  Device_Data Fwheel_Data;

  /*
    Fill in information for Fwheel connection
  */
  status = Socket_Setup( (Socket_Info *)&Fwheel_Socket, "Fwheel", debug);
  if ( status ) {
    printf("  Topbox_Server: Error in Socket_Setup for Fwheel\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Initialize all flags in the Status Info and Data structures
  */
  Status_InitFlags( &Fwheel_Socket, &Fwheel_Data);

  /*
    Device Structures: contains all device information.
  */
  Device_Info Fwheel_Info;
  Fwheel_Info.socket = &Fwheel_Socket;
  Fwheel_Info.data = &Fwheel_Data;

  Fwheel_Info.continueRunning = (int *)malloc( sizeof(int));
  *(Fwheel_Info.continueRunning) = 1;

  Fwheel_Info.running = (int *)malloc( sizeof(int));
  *(Fwheel_Info.running) = 1;

  Fwheel_Info.status_debug = (int *)malloc( sizeof(int));
  *(Fwheel_Info.status_debug) = 0;

  Fwheel_Info.cmd_debug = (int *)malloc( sizeof(int));
  *(Fwheel_Info.cmd_debug) = 0;

  /*
    Set the Status List flag pointers to the addresses of the appropriate
      flags in each of the Device_Data structure.  If a device does not use
      a flag in the Device_Data structure then there is no need to see the
      value and the Status "All" request.  To ignore a flag, send NULL instead
      of a character string.
  */
  status = Status_SetFwheelPointers( Status_List, Status_ListLength,
				     &Fwheel_Socket,
				     &Fwheel_Data,
				     "Fwheel_Connected",
				     "Fwheel_Connection_Error",
				     "Fwheel_Home",
				     "Fwheel_Position",
				     "Fwheel_Name",
				     "Fwheel_Moving",
				     debug);

  /*=====================================================================
    Start looping thread to retrieve status data from the given device
    =======================================================================*/
  status = pthread_create( &fwheelThread, NULL, Fwheel_Looper, (void *)&Fwheel_Info);
  if ( start_debug ) {
    printf("  Topbox_Server: Starting Fwheel looper thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Topbox_Server: Error starting  Fwheel looper thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=========================================================
   *=========================================================
   *
   * Power Controller declarations and flags
   *
   *=========================================================
   *=========================================================

    These structure are used by the status thread for this device
      and by the Master_Process (to send user requests to the device)
  */

  /*
    Socket Structures: contains information about the connection to 
      the specified device.
  */
  Socket_Info Power_Socket;

  /*
    Status Structures: contains device data.
  */
  pthread_t PowerThread;
  Device_Data Power_Data;

  /*
    Fill in information for Power connection
  */
  status = Socket_Setup( (Socket_Info *)&Power_Socket, "Power", debug);
  if ( status ) {
    printf("  Topbox_Server: Error in Socket_Setup for Power\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Initialize all flags in the Status Info and Data structures
  */
  Status_InitFlags( &Power_Socket, &Power_Data);

  /*
    Device Structures: contains all device information.
  */
  Device_Info Power_Info;
  Power_Info.socket = &Power_Socket;
  Power_Info.data = &Power_Data;

  Power_Info.continueRunning = (int *)malloc( sizeof(int));
  *(Power_Info.continueRunning) = 1;

  Power_Info.running = (int *)malloc( sizeof(int));
  *(Power_Info.running) = 0;

  Power_Info.status_debug = (int *)malloc( sizeof(int));
  *(Power_Info.status_debug) = 0;

  Power_Info.cmd_debug = (int *)malloc( sizeof(int));
  *(Power_Info.cmd_debug) = 0;

  /*
    Set Status pointer so appropriate addresses
  */
  status = Status_SetPowerPointers( Status_List, Status_ListLength,
				    &Power_Socket,
				    &Power_Data,
				    "Power_Connected",
				    "Power_Connection_Error",
				    "Power_WFSC",
				    "Power_Peltier",
				    "Power_Stella",
				    "Power_Alignment_Laser",
				    "Power_F15_Laser",
				    "Power_Indigo",
				    "Power_Spare1",
				    "Power_Spare2",
				    debug);

  /*=====================================================================
    Start looping thread to retrieve status data from the given device
    =======================================================================*/
  status = pthread_create( &PowerThread, NULL, Power_Looper, (void *)&Power_Info);
  if ( start_debug ) {
    printf("  Topbox_Server: Starting Power looper thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Topbox_Server: Error starting  Power looper thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=========================================================
   *=========================================================
   *
   * Smart Controller declarations and flags
   *
   * Focus and Rotation - one socket, two devices
   *  -- also a second socket for the encoder.
   *
   *=========================================================
   *========================================================= */

  Socket_Info Smart_Socket;

  status = Socket_Setup( (Socket_Info *)&Smart_Socket, "Smart", debug);
  if ( status ) {
    printf("  Topbox_Server: Error in Socket_Setup for Smart controller\n");
    fflush(stdout);
    return(-1);
  }

  Device_Data Focus_Data;
  Device_Data Rotation_Data;

  /* Initialize all flags in the Status Info and Data structures
   */
  strcpy( Smart_Socket.name, "Focus");
  Status_InitFlags( &Smart_Socket, &Focus_Data);
  strcpy( Focus_Data.name, "Focus");

  strcpy( Smart_Socket.name, "Rotation");
  Status_InitFlags( &Smart_Socket, &Rotation_Data);
  strcpy( Rotation_Data.name, "Rotation");

  strcpy( Smart_Socket.name, "Smart");

  /* Device Structures: contains all device information.
   */
  Device_Info Smart_Info;
  Smart_Info.socket = &Smart_Socket;
  Smart_Info.data = &Focus_Data;
  Smart_Info.data2 = &Rotation_Data;

  Smart_Info.continueRunning = (int *)malloc( sizeof(int));
  *(Smart_Info.continueRunning) = 1;

  Smart_Info.running = (int *)malloc( sizeof(int));
  *(Smart_Info.running) = 0;

  Smart_Info.status_debug = (int *)malloc( sizeof(int));
  *(Smart_Info.status_debug) = 0;

  Smart_Info.cmd_debug = (int *)malloc( sizeof(int));
  *(Smart_Info.cmd_debug) = 0;

  /*
    Set Status pointer so appropriate addresses
  */
  status = Status_SetFlagPointers( Status_List, Status_ListLength,
				    &Smart_Socket,
				    &Focus_Data,
				    "Focus_Connected",
				    "Focus_Connection_Error",
				    "Focus_Home",
				    "Focus_Position",
				    "Focus_Moving",
				    NULL,
				    NULL,
				    "Focus_Enabled",
				    "Focus_Negative",
				    "Focus_Positive",
				   NULL, debug);

  status = Status_SetFlagPointers( Status_List, Status_ListLength,
				    &Smart_Socket,
				    &Rotation_Data,
				    "Rotation_Connected",
				    "Rotation_Connection_Error",
				    "Rotation_Home",
				    "Rotation_Position",
				    "Rotation_Moving",
				    NULL,
				    NULL,
				    "Rotation_Enabled",
				    "Rotation_Negative",
				    "Rotation_Positive",
				    NULL, debug );

  /*=====================================================================
    Start looping thread to retrieve status data from the given device
    =======================================================================*/

  pthread_t SmartThread;

  status = pthread_create( &SmartThread, NULL, Smart_Looper, (void *)&Smart_Info);
  if ( start_debug ) {
    printf("  Topbox_Server: Starting Focus and Rotation looper thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Topbox_Server: Error starting  Focus and Rotation looper thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=========================================================
   *=========================================================
   *
   * Read default Status values
   *
   *=========================================================
   *=========================================================*/

  /*
    Status Socket Structures (listen for clients requests)
  */
  Socket_Info Status_Socket;

  /*
    Full Info information structures.
  */
  Status_Struct Status_Info;

  /*
    Put Info Socket and List structures together
  */
  Status_Info.socket = &Status_Socket;
  Status_Info.list = Status_List;
  Status_Info.length = Status_ListLength;

  /*
    Structure to pass variable information to threads
  */
  pthread_t statusThread;
  Status_Thread_Info Status_Thread;

  /*
    Put Status socket information into structure to pass to Status
      thread
  */
  Status_Thread.status = &Status_Info;

  /*
    Global flag for the Status_Master thread
  */
  Status_Thread.continueRunning = (int *)malloc( sizeof(int));
  *(Status_Thread.continueRunning) = 1;

  Status_Thread.debug = (int *)malloc( sizeof(int));
  *(Status_Thread.debug) = debug;

  /*
    Fill in information for Status server
  */
  status = Socket_Setup( (Socket_Info *)&Status_Socket, "Status", debug);
  if ( status ) {
    printf("  Topbox_Server: Error in Socket_Setup for Status\n");
    fflush(stdout);
    return(-1);
  }

  /*=====================================================================
    Start thread to serve Status data
    =======================================================================*/
  status = pthread_create( &statusThread, NULL, Status_Master, (void *)&Status_Thread);
  if ( start_debug ) {
    printf("  Topbox_Server: Starting Status thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Topbox_Server: Error starting Status thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=========================================================
   *=========================================================
   *
   * Master_Process declarations and flags
   *
   *=========================================================
   *=========================================================*/

  /*
    Client connection information
  */
  struct sockaddr_in cli_addr;
  socklen_t clilen;
  int newsockfd, saveflags;

  /*
    Master Socket Structures (listen for clients requests)
  */
  Socket_Info Master_Socket;

  /*
    Structure to pass variable information to Master threads
  */
  pthread_t masterThread[NUM_CONNECTIONS];
  Master_Thread_Info Master_Info[NUM_CONNECTIONS];

  /*
    Global flag for the Master_Process threads
  */
  int *Master_continueRunning;
  Master_continueRunning = (int *)malloc( sizeof(int));
  *Master_continueRunning = 1;

  int *Master_debug;
  Master_debug = (int *)malloc( sizeof(int));
  *Master_debug = debug;

  /*
    Fill in information for Master server
  */
  status = Socket_Setup( (Socket_Info *)&Master_Socket, "Master", debug);
  if ( status ) {
    printf("  Topbox_Server: Error in Socket_Setup for Master\n");
    fflush(stdout);
    return(-1);
  }

  /*=====================================================================
    Open a non-blocking Socket to receive requests
    =======================================================================*/

  /*
    Open a TCP socket (an Internet stream socket) and bind
      the local address so a client can connect and send request
  */
  if ( Socket_OpenServer( (Socket_Info *)&Master_Socket, debug) ) {
    printf("  Topbox_Server: Error opening Master socket\n");
    printf("    Exiting...\n");
    fflush(stdout);
    return(-1);
  }

  if ( start_debug ) {
    printf("  Topbox_Server: Starting Topbox_Master thread\n");
    fflush(stdout);
  }

  /*
    Set sockfd as non-blocking.  In this way we can see if a request to
      stop the Topbox_Server has been sent.

    First, save the current flags for sockfd
  */
  saveflags=fcntl(Master_Socket.sockfd,F_GETFL,0);
  if(saveflags<0) {
    printf("  Topbox_Server: Error saving original configuration of sockfd\n");
    printf("    Exiting...\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Now set this socket as non-blocking
  */
  if(fcntl(Master_Socket.sockfd,F_SETFL,saveflags|O_NONBLOCK)<0) {
    printf("  Topbox_Server: Error setting sockfd to non-blocking\n");
    printf("    Exiting...\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Set all Master_Info[*].sockfd to -1 to indicate they are not in use
  */
  for ( i=0; i<NUM_CONNECTIONS; i++) {
    Master_Info[i].sockfd = -1;
  }

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

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
	if (      Master_Info[i].sockfd < 0 ) break;
      }

      if ( i<NUM_CONNECTIONS ) {

	/*
	  Restore new socket to blocking connection
	*/
	if(fcntl(newsockfd,F_SETFL,saveflags)<0) {
	  printf("  Topbox_Server: Error restoring newsockfd to blocking connection\n");
	  fflush(stdout);
	  *Master_continueRunning = 0;
	  continue;
	}

	/*
	  Fill in Thread_Info structure with PCR information and this new connection
	*/
	Master_Info[i].fsm_tip = &FSM_Tip_Info;
	Master_Info[i].fsm_tilt = &FSM_Tilt_Info;
	Master_Info[i].oap_tip = &OAP_Tip_Info;
	Master_Info[i].oap_tilt = &OAP_Tilt_Info;
	Master_Info[i].indigo = &Indigo_Info;
	Master_Info[i].f15 = &F15_Info;
	Master_Info[i].fwheel = &Fwheel_Info;
	Master_Info[i].power = &Power_Info;
	Master_Info[i].status = &Status_Info;
	Master_Info[i].smart = &Smart_Info;
	Master_Info[i].sockfd = newsockfd;

	Master_Info[i].continueRunning = Master_continueRunning;
	Master_Info[i].debug = Master_debug;

      } else {

	printf("  Topbox_Server: Too many connections\n");
	fflush(stdout);
	continue;

      }

      /*
	Start Thread
      */
      status = pthread_create( &masterThread[i], &attr, Master_Process, (void *)&Master_Info[i]);
      if ( start_debug ) {
	printf("  Topbox_Server: Starting CMD thread\n");
	fflush(stdout);
      }
      if ( status ) {
	printf("  Topbox_Server: Error starting CMD thread\n");
	fflush(stdout);
	*Master_continueRunning = 0;
	continue;
      }

    }

  } while ( *Master_continueRunning );

  if ( debug ) {
    printf("  Topbox_Server: Server Shutdown\n");
    fflush(stdout);
  }

  /*
    Send an exit signal to any running Master_Process threads
  */
  for ( i=0; i<NUM_CONNECTIONS; i++) {
    if ( Master_Info[i].sockfd >= 0 ) {
      if ( debug ) {
	printf("  Topbox_Server: Canceling thread %d\n", i);
	fflush(stdout);
      }
      pthread_cancel( masterThread[i]);
    }
  }
  Socket_Close( (Socket_Info *)&Master_Socket, debug);

  /*
    Wait for a bit until the other master threads have time
     to shutdown
  */
  sleep(1);

  return 0;

} /* end of main() */

/* THE END */
