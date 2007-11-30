/**********************************************************
 * Status.h
 **********************************************************

  Definitions and prototypes for Status routines.  This file
    is included in all Status routines as well as the top level
    routine, Topbox_Server.h

  Written 13aug07  DLM  Started with Info.h from pcr_server

 */

#include "Socket.h"

/*
  Number of connections to Status server possible
*/
#define STATUS_CONNECTIONS 20

typedef struct _Status_Entry {
  char *name;                   // Name of Status Variable
  char *description;            // Short description
  char *type;                   // Type of value to be stored
  int i_value;                  //   type "i"
  int *i_ptr;                   //   type "I"
  float f_value;                //   type "f"
  float *f_ptr;                 //   type "F"
  char s_value[STRING_LENGTH];  //   type "s"
  char *s_ptr;                  //   type "S"
} Status_Entry;

typedef struct _Status_Struct {
  Socket_Info *socket;
  Status_Entry *list;
  int length;
} Status_Struct;

typedef struct _Status_Thread_Info {
  Status_Struct *status;               // Status data server information
  int sockfd;                          // Socket of client
  int *continueRunning;                // Flag used to shutdown Status server
  int *debug;                          // Flag used for debugging Status server
} Status_Thread_Info;

/*
  Prototypes for Status routines
*/
int Status_FindParameter( Status_Entry *List, long list_length, char *request, long *position, long debug);
int Status_InitFlags( Socket_Info *Info, Device_Data *Data);
void *Status_Master( void *Passed_Info);
void *Status_Process( void *Passed_Info);
int Status_ReadFile( Status_Entry *List, long list_length, char *filename, long debug);
int Status_ReadLine(FILE *f, char *header);
int Status_SendAll( int clientfd, Status_Entry *List, long list_length);
int Status_SendHelp( int sockfd);
int Status_SetParameter( int sockfd, Status_Entry *List, long list_length, int debug);

int Status_SetFlagPointers( Status_Entry *List, long list_length,
			    Socket_Info *Info,
			    Device_Data *Data,
			    char *Connected_String,
			    char *Connection_Error_String,
			    char *Home_String,
			    char *Position_String,
			    char *Moving_String,
			    char *In_Position_String,
			    char *Error_String,
			    char *Enabled_String,
			    char *Negative_String,
			    char *Positive_String,
			    char *Event_2_String,
			    long debug);

int Status_SetFwheelPointers( Status_Entry *List, long list_length,
			      Socket_Info *Info,
			      Device_Data *Data,
			      char *Connected_String,
			      char *Connection_Error_String,
			      char *Home_String,
			      char *Position_String,
			      char *Moving_String,
			      char *Name_String,
			      long debug);

int Status_SetPowerPointers( Status_Entry *List, long list_length,
			     Socket_Info *Info,
			     Device_Data *Data,
			     char *Connected_String,
			     char *Connection_Error_String,
			     char *WFSC_String,
			     char *Peltier_String,
			     char *Stella_String,
			     char *Alignment_Laser_String,
			     char *F15_Laser_String,
			     char *Indigo_String,
			     char *Spare1_String,
			     char *Spare2_String,
			     long debug);

int Status_ZeroFlags( Device_Data *Data);

/* THE END */
