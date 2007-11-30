/*
  ==========================
  Client_Read.c
  ==========================

  This routine reads the requested number of bytes from a socket
 
  Written  17jun05  DLM  Started with TCS_Read.c

*/

#include "PCR_Wish.h"
#include "Client.h"

/*================================================================================
 * Client_Read: Read requested number of bytes from a socket
 *================================================================================*/
int Client_Read( int fd, char *reply, int nBytes )
{
  int nleft, nread;
  char *ptr;

  ptr = reply;
  
  nleft = nBytes;
  while (nleft > 0) {
    nread = read(fd, ptr, nleft);
   if (nread < 0)return(nread);
    else if (nread == 0)break;

    nleft -= nread;
    ptr += nread;
  }

  return( nBytes - nread );

}
