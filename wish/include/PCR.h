/* 
   PCR.h
*/

/*
  Prototypes
*/
int PCR_Connect( Server_Info *PCR_Info, int debug, char *errorMsg);
int PCR_Request( Server_Info *PCR_Info, char *request, char *errorMsg);
int PCR_Write( register int fd, register char *ptr, register int nbytes);
int PCR_ParameterError( char *type, char *routine, char *request, int required, 
			int found, char *returnString);
int PCR_RoutineError( char *type, char *routine, char *request, char *errorMsg,
		      char *returnString);
int PCR_RoutineWarning( char *type, char *request, char *returnString);
int PCR_ConnectionWarning( char *type, char *routine, char *request, char *returnString);
