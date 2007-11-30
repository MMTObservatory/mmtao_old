/************************************************************************

  Server_Comm.c
  --------------

    Server_Read.c
    Server_Write.c

  Written  23mar05  DLM  Grouped all the Read and Write routines together

************************************************************************/

#include "Server.h"

/************************************************************************

  Server_ReadChar.c

  Read the requested number of bytes from the given socket

  Written  26aug03  DLM

  Altered  23mar05  DLM  Changed from DClient_ReadChar.

                         We no longer send fd, string, and length, just
                           the structure Server_Info

************************************************************************/

int Server_Read( Server_Info *Info, int nBytes )
{
  int nleft, nread;
  //  char ptr[80];
  char *ptr;

  ptr = Info->in_line;

  nleft = nBytes;
  while (nleft > 0) {
    nread = read(Info->fd, ptr, nleft);
    if (nread < 0)return(nread);
    else if (nread == 0)break;

    nleft -= nread;
    ptr += nread;
  }
  return( nBytes - nread );
}

/************************************************************************
  Server_Write.c

  Send a request string to a Server.  The file descripter and request
    are in the Server_Info structure.

  Written  27aug03  DLM

  Altered  23mar05  DLM  Changed from DClient_WriteString.

                         We no longer send fd, string, and length, just
                           the structure Server_Info

************************************************************************/

int Server_Write( Server_Info *Info )
{

  int n, num;

  n = strlen(Info->out_line);

  num = write( Info->fd, Info->out_line, n);
  if (num != n) {
    printf("  Server_Write: write error on socket\n");
    fflush(stdout);
    return(-1);
  }

  return(0);
}
