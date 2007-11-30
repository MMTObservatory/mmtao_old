/*=======================================================================
 * Socket_StringTrim.c
 *=======================================================================

  This routine trims \n, \l and \f from the end of the passed string.

  Written  23may05  DLM

*/

#include "PCR.h"
#include "Socket.h"

/*================================================================================*
 * Return a trimed string
 *================================================================================*/
int Socket_StringTrim( char *request)
{

  /* local variables */

  int i;

  /*
    Search for control characters.  If one is found, change it to \0 and return.
  */
  for ( i=0; i<strlen(request); i++ ) {
    
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


  return(-1);
}
