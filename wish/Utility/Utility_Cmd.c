/*===========================================================================*
**
**  Utility_Cmd.c
**
**  In this section, new TclTk procedures written in C are created.
**  ---------------------------------------------------------------
**
**  All new procedures must be define above in Tcl_AppInit as well as
**    declared in the headed (see List of new Tcl Commands section).
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "Client.h"
#include "Server.h"
#include "Utility.h"

#include "Cmd_Globals.h"
#include "Cmd_Prototypes.h"

extern char *reply_data;

/*===========================================================================*
 * Utility_Cmd
 *===========================================================================*/

int Utility_Cmd( ClientData client_data, Tcl_Interp* interp, int argc, char *argv[])
{

  /* Variables sent by TclTk and passed on to the subroutines */
  char name[STRING_LENGTH];
  char request[STRING_LENGTH];

  /* Character string used to send information back from called routines.
       This information is passed back to TckTk via the linked variable
       reply_data.
  */
  char errorMsg[IO_LENGTH];

  /* Time variables */

  time_t *tp;
  struct tm *stp;
  char time_string[40];

  /* Local variables */
  int required_args;
  int status;
  int error;

/*************************************************************************
**
**  Check that we have at least one parameter.  This is the request and
**    is copied into name[].
**
**************************************************************************/

  Tcl_ResetResult(interp);

  required_args = 1;

  if (argc < 1+required_args ) {
    status = Utility_ParameterError( "ERROR", argv[0], argv[1], required_args,
				     argc-2, reply_data);
    return TCL_ERROR;
  }
  strcpy( name, argv[1]);

  if ( debug_Utility ) {
    printf("  Utility_Cmd: Request = %s\n", name);
  }

  strcpy( reply_data, "Utility_Cmd OK");
  strcpy( errorMsg, "");

/**************************************************************************
**
**  Process Request
**
**************************************************************************/

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  strcpy( request, name);
  status = Server_StringLowerCase(request);

  if ( !strncmp( request, "current_time", strlen("current_time") ) ) {

    /*
      Get system time from built-in C routine

      For the purpose of writing files to disk, we will consider
        one day to go from 12:00 noon to 12:00 noon.
        In this case, the date (ie the day of the month) for one
	day will include from 12:00 to 12:00 midnight of that day plus
	12:00 midnight to 12:00 noon of the next day.  Thus, we must
	check to see if the current time is AM or PM, and possibly
	decrease the date by one.

      For the date to save in the header of files we will use the correct
        date and and time (on a 24 hour clock).

      First, get current time in pointer to arithmetic variable, *tp
        and create date and time strings
    */
    tp = (time_t *)malloc( sizeof(time_t) );
    time( (time_t *)tp );

    status = strftime( time_string, sizeof(time_string), "%y%m%d", localtime( tp));
    strcpy(reply_data, time_string);

    status = strftime( time_string, sizeof(time_string), "%H.%M.%S", localtime( tp));
    strcat(reply_data, " ");
    strcat(reply_data, time_string);

    /*
      Next, convert arithmetic time, *tp, to structure containing
        time, *tm, check if the time is in the AM.  If so, then
	decrement the day by one.  We must check to see if we
	are on the first day of the month.  If we are, then we
	must decrease the month by one
    */

    stp = localtime( tp);

    if ( stp->tm_hour < 12 ) {
      if ( stp->tm_mday == 1) {
	stp->tm_mon -=1;
      }
      stp->tm_mday -=1;
    }

    *tp = mktime( stp);

    status = strftime( time_string, sizeof(time_string), "%y%m%d", localtime( tp));

    strcat(reply_data, " ");
    strcat(reply_data, time_string);

    if ( debug_Utility ) {
      printf("  Utility_Cmd: Current Time = %s\n", reply_data);
    }

 } else if ( !strncmp( request, "ut_time", strlen("ut_time") ) ) {

    /********************************************************
     *  Retreive the Universal Time
     ********************************************************/

    required_args = 0;

    if (argc != 2+required_args ) {
      error = Client_ParameterError( interp, "ERROR", argv[0], argv[1], required_args,
				       argc-2, reply_data, debug_Utility);
      return TCL_ERROR;
    }

    /*
      Get UT time from built-in C routine

      For the purpose of writing files to disk

      For the date to save in the header of files we will use the correct
        date and and time (on a 24 hour clock).

      First, get current time in pointer to arithmetic variable, *tp
        and create date and time strings
    */
    tp = (time_t *)malloc( sizeof(time_t) );
    time( (time_t *)tp );

    status = strftime( time_string, sizeof(time_string), "%y%m%d", gmtime( tp));
    strcpy(reply_data, time_string);

    status = strftime( time_string, sizeof(time_string), "%H.%M.%S", gmtime( tp));
    strcat(reply_data, " ");
    strcat(reply_data, time_string);

    if ( debug_Utility ) {
      printf("  Utility_Cmd: UT Time = %s\n", reply_data);
    }

/**************************************************************************
**
**  Unknown call and cleanup
**
**************************************************************************/

  } else {

    sprintf( errorMsg, "Unknown request: %s", argv[1]);
    error = Client_RoutineError( interp, "ERROR", argv[0], argv[1], errorMsg,
				   reply_data, debug_Utility);
    return TCL_ERROR;

  }

  Tcl_AppendResult(interp,"OK", NULL);

  return TCL_OK;
}
