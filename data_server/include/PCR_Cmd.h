/*
  Definitions and prototypes used by the PCR_* routines

  Written  27jan07  DLM  Separated PCR routines from DServ.h
*/

/*
  These default camera bias definitions should go somewhere else
    29jan07  DLM
*/
#define WFSC_BIAS_FULL_0 450
#define WFSC_BIAS_FULL_1 451
#define WFSC_BIAS_FULL_2 452
#define WFSC_BIAS_FULL_3 453

#define WFSC_BIAS_BIN_0 460
#define WFSC_BIAS_BIN_1 465
#define WFSC_BIAS_BIN_2 511
#define WFSC_BIAS_BIN_3 509

/* prototypes */
int PCR_SendHelp( int sockfd);
int PCR_SendCmd( DServ_Info *Info, char *request, int *debug );
int PCR_SendCmd_NoReply( DServ_Info *Info, char *request, int sockfd, int *debug );
int PCR_SendSlopeOffsets( DServ_Info *Info, Info_Entry *List, int sockfd, int *debug );
int PCR_SendFloatVector( DServ_Info *Info, int sockfd, char *name, int *debug );
int PCR_SendIntVector( DServ_Info *Info, int sockfd, char *name, int *debug );
int PCR_SendShortVector( DServ_Info *Info, int sockfd, char *name, int *debug );
