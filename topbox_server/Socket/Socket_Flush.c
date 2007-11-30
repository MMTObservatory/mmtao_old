/************************************************************************

  Socket_Flush.c

  Read the given socket util there are no data are available

  Written  24aug06  DLM  Started with FSM_Flush.c

************************************************************************/

#include "Topbox.h"
#include "Socket.h"

/************************************************************************
 * Read until the socket is empty
 ************************************************************************/
int Socket_Flush ( int sockfd)
{
  struct timeval delay;
  fd_set mySet;
  char buff [SOCKET_MAXLINE];
  int n;

  /*
    Set time to wait to 0
  */
  FD_ZERO (&mySet);
  FD_SET (sockfd, &mySet);
  delay.tv_sec  = 0;
  delay.tv_usec = 1000;

  /*
    Read until socket is empty
  */
  do {
    n =0;
    if ( select (FD_SETSIZE, &mySet, NULL, NULL, &delay) ) {
      n = read (sockfd, buff, SOCKET_MAXLINE);
      //            printf("flush %d >%s<\n", n, buff);
      //            fflush(stdout);
    }
  } while ( n > 0 );

  return(0);
}
