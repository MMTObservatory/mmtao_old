/*=======================================================================
 * Socket_StringRead.c
 *=======================================================================

  This routine reads data from the given open socket

  Written  24may05  DLM Started with DServ_PassData.c

*/

#include "Topbox.h"
#include "Socket.h"

/*======================================================================
 * Read a string via a socket from a client
 *=======================================================================*/
int Socket_StringRead(int clientfd, char *buffer)
{
  int  n, i=0;
  char c;

  do {
    n = read (clientfd, &c, 1);
    buffer [i++] = c;
  } while (n>0  &&  c!='\n'  &&  i<STRING_LENGTH);

  if (n<=0  ||  i<=0) { 
    return(-1);
  }

  buffer [i - 1] = 0;

  return(0);
}
