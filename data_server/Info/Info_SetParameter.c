/*=======================================================================
 * Info_SetParameter.c
 *=======================================================================

  This routine send a string with the Info Parameter name and value
    separated by a space and delimited by a newline (\n) to the
    requesting client.

  Client are only allowed to set character parameters ("s") so when the
    the position of the Info parameter is found we need to check the
    it is a character parameter

  Written  17may05  DLM  Started with SInfo_FileNameSave.c

  Altered  19nov06  DLM  Modified to work with DServ for the PCR

*/

#include "PCR.h"
#include "Info.h"

#define MAX_TIME_LENGTH 20

/*================================================================================*
 * Return an Info Parameter and its value
 *================================================================================*/
int Info_SetParameter( int sockfd, Info_Entry *List, int list_length, Info_Array *Seeing,
                    Info_Array *Strehl, int debug)
{

  /* local variables */

  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  char value[STRING_LENGTH];
  int position;
  int status;

  /* time variable */
  time_t tp;
  char time_string[MAX_TIME_LENGTH];
  struct tm tms;

  /*
    Allocate structure needed for timestamp

  if ( first_time ) {
    systime = (struct tm *)malloc( sizeof(struct tm) );
  }
  */
  /*
    Read request from the client
  */  
  if ( Socket_StringRead( sockfd, request) ) {
    if ( debug ) {
      printf("  Info_SetParameter: Error reading request in Socket_StringRead\n");
      fflush(stdout);
    }
    return(-1);
  }

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
  */
  status = Socket_StringTrim(request);

  /*
    Read value from the client
  */  
  if ( Socket_StringRead( sockfd, value) ) {
    if ( debug ) {
      printf("  Info_SetParameter: Error reading value in Socket_StringRead\n");
      fflush(stdout);
    }
    return(-1);
  }

  /*
    Trim control characters from the end (\n, \r, \t etc)
  */
  status = Socket_StringTrim(value);

  /*
    Get the value of the requested Info Parameter
  */
  status = Info_FindParameter( List, list_length, request, &position, debug);
  if ( status ) {
    printf("  Info_SetParameter: Error, %s not found in parameter list\n", request);
    fflush(stdout);
    sprintf(reply, "Error: Parameter %s not found in parameter list\n", request);
  } else {
    if ( !strncmp( "s", List[position].type, strlen(List[position].type) ) ) {
      strcpy( List[position].s_value, value);
      sprintf(reply, "%s %s\n", List[position].name, List[position].s_value);
    } else {
      printf("  Info_SetParameter: Client are not allowed to set %s parameter\n", List[position].name);
      fflush(stdout);
      sprintf(reply, "Error: Clients are not allowed to set %s parameter\n", List[position].name);
    }
  }

  if ( debug ) {
    printf("  Info_SetParameter: Set %s", reply);
    fflush(stdout);
  }

  if ( !strncmp( "Seeing_Value", List[position].name, strlen(List[position].name) )) {

    /*
      Save seeing value in seeing array
    */
    Seeing->value[Seeing->position] = atof(value);

    /*
      Save time that the seeing value was received
    */
    time ( &tp);
    strftime( time_string, MAX_TIME_LENGTH, "%I:%M:%S_%d%b%Y", localtime_r(&tp,&tms));
    strncpy( Seeing->time[Seeing->position], time_string, strlen(time_string));

    if ( debug ) {
      printf("  Seeing Array: %s %f\n", Seeing->time[Seeing->position], Seeing->value[Seeing->position]);
      fflush(stdout);
    }

    /*
      Increment pointers
    */
    Seeing->position++;
    Seeing->total_number++;
    if ( Seeing->position >= SEEING_VALUES ) {
      Seeing->position = 0;
    }

  } else if ( !strncmp( "Strehl_Value", List[position].name, strlen(List[position].name) )) {

    /*
      Save strehl value in strehl array
    */
    Strehl->value[Strehl->position] = atof(value);

    /*
      Save time that the strehl value was received
    */
    time ( &tp);
    strftime( time_string, MAX_TIME_LENGTH, "%I:%M:%S_%d%b%Y", localtime_r(&tp,&tms));
    strncpy( Strehl->time[Strehl->position], time_string, strlen(time_string));

    if ( debug ) {
      printf("  Strehl Array: %s %f\n", Strehl->time[Strehl->position], Strehl->value[Strehl->position]);
      fflush(stdout);
    }

    /*
      Increment pointers
    */
    Strehl->position++;
    Strehl->total_number++;
    if ( Strehl->position >= SEEING_VALUES ) {
      Strehl->position = 0;
    }

  }

  /*
    Return an Info Parameter
  */
  /*
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    printf("  Info_SetParameter: Error in Server_StringWrite sending number of bytes\n");
    fflush(stdout);
  }
  */

  return(0);
}
