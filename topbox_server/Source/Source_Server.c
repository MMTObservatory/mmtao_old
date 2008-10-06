/*
  Source_Server.c

  This routine creates several threads to imulate the devices in
    the topbox:

    - FSM_Tip

  Written  15aug07  DLM  Started with Topbox_Server.c

*/

#include "Topbox.h"
#include "Device.h"
#include "MVP.h"
#include "Status.h"
#include "Source.h"

/*================================================================================*
 * Start main process that creates the thread for topbox devices
 *================================================================================*/
int main( )
{

  /* Local variables */
  int debug = 1;
  int start_debug = 1;
  int status;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

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
  *(FSM_Tip_Info.cmd_debug) = 1;

  /*=====================================================================
    Start FSM Tip motor server
    =======================================================================*/
  status = pthread_create( &fsm_tipThread, &attr, Source_MVP, (void *)&FSM_Tip_Info);
  if ( start_debug ) {
    printf("  Source_Server: Starting FSM_Tip_Source thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Source_Server: Error starting FSM_Tip_Source thread\n");
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
  *(FSM_Tilt_Info.cmd_debug) = 1;

  /*=====================================================================
    Start FSM Tilt motor server
    =======================================================================*/
  status = pthread_create( &fsm_tiltThread, &attr, Source_MVP, (void *)&FSM_Tilt_Info);
  if ( start_debug ) {
    printf("  Source_Server: Starting FSM_Tilt_Source thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Source_Server: Error starting FSM_Tilt_Source thread\n");
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
  *(OAP_Tip_Info.cmd_debug) = 1;

  /*=====================================================================
    Start OAP Tip motor server
    =======================================================================*/
  status = pthread_create( &oap_tipThread, &attr, Source_MVP, (void *)&OAP_Tip_Info);
  if ( start_debug ) {
    printf("  Source_Server: Starting OAP_Tip_Source thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Source_Server: Error starting OAP_Tip_Source thread\n");
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
  *(OAP_Tilt_Info.cmd_debug) = 1;

  /*=====================================================================
    Start OAP Tilt motor server
    =======================================================================*/
  status = pthread_create( &oap_tiltThread, &attr, Source_MVP, (void *)&OAP_Tilt_Info);
  if ( start_debug ) {
    printf("  Source_Server: Starting OAP_Tilt_Source thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Source_Server: Error starting OAP_Tilt_Source thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=========================================================
   *=========================================================
   *
   * Indigo declarations and flags
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
  *(Indigo_Info.cmd_debug) = 1;

  /*=====================================================================
    Start Indigo Stage server
    =======================================================================*/
  status = pthread_create( &indigoThread, &attr, Source_Stage, (void *)&Indigo_Info);
  if ( start_debug ) {
    printf("  Source_Server: Starting Indigo_Source thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Source_Server: Error starting Indigo_Source thread\n");
    fflush(stdout);
    exit(-1);
  }

  /*=========================================================
   *=========================================================
   *
   * F15 declarations and flags
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
  *(F15_Info.cmd_debug) = 1;

  /*=====================================================================
    Start F15 Stage server
    =======================================================================*/
  status = pthread_create( &f15Thread, &attr, Source_Stage, (void *)&F15_Info);
  if ( start_debug ) {
    printf("  Source_Server: Starting F15_Source thread\n");
    fflush(stdout);
  }
  if ( status ) {
    printf("  Source_Server: Error starting F15_Source thread\n");
    fflush(stdout);
    exit(-1);
  }

  do {

    sleep(1);

  } while ( 1 );

  return(0);
}
