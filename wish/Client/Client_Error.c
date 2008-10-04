/*===========================================================================*
**
**  Client_Error.c
**
**  Builds an error message from the parameters passed to the calling routine
**    and send this message back to the Tcl routine which made the original
**    call
**
**  Written  17jun05  DLM  Started with Msg_Error.c
**
**  Altered  22jan07  DLM
**
**===========================================================================*/

#include "PCR_Wish.h"

/*=========================================
 * function: Client_ParameterError
 *=========================================*/
int Client_ParameterError( Tcl_Interp *interp, char *type, char *routine, char *request, 
			int required, int found, char *returnString, int debug)
{
  char errorMsg[IO_LENGTH];

  sprintf( errorMsg, "%s in %s: function %s requires %d arguments, found %d",
	   type, routine, request, required, found);
  if ( debug ) {
    printf("%s\n", errorMsg);
  }
  Tcl_AppendResult(interp, errorMsg, (char *) NULL);
  strcpy(returnString, errorMsg);

  return(0);
}

/*=========================================
 * function: Client_RoutineError
 *=========================================*/
int Client_RoutineError( Tcl_Interp *interp, char *type, char *routine, char *request,
		      char *message, char *returnString, int debug)
{
  char errorMsg[IO_LENGTH];

  sprintf( errorMsg, "%s in function %s:%s\n%s", type, routine, request, message);
  if ( debug ) {
    printf("%s\n", errorMsg);
  }
  Tcl_AppendResult(interp, errorMsg, (char *) NULL);
  strcpy(returnString, errorMsg);

  return(0);
}

/*=========================================
 * function: Client_ConnectionWarning
 *=========================================*/
int Client_ConnectionWarning(  Tcl_Interp *interp, char *type, char *routine, char * name,
			    char *request, char *returnString, int debug)
{
  char errorMsg[IO_LENGTH];

  sprintf( errorMsg, "%s in function %s:%s, %s is not connected",
	   type, routine, request, name);
  if ( debug ) {
    printf("%s\n", errorMsg);
  }
  Tcl_AppendResult(interp, errorMsg, (char *) NULL);
  strcpy(returnString, errorMsg);

  return(0);
}
