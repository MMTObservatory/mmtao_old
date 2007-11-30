/*=======================================================================
 * Socket_StringLowerCase.c
 *=======================================================================

  This routine changes the argument string to all lower case letters and
    trims control characters from the end of the string

  Written  24may05  DLM

  Altered  19nov05  DLM  Changed from Server_Open.c

*/

#include "PCR.h"
#include "Socket.h"

/*================================================================================*
 * Change the passed string into all lower case, and trim control characters
 *================================================================================*/
int Socket_StringLowerCase( char *request)
{

  /* local variables */

  int i;

  /*
    Search for upper case letters and control characters and replace with lower
      case or \0
  */
  for ( i=0; i<strlen(request); i++ ) {
    
    /*
      Check to see if the current character is upper case (between 65 and 90).
        If so then add 32 to make it lower case.  All non-alphabetic characters
	are left unchanged (except for \n, \l, \f below)
     */
    if ( request[i] >= 65 && request[i] <= 90 ) {
      request[i] = request[i] + 32;
    }

    /* 
       Remove control characters from the end of string, if exists 
    */
    /* '\n' \x10 new line */
    if ( request[i] == 10 ) {
      request[i] = '\0';
      break;
    }
    /* '\f' \x12 formfeed */
    if ( request[i] == 12 ) {
      request[i] = '\0';
      break;
    }
    /* '\r' \x13 carriage return */
    if ( request[i] == 13 ) {
      request[i] = '\0';
      break;
    }
  }


  return(0);
}
