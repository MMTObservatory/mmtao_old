/************************************************************************

  Server_Comm.c
  --------------

    Server_Read.c
    Server_Write.c

  Written  23mar05  DLM  Grouped all the Read and Write routines together

************************************************************************/

#include "PCR_Wish.h"
#include "Client.h"
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
    if (nread < 0){
      perror( "  Server_Read");
      return(nread);
    }
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
    return(-1);
  }

  return(0);
}

/************************************************************************

  Server_Flush.c

  Read the given socket util there are no data are available

  Written  9sep06  DLM  Started with Socket_Flush.c

************************************************************************/

int Server_Flush ( Server_Info *Info )
{
  struct timeval delay;
  fd_set mySet;
  char buff[IO_LENGTH];
  int n;

  /*
    Set time to wait to 0
  */
  FD_ZERO (&mySet);
  FD_SET ( Info->fd, &mySet);
  delay.tv_sec  = 0;
  delay.tv_usec = 100000;

  /*
    Read until socket is empty
  */
  do {
    n =0;
    if ( select (FD_SETSIZE, &mySet, NULL, NULL, &delay) ) {
      n = read ( Info->fd, buff, IO_LENGTH);
      //printf("flush %d >%s<\n", n, buff);
    }
  } while ( n > 0 );

  return(0);
}
