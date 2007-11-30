/*
  Master.h

  Definitions needed to pass information from the starting process to
    the Master_Process (theads that interact with connections from other
    processes)

  Written  13aug07  DLM  Started with DServ.h from pcr_server

*/

/*
  Number of connections possible
*/
#define NUM_CONNECTIONS 10

/*
  Structure to send all information to each client connection
*/

typedef struct _Master_Thread_Info {
  Device_Info *fsm_tip;               // FSM Tip device information
  Device_Info *fsm_tilt;              // FSM Tilt device information
  Device_Info *oap_tip;               // OAP Tip device information
  Device_Info *oap_tilt;              // OAP Tilt device information
  Device_Info *indigo;                // Indigo Stage information
  Device_Info *f15;                   // F15 Stage information
  Device_Info *fwheel;                // Filter Wheel information
  Device_Info *power;                 // Power information
  Device_Info *smart;                 // Focus and Rotation information
  Status_Struct *status;              // Array containing all the Status information
  int sockfd;                         // Socket of client
  int *continueRunning;               // Flag to tell all Master client connections are running
  int *debug;                         // Flag used for debugging all client connections
} Master_Thread_Info;

/* Prototypes */
void *Master_Process( void *Passed_Info);
int Master_SendHelp( int sockfd);

/* THE END */
