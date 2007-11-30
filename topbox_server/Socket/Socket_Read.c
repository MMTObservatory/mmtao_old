/************************************************************************

  Socket_Read.c

    Read the requested number of bytes from the given socket

  Written  12dec05  DLM  Started with Server_Com.c

************************************************************************/

#include "Topbox.h"
#include "Socket.h"

/************************************************************************
 * Read the request number of byte from given socket
 ************************************************************************/
int Socket_Read( int sockfd, char *buffer, int nBytes)
{
  int nleft, nread, total_read;
  char *ptr;

  ptr = buffer;

  nleft = nBytes;
  total_read = 0;

  while (nleft > 0) {
    nread = read(sockfd, ptr, nleft);
    if (nread < 0) {
      return(nread);
    } else if (nread == 0) {
      break;
    }
    nleft -= nread;
    total_read += nread;
    ptr += nread;
  }
  return( nBytes - total_read );
}
