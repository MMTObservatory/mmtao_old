/*
  ==========================
  Client_Read.c
  ==========================

  This routine Reads one line from the given socket.
 
  Written  17jun05  DLM  Started with TCS_Read.c

*/

#include "PCR_Wish.h"
#include "Client.h"

/*================================================================================
 * TCS_Write: Write requested number of byte to a socket
 *================================================================================*/
int Client_Write( register int fd, register char *ptr, register int nbytes)
{
  int nleft, nwritten;
  int status;

  status = Client_Flush ( fd);

  nleft = nbytes;
  while (nleft > 0) {
    nwritten = write(fd, ptr, nleft);
    if (nwritten <= 0)
      return(nwritten);	  /* error */

    nleft -= nwritten;
    ptr   += nwritten;
  }
  return(nleft);
}
