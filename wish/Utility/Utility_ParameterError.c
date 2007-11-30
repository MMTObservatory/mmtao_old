/*===========================================================================*
**
**  Utility_Error.c
**
**    Utility_ParameterError()
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
#include "Utility.h"

/*=========================================
 * function: Utility_ParameterError
 *=========================================*/

int Utility_ParameterError( char *type, char *routine, char *request, int required, 
			    int found, char *returnString)
{
  char tempMsg[IO_LENGTH];

  sprintf( tempMsg, "%s in %s: function %s requires %d arguments, found %d",
	   type, routine, request, required, found);
  strcpy(returnString, tempMsg);

  return(0);
}
