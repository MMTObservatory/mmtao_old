/*
  Define the prototypes for the Source routines

  This should be included in all routines which use
    and Source_* routines

  Written 15aug07  DLM
*/

typedef struct _Source_Info {
  Socket_Info *socket;   // Socket structure in Socket.h
  Device_Data *data;     // Data structure defined above
  float *distance;       // Requested relative distance to move (float for Stage motion)
  int *home_trigger;     // Home trigger to stop motor motion when flag is tripped
  int *remaining;        // Remaining distance to move (int for MVP motion)
  int *relative;         // Requested relative distance to move (int for MVP motion)
  int *absolute;         // Requested absolute position (int for MVP motion)
  int *running;          // Flag indicating if the status thread for this device is running
} Source_Info;

/* Source Routine Prototypes */
void *Source_MVP( void *Passed_Info);
int Source_ReadMVP( int sockfd, Socket_Info *Socket, char *reply);
void *Source_RelativeMVP( void *Passed_Info);
void *Source_Stage( void *Passed_Info);
int Source_StageRead( int sockfd, Socket_Info *Socket, char *reply);
void *Source_StageRelativeMove( void *Passed_Info);
int Source_StatusMVP( Socket_Info *Socket, Device_Data *Data, char *statusString, int debug);
