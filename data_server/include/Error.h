/**********************************************************
 * Error.h
 **********************************************************

  Definitions and prototypes for Error routines.  This file
    is included in all Info routines as well at the 
    standard "mmtao.h" file.

  Written 15mar07  DLM

 */

/*
  Prototypes for Error routines
*/
void *Error_Master( void *Passed_Info);
int Error_FindMessage( int errorNumber, char *errorMessage, int debug);
int Error_PlaySound( int debug);
