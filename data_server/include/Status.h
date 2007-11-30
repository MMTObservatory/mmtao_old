/**********************************************************
 * Status.h
 **********************************************************

  Definitions and prototypes for Status routines.  This file
    is included in all Info routines as well at the 
    standard "mmtao.h" file.

  Written 1feb07  DLM

 */

#include "Socket.h"

#define STATUS_REPLY_LENGTH 4096

typedef struct _Status_Thread {
  Socket_Info *socket;                 // Information about connection to PCR
  Info_Struct *info;                   // Info data server information
  int *continueRunning;                // Flag used to shutdown Info server
  int *debug;                          // Flag used for debugging Info server
} Status_Thread;

/*
  Prototypes for Info routines
*/
int Status_EndOfData( char *input );
void *Status_Master( void *Passed_Info);
int Status_ParameterPositions( Info_Struct *Info, char *Status_Reply, long **Status_Position,
			       long *Status_Count, long debug);
int Status_ReadLine( register int fd, register char *ptr, register int maxlen );
int Status_ReadToEOF( Socket_Info *Info, char *Status_Reply, long *Status_Lines, int debug);
int Status_UpdateParameters( Info_Struct *Info, char *Status_Reply, long *Status_Position,
			     long Status_Count, long debug);
