/*===========================================================================*
**
**  Science_Cmd.c
**
**  In this section, new TclTk procedures written in C are created.
**  ---------------------------------------------------------------
**
**  All new procedures must be define above in Tcl_AppInit as well as
**    declared in the headed (see List of new Tcl Commands section).
**
**  2008-10-28 skip
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "Science.h"
#include "Server_Ports.h"

#include "Client_Globals.h"
#include "Client_Prototypes.h"

extern char *reply_data;


/*===========================================================================*
 * Science_Cmd
 *===========================================================================*/

int Science_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
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
  static Client_Info Science_Info;
  static Gui_Variables Science_GuiVariables;
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
				     argc-2, reply_data, debug_Science);
    if ( debug_Science ) {
        printf("%s\n", reply_data);
    }
    return TCL_ERROR;
  }
  strcpy( name, argv[1]);

  strcpy( reply_data, "OK: Science_Cmd");
  strcpy( errorMsg, "");

/**************************************************************************
**
**  Initialize socket information structures
**
**************************************************************************/

  if ( first_time ) {

    /*
      Setup Science Client Info structure and Gui variables
    */

    status = Client_SetupInfo( &Science_Info, &Science_GuiVariables, "Science",
			       &Science_Connected, &Science_Error);

    first_time = 0;

    status = Client_GuiUpdate( interp, &Science_Info, &Science_GuiVariables );

  }

/**************************************************************************
**
**  Send request to Science Server
**
**************************************************************************/

  if ( debug_Science ) {
    printf("  Science_Cmd: request = %s\n", name);
  }

  /*
    Check that a request was sent
  */
  if ( strlen(name) <= 1 ) {
    printf("  Science_Cmd: Warning: request of zero length recieved\n");
    return(-1);
  }

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  strcpy( request, name);
  status = Server_StringLowerCase(request);

  /*
    All commands require a connection to the DServ server, so every time
      Science_Cmd is called, we will check to make sure we are connected, or
      will initiate this connection
  */
  if ( !Science_Info.connected ) {

    status = Client_Open( &Science_Info, debug_Science);
    if ( status ) {
      sprintf( errorMsg, "Error connecting to %s", Science_Info.name);
      status = Client_GuiUpdate( interp, &Science_Info, &Science_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				    reply_data, debug_Science);
      return TCL_ERROR;
    }

    if ( debug_Science ) {
      printf("  Science_Cmd: Connected to Science_Server\n");
    }
  }

  if ( !strncmp( request, "connect", strlen("connect") ) ) {

    /********************************************************
     *  This is already done above, but in case the user send the
     *    connect request, the stub is here
     ********************************************************/

  } else if ( !strncmp( request, "close", strlen("close") ) ) {

    /********************************************************
     *  Close a connection to the Science Server
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				      argc-2, reply_data, debug_Science);
      return TCL_ERROR;
    }

    if ( !Science_Info.connected ) {
      return TCL_OK;
    }

    status = Client_Close( &Science_Info, debug_Science);
    if ( status ) {
      sprintf( errorMsg, "Error closing connection to %s", Science_Info.name);
      status = Client_GuiUpdate( interp, &Science_Info, &Science_GuiVariables);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				    reply_data, debug_Science);
      return TCL_ERROR;
    }

  } else if ( !strncmp( request, "send", strlen("send") ) ) {
    
    /********************************************************
     *  Send a request to the Science Server
    printf("  argc = %d\n", argc);
     ********************************************************/


    required_args = 1;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Science);
      return TCL_ERROR;
    }

    i=2;
    strcpy( variable, argv[i++]);
    sprintf( request, "%s\n", variable);
    if ( debug_Science ) {
      printf("%s\n", request);
    }

    status = Client_Write( Science_Info.fd, request, strlen(request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Science);
      return TCL_ERROR;
    }

    status = Client_ReadLine( Science_Info.fd, Science_Reply, CLIENT_REPLY_LENGTH);
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
                                 reply_data, debug_Science);
         return TCL_ERROR;
    }


  } else if ( !strncmp( request, "all", strlen("all") ) ) {

    /********************************************************
     *  Get variables from Science Server
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      status = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Science);
      return TCL_ERROR;
    }

    i=2;
    strcpy( request, "all\n");

    status = Client_Write( Science_Info.fd, request, strlen(request));
    if ( status ) {
      sprintf( errorMsg, "Error with %s", request);
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Science);
      return TCL_ERROR;
    }

    status = Client_ReadToEOF( &Science_Info, Science_Reply, errorMsg, debug_local);
    if ( status ) {
      status = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				 reply_data, debug_Science);
      return TCL_ERROR;
    }

    /* strip out any \r (like from the old server) */
    {
        int n = strlen(Science_Reply);
	char *ip = Science_Reply;
	char *op = Science_Reply;
	char c;
	while (ip < Science_Reply + n) {
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
				   reply_data, debug_Science);
    return TCL_ERROR;

  }

  Tcl_AppendResult(interp,"OK", NULL);
  strcpy(reply_data, "OK Fine");

  status = Client_GuiUpdate( interp, &Science_Info, &Science_GuiVariables);
  return TCL_OK;
}
