/*
  Definitions needed by DServ

  The WFSC, Spots, Slopes and DM servers will include this file as
    well as one specifically for each server.

  Written  19nov05  DLM

  Altered   2may06  DLM  Added frameSize and frameBytes to the DServ_Data structure

*/

#include "Socket.h"
#include "Info.h"

/*
  Number of connections possible
*/
#define NUM_CONNECTIONS 10

/*
  NUM_WFSC_FRAMES and NUM_DM_FRAMES are the number of frames in the storage
    buffers
*/
#define NUM_WFSC_FRAMES      1000
#define NUM_DM_FRAMES        1000
#define NUM_SLOPES_FRAMES    1000

/*
  This is size of the header, which includes "SOF" and the frame number (long)
*/
#define HEADER_BYTES      (3 * sizeof(char) + sizeof(long))

/*
  This is the maximum possible size for a WFSC frame
*/
#define TOTAL_WFSC_VALUES (72*72)
#define TOTAL_WFSC_BYTES  (HEADER_BYTES + TOTAL_WFSC_VALUES * sizeof(short))

/*
  Definitions of sizes and positions of DM data.  Order is important
*/
#define WFSC_OFFSET       HEADER_BYTES
#define WFSC_VALUES       (24*24)
#define WFSC_BYTES        (WFSC_VALUES * sizeof(short))

#define SLOPE_OFFSET      (WFSC_OFFSET + WFSC_BYTES)
#define SLOPE_VALUES      (2*12*12)
#define SLOPE_BYTES       (SLOPE_VALUES * sizeof(short))

#define RTR_OFFSET        (SLOPE_OFFSET + SLOPE_BYTES)
#define RTR_VALUES        336
#define RTR_BYTES         (RTR_VALUES * sizeof(short))

#define CMD_OFFSET        (RTR_OFFSET + RTR_BYTES)
#define CMD_VALUES        336
#define CMD_BYTES         (CMD_VALUES * sizeof(short))

#define CUR_OFFSET        (CMD_OFFSET + CMD_BYTES)
#define CUR_VALUES        336
#define CUR_BYTES         (CUR_VALUES * sizeof(short))

#define POS_OFFSET        (CUR_OFFSET + CUR_BYTES)
#define POS_VALUES        336
#define POS_BYTES         (POS_VALUES * sizeof(short))

#define ALL_OFFSET        WFSC_OFFSET
#define ALL_VALUES        (WFSC_VALUES + SLOPE_VALUES + RTR_VALUES + CMD_VALUES + CUR_VALUES + POS_VALUES)
#define ALL_BYTES         (WFSC_BYTES + SLOPE_BYTES + RTR_BYTES + CMD_BYTES + CUR_BYTES + POS_BYTES)

#define TOTAL_DM_VALUES   (WFSC_VALUES + SLOPE_VALUES + RTR_VALUES + CMD_VALUES + CUR_VALUES + POS_VALUES)
#define TOTAL_DM_BYTES    (POS_OFFSET + POS_BYTES)

typedef struct _DServ_Data {
  int num_frames;     // number of frames in buffer
  int header_bytes;   // number of bytes in the header (SOF and Frame number)
  int wfsc_offset;    // number of bytes from the beginning of the buffer where WFSC data starts
  int wfsc_bytes;     // number of bytes in the WFSC frame
  int slope_offset;   // number of bytes from the beginning of the buffer where Slope data starts
  int slope_bytes;    // number of bytes in the Slope frame
  int rtr_offset;     // number of bytes from the beginning of the buffer where RTR Delta data starts
  int rtr_bytes;      // number of bytes in the RTR Delta frame
  int cmd_offset;     // number of bytes from the beginning of the buffer where Command data starts
  int cmd_bytes;      // number of bytes in the Command frame
  int cur_offset;     // number of bytes from the beginning of the buffer where Current data starts
  int cur_bytes;      // number of bytes in the Current frame
  int pos_offset;     // number of bytes from the beginning of the buffer where Position data starts
  int pos_bytes;      // number of bytes in the Position frame
  int all_offset;     // number of bytes from the beginning of the buffer where data starts (WFSC data)
  int all_bytes;      // number of bytes in the entire frame
  int total_bytes;    // number of bytes in entire DM frame (including SOF and frame number)
  char *buffer;       // pointer to the beginning of the data buffer
  char **framePtr;    // vector of pointers, each pointing to the beginning of each frame in the buffer
  int *nextFrame;     // linked list of next position in buffer to fill
  int active;         // position of the active frame (frame where data is being put)
  char *dataPtr;      // pointer to the beging of the active frame
  int last;           // position the last filled frame
} DServ_Data;

typedef struct _DServ_Info {
  Socket_Info *socket;  // Socket structure in Socket.h
  DServ_Data *data;     // Data structure defined above
  int *receiving_data;  // Flag used to indicate data is being received from the PCR
  int *rate;            // Flag used to indicate data is being received from the PCR
} DServ_Info;

typedef struct _PCR_Thread_Info {
  DServ_Info *cmd;                    // CMD server information
  DServ_Info *wfsc;                   // WFSC server information
  DServ_Info *dm;                     // DDS server information
  DServ_Info *error;                  // Error server information
  Info_Struct *info;                  // Array containing all the Info information
  int sockfd;                         // Socket of client
  int *DServ_continueRunning;         // Flag used to shutdown all PCR clients
  int *PCR_CMD_continueRunning;       // Flags used to shutdown all individual threads
  int *PCR_WFSC_continueRunning;      //
  int *PCR_DM_continueRunning;        //
  int *PCR_Error_continueRunning;     //
  int *PCR_WFSC_running;              // Flags used to indicate the PCR WFSC Client is connected
  int *PCR_DM_running;                // Flags used to indicate the PCR DM Client is connected
  int *PCR_WFSC_reset;                // Flag used to reset PCR WFSC client
  int *PCR_DM_reset;                  // Flag used to reset PCR DM client
  int *DServ_debug;                   // Flag used for debugging all PCR clients
  int *PCR_CMD_debug;                 // Flag used for debugging individual threads
  int *PCR_WFSC_debug;                //
  int *PCR_DM_debug;                  //
  int *PCR_Error_debug;               //
} PCR_Thread_Info;

typedef struct _DServ_Thread_Info {
  DServ_Info *wfsc;                   // Data server information
  DServ_Info *dm;                     // Data server information
  int sockfd;                         // Socket of client
  long *frameNumber;                  // Storelast frame number sent to the client
  int *DServ_WFSC_continueRunning;    // Flag used to shutdown WFSC server
  int *DServ_DM_continueRunning;      // Flag used to shutdown DM server
  int *DServ_WFSC_debug;              // Flag used for debugging WFSC server
  int *DServ_DM_debug;                // Flag used for debugging DM server
} DServ_Thread_Info;

/*
  Structure to send pointer to data memory to PCR_Get*()
*/
typedef struct _PCR_Thread_Client {
  DServ_Info *info;      // Data server information
  int *continueRunning;  // Flag used to shutdown all DServ servers
  int *running;          // Flag indicating this thread is currently running
  int *reset;            // Flag used to reset client (change frame size)
  int *debug;            // Flag used for debugging all DServ servers
} PCR_Thread_Client;

/*
  Prototypes for startup routines
*/
int DServ( int debug );
int DServ_InitWFSC( DServ_Data *Data, int debug );
int DServ_InitDM( DServ_Data *Data, int debug);

/*
  Clients to PCR
*/
int PCR_FindSOF( char *sof, int sockfd, int debug);
void *PCR_GetDM( void *PCR_Thread_Client );
void *PCR_GetWFSC( void *PCR_Thread_Client );
void *PCR_Process( void *PCR_Thread_Info );

/*
  DServ servers
*/
int DServ_GetSingle( DServ_Info *WFSC_Info, DServ_Info *DM_Info,
		     int sockfd, long *old_frameNumber, int *debug);
void *DServ_Master( void *Passed_Info);
int DServ_SendHelp( int sockfd);
void *DServ_Process( void *Passed_Info);
int DServ_Stream( DServ_Info *WFSC_Info, DServ_Info *DM_Info,
		  int sockfd, int *debug);

void *WFSC_Master( void *Passed_Info);
void *WFSC_Process ( void *DServ_Thread_Info);
int WFSC_GetSingle( DServ_Info *Info, int sockfd, int debug);
