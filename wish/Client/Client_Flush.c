/*===========================================================================*
**
**  Client_Flush
**
**  Utility routines to read from a socket till it is empty
**
**  Returns :  0 success
**            -1 error
**
**  Written   2feb06  DLM  Started with FSM_Comm.
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "Client.h"

/*=========================================
 * Read from socket until empty
 *=========================================*/

int Client_Flush ( register int fd)
{
  struct timeval delay;
  fd_set mySet;
  char buff [IO_LENGTH];
  int n;

  /*
    Set time to wait to 0
  */
  FD_ZERO (&mySet);
  FD_SET (fd, &mySet);
  delay.tv_sec  = 0;
  delay.tv_usec = 100;

  /*
    Read until socket is empty
  */
  do {
    n =0;
    if ( select (FD_SETSIZE, &mySet, NULL, NULL, &delay) ) {
      n = read (fd, buff, IO_LENGTH);
    }
  } while ( n > 0 );

  return(0);
}
