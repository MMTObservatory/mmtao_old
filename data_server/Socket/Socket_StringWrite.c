/*=======================================================================
 * Socket_StringWrite.c
 *=======================================================================

  This routine sends data to the given open socket

  Written  24may05  DLM Started with DServ_PassData.c

*/

#include "PCR.h"
#include "Socket.h"

/*======================================================================
 * Write a string via a socket to a client.
 *=======================================================================*/
int Socket_StringWrite( register int fd, register char *ptr, register int nbytes)
{
  int nleft, nwritten;

  nleft = nbytes;
  while (nleft > 0) {
    nwritten = write(fd, ptr, nleft);
    if (nwritten <= 0) {
      return(nwritten);
    }
    nleft -= nwritten;
    ptr += nwritten;
  } 

  return(nbytes - nleft);
}
