/*===========================================================================*
**
**  VoltMeter_Error.c
**
**    VoltMeter_ParameterError()
**    VoltMeter_RoutineError()
**    VoltMeter_ConnectionWarning()
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
#include "VoltMeter.h"

/*=========================================
 * function: VoltMeter_ParameterError
 *=========================================*/

int VoltMeter_ParameterError( Tcl_Interp *interp, Motor_Info *Info, char *type, char *routine,
			      char *request, int required, int found, char *returnString)
{
  char errorMsg[IO_LENGTH];

  sprintf( errorMsg, "%s in %s: function %s requires %d arguments, found %d",
	   type, routine, request, required, found);
  printf("%s\n", errorMsg);
  Tcl_AppendResult(interp, errorMsg, (char *) NULL);
  strcpy(returnString, errorMsg);

  return(0);
}

/*=========================================
 * function: VoltMeter_RoutineError
 *=========================================*/

int VoltMeter_RoutineError( Tcl_Interp *interp, Motor_Info *Info, char *type, char *routine,
		      char *request, char *returnString)
{
  char errorMsg[IO_LENGTH];

  sprintf( errorMsg, "%s in function %s:%s, %s:%s\n%s",
	   type, routine, request, Info->motor, Info->name, Info->in_line);
  printf("%s\n", errorMsg);
  Tcl_AppendResult(interp, errorMsg, (char *) NULL);
  strcpy(returnString, errorMsg);

  return(0);
}
/*=========================================
 * function: VoltMeter_ConnectionWarning
 *=========================================*/

int VoltMeter_ConnectionWarning(  Tcl_Interp *interp, Motor_Info *Info, char *type, char *routine,
			     char *request, char *returnString)
{
  char errorMsg[IO_LENGTH];

  sprintf( errorMsg, "%s in function %s:%s, %s:%s is not connected",
	   type, routine, request, Info->motor, Info->name);
  printf("%s\n", errorMsg);
  Tcl_AppendResult(interp, errorMsg, (char *) NULL);
  strcpy(returnString, errorMsg);

  return(0);
}
