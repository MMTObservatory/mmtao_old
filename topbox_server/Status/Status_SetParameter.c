/*=======================================================================
 * Status_SetParameter.c
 *=======================================================================

  This routine send a string with the Status Parameter name and value
    separated by a space and delimited by a newline (\n) to the
    requesting client.

  Client are only allowed to set character parameters ("s") so when the
    the position of the Status parameter is found we need to check the
    it is a character parameter

  Written  17may05  DLM  Started with SInfo_FileNameSave.c

  Altered  19nov06  DLM  Modified to work with DServ for the PCR

*/

#include "PCR.h"
#include "Status.h"

/*================================================================================*
 * Return an Status Parameter and its value
 *================================================================================*/
int Status_SetParameter( int sockfd, Status_Entry *List, long list_length, int debug)
{

  /* local variables */

  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  char value[STRING_LENGTH];
  long position;
  int status;

  /* time variable */
  const time_t time_string[26];

  /*
    Read request from the client
  */  
  if ( Socket_StringRead( sockfd, request) ) {
    if ( debug ) printf("  Status_SetParameter: Error reading request in Socket_StringRead\n");
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
    if ( debug ) printf("  Status_SetParameter: Error reading value in Socket_StringRead\n");
    return(-1);
  }

  /*
    Trim control characters from the end (\n, \r, \t etc)
  */
  status = Socket_StringTrim(value);

  /*
    Get the value of the requested Status Parameter
  */
  status = Status_FindParameter( List, list_length, request, &position, debug);
  if ( status ) {
    if ( debug ) {
      printf("  Status_SetParameter: Error in Status_FindParamter\n");
    }
    return(-1);
  } else {
    if ( !strncmp( "s", List[position].type, strlen(List[position].type) ) ) {
      strcpy( List[position].s_value, value);
      sprintf(reply, "%s %s\n", List[position].name, List[position].s_value);
    } else {
      printf("  Status_SetParameter: Client are not allowed to set this parameter");
      return(-1);
    }
  }

  if ( debug ) {
    printf("  Status_SetParameter: Set %s", reply);
  }

  /*
    Return an Status Parameter
  */
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    printf("  Status_SetParameter: Error in Server_StringWrite sending number of bytes\n");
  }

  return(0);
}
