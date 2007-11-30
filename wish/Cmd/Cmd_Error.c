/*===========================================================================*
**
**  Cmd_Error.c
**
**    Cmd_ParameterError()
**    Cmd_RoutineError()
**    Cmd_ConnectionWarning()
**
**  Build an appropriate error message for output and return it and an error
**    flag to the calling TclTk routine.
**
**  Returns :  0 success
**            -1 error
**
**  Written   8apr04  DLM
**
**  Altered  29dec04  DLM  Change name from FSM_*Error().  Now use Socket_Info
**                           instead of Motor_Info.
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "Socket.h"
#include "Cmd.h"

/*=========================================
 * function: Cmd_ParameterError
 *=========================================*/

int Cmd_ParameterError( Tcl_Interp *interp, Socket_Info *Info, char *type, char *routine,
			  char *request, char *axis, int required, int found, char *returnString)
{
  char errorMsg[IO_LENGTH];

  sprintf( errorMsg, "%s in %s: function %s:%s requires %d arguments, found %d",
	   type, routine, request, axis, required, found);
  printf("%s\n", errorMsg);
  Tcl_AppendResult(interp, errorMsg, (char *) NULL);
  strcpy(returnString, errorMsg);

  return(0);
}

/*=========================================
 * function: Cmd_RoutineError
 *=========================================*/

int Cmd_RoutineError( Tcl_Interp *interp, Socket_Info *Info, char *type, char *routine,
			char *request, char *returnString)
{
  char errorMsg[IO_LENGTH];

  sprintf( errorMsg, "%s in function %s:%s, motor %s:%s\n%s",
	   type, routine, request, Info->motor, Info->name, Info->in_line);
  printf("%s\n", errorMsg);
  Tcl_AppendResult(interp, errorMsg, (char *) NULL);
  strcpy(returnString, errorMsg);

  return(0);
}

/*=========================================
 * function: Cmd_ConnectionWarning
 *=========================================*/

int Cmd_ConnectionWarning(  Tcl_Interp *interp, Socket_Info *Info, char *type, char *routine,
			     char *request, char *returnString)
{
  char errorMsg[IO_LENGTH];

  sprintf( errorMsg, "%s in function %s:%s, motor %s:%s is not connected",
	   type, routine, request, Info->motor, Info->name);
  printf("%s\n", errorMsg);
  Tcl_AppendResult(interp, errorMsg, (char *) NULL);
  strcpy(returnString, errorMsg);

  return(0);
}
