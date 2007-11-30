/**********************************************************
 * Info.h
 **********************************************************

  Definitions and prototypes for Info routines.  This file
    is included in all Info routines as well at the 
    standard "mmtao.h" file.

  Written 19may05  DLM

 */

#include "Socket.h"

/* 
  Info array size.  This is used for seeing values and strehl values
*/
#define SEEING_VALUES     10

/*
  Number of connections to Info server possible
*/
#define INFO_CONNECTIONS 20

typedef struct _Info_Entry {
  char *name;                   // Name of Info Variable
  char *description;            // Short description
  char *type;                   // Type of value to be stored
  int i_value;                  //   type "i"
  int *i_ptr;                   //   type "I"
  float f_value;                //   type "f"
  float *f_ptr;                 //   type "F"
  char s_value[STRING_LENGTH];  //   type "s"
} Info_Entry;

typedef struct _Info_Array {
  long array_size;
  long total_number;
  long position;
  char time[SEEING_VALUES][STRING_LENGTH];
  double value[SEEING_VALUES];
} Info_Array;

typedef struct _Info_Struct {
  Socket_Info *socket;
  Info_Entry *list;
  int length;
  Info_Array *seeing;
  Info_Array *strehl;
} Info_Struct;

typedef struct _Info_Thread {
  Info_Struct *info;                   // Info data server information
  int sockfd;                          // Socket of client
  int *continueRunning;                // Flag used to shutdown Info server
  int *debug;                          // Flag used for debugging Info server
} Info_Thread;

/*
  Prototypes for Info routines
*/
int Info_FindParameter( Info_Entry *List, long list_length, char *request, long *position, long debug);
void *Info_Master( void *Passed_Info);
void *Info_Process( void *Passed_Info);
int Info_ReadFile( Info_Entry *List, long list_length, char *filename, long debug);
int Info_ReadLine(FILE *f, char *header);
int Info_SeeingPositions( Info_Struct *Info, long **Seeing_Positions,
			  long *Seeing_Count, long debug);
int Info_SendAll( int clientfd, Info_Entry *List, long list_length);
int Info_SendHelp( int sockfd);
int Info_SendSeeing( int sockfd, Info_Entry *List, long list_length, long *Seeing_Positions, long Seeing_Count);
int Info_SetParameter( int sockfd, Info_Entry *List, long list_length, Info_Array *Seeing, 
		       Info_Array *Strehl, int debug);
int Info_SetFlagPointers( Info_Entry *List, long list_length, int *PCR_Client_running,
			  int *DServ_WFSC_running, int *DServ_DDS_running, int *Info_running,
			  int *PCR_WFSC_receiving_data, int *PCR_DDS_receiving_data,
			  int *PCR_WFSC_rate, int *PCR_DDS_rate,
			  Socket_Info *PCR_CMD_Socket, Socket_Info *PCR_WFSC_Socket,
			  Socket_Info *PCR_DDS_Socket,
			  Socket_Info *PCR_Error_Socket, long debug);
