/*===========================================================================*
**
**  PCR_Error.c
**
**    PCR_ParameterError()
**    PCR_RoutineError()
**    PCR_RoutineWarning()
**    PCR_ConnectionWarning()
**
**  Build an appropriate error message for output and return it and an error
**    flag to the calling TclTk routine.
**
**  Returns :  0 success
**            -1 error
**
**  Written   8apr04  DLM  
**
**===========================================================================*/

#include "PCR_Wish.h"

/*=========================================
 * function: PCR_ParameterError
 *=========================================*/

int PCR_ParameterError( char *type, char *routine, char *request, int required, 
			    int found, char *returnString)
{
  char tempMsg[IO_LENGTH];

  sprintf( tempMsg, "%s in %s: function %s requires %d arguments, found %d",
	   type, routine, request, required, found);
  strcpy(returnString, tempMsg);

  return(0);
}

/*=========================================
 * function: PCR_RoutineError
 *=========================================*/

int PCR_RoutineError( char *type, char *routine, char *request, char *errorMsg,
			  char *returnString)
{
  char tempMsg[IO_LENGTH];

  sprintf( tempMsg, "%s in function %s:%s\n%s",
	   type, routine, request, errorMsg);
  strcpy(returnString, tempMsg);

  return(0);
}

/*=========================================
 * function: PCR_RoutineWarning
 *=========================================*/

int PCR_RoutineWarning( char *type, char *request, char *returnString)
{
  char tempMsg[IO_LENGTH];

  sprintf( tempMsg, "%s: %s", type, request);
  strcpy(returnString, tempMsg);

  return(0);
}

/*=========================================
 * function: PCR_ConnectionWarning
 *=========================================*/

int PCR_ConnectionWarning( char *type, char *routine, char *request, char *returnString)
{
  char tempMsg[IO_LENGTH];

  sprintf( tempMsg, "%s in function %s:%s Server is not connected",
	   type, routine, request);
  strcpy(returnString, tempMsg);

  return(0);
}
