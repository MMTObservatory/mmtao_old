/*===========================================================================*
**
**  TSS_Cmd.c
**
**  In this section, new TclTk procedures written in C are created.
**  ---------------------------------------------------------------
**
**  All new procedures must be define above in Tcl_AppInit as well as
**    declared in the headed (see List of new Tcl Commands section).
**
**  2008-11-03 skip
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "TSS.h"
#include "Server_Ports.h"

#include "Client_Globals.h"
#include "Client_Prototypes.h"

extern char *reply_data;


/*===========================================================================*
 * TSS_Cmd
 *===========================================================================*/

int TSS_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
{

  /* Variables sent by TclTk and passed on to the subroutines */
  char name[STRING_LENGTH];
  char variable[STRING_LENGTH];
  char request[STRING_LENGTH];

  /* Character string used to send information back from called routines.
       This information is passed back to TckTk via the linked variable
       reply_data.
  */
  char errorMsg[IO_LENGTH];

  /* Server variables */
  static Client_Info TSS_Info;
  static Gui_Variables TSS_GuiVariables;
  static int first_time = 1;

  /* Local variables */
  int required_args;
  int status, i;
  int debug_local = 0;

/*************************************************************************
**
**  Check that we have at least one parameter.  This is the request and
**    is copied into name[].
**
**************************************************************************/

  Tcl_ResetResult(interp);

  required_args = 1;

  if (argc < 1+required_args ) {
    status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				     argc-2, reply_data, debug_TSS);
    if ( debug_TSS ) {
        printf("%s\n", reply_data);
    }
    return TCL_ERROR;
  }
  strcpy( name, argv[1]);

  strcpy( reply_data, "OK: TSS_Cmd");
  strcpy( errorMsg, "");

/**************************************************************************
**
**  Initialize socket information structures
**
**************************************************************************/

  if ( first_time ) {

    /*
      Setup TSS Client Info structure and Gui variables
    */

    status = Client_SetupInfo( &TSS_Info, &TSS_GuiVariables, "TSS",
			       &TSS_Connected, &TSS_Error);

    first_time = 0;

    status = Client_GuiUpdate( interp, &TSS_Info, &TSS_GuiVariables );

  }

/**************************************************************************
**
**  Send request to TSS Server
**
**************************************************************************/

  if ( debug_TSS ) {
    printf("  TSS_Cmd: request = %s\n", name);
  }

  /*
    Check that a request was sent
  */
  if ( strlen(name) <= 1 ) {
    printf("  TSS_Cmd: Warning: request of zero length recieved\n");
    return(-1);
  }

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  strcpy( request, name);
  status = Server_StringLowerCase(request);

  /*
    All commands require a connection to the DServ server, so every time
      TSS_Cmd is called, we will check to make sure we are connected, or
      will initiate this connection
  */
  if ( !TSS_Info.connected ) {

    status = Client_Open( &TSS_Info, debug_TSS);
    if ( status ) {
      sprintf( errorMsg, "Error connecting to %s", TSS_Info.name);
      status = Client_GuiUpdate( interp, &TSS_Info, &TSS_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				    reply_data, debug_TSS);
      return TCL_ERROR;
    }

    if ( debug_TSS ) {
      printf("  TSS_Cmd: Connected to TSS_Server\n");
    }
  }

  if ( !strncmp( request, "connect", strlen("connect") ) ) {

    /********************************************************
     *  This is already done above, but in case the user send the
     *    connect request, the stub is here
     ********************************************************/

  } else if ( !strncmp( request, "close", strlen("close") ) ) {

    /********************************************************
     *  Close a connection to the TSS Server
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				      argc-2, reply_data, debug_TSS);
      return TCL_ERROR;
    }

    if ( !TSS_Info.connected ) {
      return TCL_OK;
    }

    status = Client_Close( &TSS_Info, debug_TSS);
    if ( status ) {
      sprintf( errorMsg, "Error closing connection to %s", TSS_Info.name);
      status = Client_GuiUpdate( interp, &TSS_Info, &TSS_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				    reply_data, debug_TSS);
      return TCL_ERROR;
    }

  } else if ( !strncmp( request, "send", strlen("send") ) ) {
    
    /********************************************************
     *  Send a request to the TSS Server
    printf("  argc = %d\n", argc);
     ********************************************************/


    required_args = 1;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_TSS);
      return TCL_ERROR;
    }

    i=2;
    strcpy( variable, argv[i++]);
    sprintf( request, "%s\n", variable);
    if ( debug_TSS ) {
      printf("%s\n", request);
    }

    status = Client_Write( TSS_Info.fd, request, strlen(request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TSS);
      return TCL_ERROR;
    }

    status = Client_ReadLine( TSS_Info.fd, TSS_Reply, CLIENT_REPLY_LENGTH);
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
                                 reply_data, debug_TSS);
         return TCL_ERROR;
    }


  } else if ( !strncmp( request, "all", strlen("all") ) ) {

    /********************************************************
     *  Get variables from TSS Server
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_TSS);
      return TCL_ERROR;
    }

    i=2;
    strcpy( request, "all\n");

    status = Client_Write( TSS_Info.fd, request, strlen(request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TSS);
      return TCL_ERROR;
    }

    status = Client_ReadToEOF( &TSS_Info, TSS_Reply, errorMsg, debug_local);
    if ( status ) {
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_TSS);
      return TCL_ERROR;
    }

    /* strip out any \r (like from the old server) */
    {
        int n = strlen(TSS_Reply);
	char *ip = TSS_Reply;
	char *op = TSS_Reply;
	char c;
	while (ip < TSS_Reply + n) {
	    c = *ip++;
	    if (c != '\r')
	        *op++ = c;
	}
	*op = 0;
    }

/**************************************************************************
**
**  Unknown call and cleanup
**
**************************************************************************/

  } else {

    sprintf( errorMsg, "Unknown request: %s", argv[1]);
    status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				   reply_data, debug_TSS);
    return TCL_ERROR;

  }

  Tcl_AppendResult(interp,"OK", NULL);
  strcpy(reply_data, "OK Fine");

  status = Client_GuiUpdate( interp, &TSS_Info, &TSS_GuiVariables);
  return TCL_OK;
}
