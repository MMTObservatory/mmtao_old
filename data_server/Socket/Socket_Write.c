/************************************************************************

  Socket_Write.c

    Write the requested number of bytes to the given socket

  Written  12dec05  DLM  Started with Server_Com.c

************************************************************************/

#include <stdio.h>
#include "Socket.h"

/************************************************************************
 * Write the request number of byte to given socket
 ************************************************************************/
int Socket_Write( register int fd, register char *ptr, register long nbytes)
{
  long nleft, nwritten;

  nleft = nbytes;
  while (nleft > 0) {
    nwritten = write(fd, ptr, nleft);
    if (nwritten <= 0) {
      return(nwritten);
    }
    nleft -= nwritten;
    ptr += nwritten;
  } 

  return(nwritten-nbytes);
}
