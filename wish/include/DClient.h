/* 
   Make MAX_VALUES as large as the largest possible data set to
     to be read.  This is 6400 pixel values when the camera is
     in 80x80 mode
*/

#define MAX_SPOTS 6400
#define MAX_MIRROR 1680
#define MAX_SLOPES 288

#define MAX_VALUES MAX_SPOTS*2+2  // char = 1 byte, int = 2 bytes +2 for frame number

#define MAX_REQUEST 512

/*
  Definitions of sizes and positions of DM data.  Order is important
*/
#define WFSC_OFFSET       0
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

#define TOTAL_DM_VALUES   (WFSC_VALUES + SLOPE_VALUES + RTR_VALUES + CMD_VALUES + CUR_VALUES + POS_VALUES)
#define TOTAL_DM_BYTES    (POS_OFFSET + POS_BYTES)

/*
  Server information
*/

#include "Client.h"
#include "Server.h"
#include "PGplot.h"

/*
  DClient information
*/

/*
  Frame information:
    dims: the number of dimensions (1=vector, 2=array)
          1 - DM vectors: Position, Command, Current, RTR, All
	  2 - WFSC, Slopes
    size: the lengh of each dimension
      WFSC
          [0] - 24/72
	  [1] - 24/72
      Slopes
          [0] - 12
          [1] - 12
      DM (Position, Command, Current, RTR)
          [0] - 336
      DM (All)
          [0] - 336 x 5
 */
typedef struct _DClient_Frame {
  int dims;                  // number of dimensions
  int length[2];             // size of each dimension
} DClient_Frame;

typedef struct _DClient_State {
  int size;                   // old: remove?
  int dimension[2];           // old: remove?
} DClient_State;

typedef struct _DClient_Info {
  Server_Info server;             // structure with server information
  DClient_Frame frame;            // structure with the state of the server
  DClient_State state;            // structure with the state of the server
} DClient_Info;

/*
  Prototypes
*/
/* DClient_Comm.c */
int DClient_ReadString(register int fd, register char *ptr, register int nbytes, char *msg);
int DClient_ReadLong( int sockfd, char *request, char *msg);
int DClient_WriteLong( int sockfd, int longValue, char *msg);
int DClient_WriteString( int sockfd, char *request, char *msg);

/* DClient_Error.c */
int DClient_ParameterError( char *type, char *routine, char *request, int required, 
			    int found, char *returnString);
int DClient_RoutineError( char *type, char *routine, char *request, char *errorMsg, char *returnString);
int DClient_RoutineWarning( char *type, char *request, char *returnString);
int DClient_ConnectionWarning( char *type, char *routine, char *request, char *returnString);

/* Generic routine used by all clients */
int DClient_OpenSocket( char *host_address, short host_port, char *msg);
int DClient_SendDebug( Server_Info *Info, int debug, char *errorMsg);
int Server_GuiUpdate( Tcl_Interp *interp, Server_Info *Info, Gui_Variables *Gui);

/* Server control routines */
int DClient_StartDServ( char *replyData );
int DClient_StartServer( char *serverName, char *replyData );
int DClient_StopServer( char *serverName, char *replyData );

/* WFSC client routines */
int DClient_WFSC_Init( DClient_Info *WFSC_Single_Info, char *name,
		       short **WFSC_Array, int debug, char *errorMsg);
int DClient_Spots_Display( DClient_Info *Client_Info, PGplot_Info *Spots_Info, char *name, char *type,
			   int debug_DClient, int debug_PGplot, char *errorMsg);
int DClient_Spots_Init( DClient_Info *WFSC_Info, PGplot_Info *Spots_Info, char *name,
			short **WFSC_Array, int debug, char *errorMsg);
int DClient_Spots_Single( DClient_Info *Client_Info, short *Spots_Array, int *frameNumber,
			  int debug, char *errorMsg);
int DClient_WFSC_Save( DClient_Info *WFSC_Single_Info, char *fileName, int frames,
		       short *WFSC_Array, int debug, char *errorMsg);
int DClient_WFSC_Single( DClient_Info *WFSC_Single_Info, short *WFSC_Array, int *frameNumber,
			 int debug, char *errorMsg);
int DClient_SaveFrameNumbers( int framesReceived, int *frameNumbers, char *fname, char *time_string);

/* Slope client routines */
int DClient_Slopes_Display( DClient_Info *Client_Info, PGplot_Info *Slopes_Info, char *name, char *type,
			    int debug_DClient, int debug_PGplot, char *errorMsg);
int DClient_Slopes_Init( DClient_Info *Client_Info, PGplot_Info *Slopes_Info, char *name,
			 short **Slopes_Vector, int debug, char *errorMsg);
int DClient_Slopes_Save( DClient_Info *Slopes_Save_Info, char *fname, int frames, 
			 short *Slopes_Array, int debug, char *errorMsg);
int DClient_Slopes_Single( DClient_Info *Slopes_Single_Info, short *Slopes_Array, int *frameNumber,
			   int debug, char *errorMsg);

/* DM client routines */
int DClient_DM_Connect( DClient_Info *DM_Info, char *name,
			int debug, char *errorMsg);
int DClient_DM_Init( DClient_Info *Slopes_Single_Info, char *name,
		       char **DM_Array, int debug, char *errorMsg);
int DClient_RTR_Single( DClient_Info *RTR_Single_Info, char *RTR_Array, int *frameNumber,
			int debug, char *errorMsg);
int DClient_DM_Save( DClient_Info *DM_Save_Info, char *request, char *dirname, char *fname, int frames,
		     int debug, char *errorMsg);
