/*
  PCR_Write.c

  This routine writes the requested number of bytes to a socket

  Written  29mar05  DLM  Started with Socket_Write.c

*/

#include "PCR_Wish.h"
#include "Client.h"
#include "Server.h"
#include "PCR.h"

/************************************************************************
 * Write the request number of byte to given socket
 ************************************************************************/
int PCR_Write( register int fd, register char *ptr, register long nbytes)
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
