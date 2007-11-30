/****************************
  Cmd information
****************************/

/* Prototypes */
int Cmd_ConnectionWarning(  Tcl_Interp *interp, Socket_Info *Info, char *type,
			    char *routine, char *request, char *returnString);
int Cmd_ParameterError( Tcl_Interp *interp, Socket_Info *Info, char *type, char *routine,
			char *request, char *axis, int required, int found,
			char *returnString);
int Cmd_RoutineError( Tcl_Interp *interp, Socket_Info *Info, char *type, char *routine,
		      char *request, char *returnString);
