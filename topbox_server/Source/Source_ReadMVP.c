/*===========================================================================*
**
**  Source_ReadMVP.c
**
**  Utility routines to read from an MVP device controller
**
**  Returns :  0 success
**            -1 error
**
**  Written  14aug07  DLM  Started with FSM_Comm from wish.
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "MVP.h"
#include "Source.h"

/*=========================================
 * function: Source_ReadMVP
 *=========================================*/

int Source_ReadMVP( int sockfd, Socket_Info *Socket, char *reply)
{
  struct timeval delay;
  fd_set mySet;
  char buff[STRING_LENGTH];
  int n, i, j;
  int local_debug = 0;

  /*
    Read data from the motor (open socket => sockfd)
      => select is used to wait for the requested data to become
         available.
      => if nothing to read, return(1)
  */
  FD_ZERO (&mySet);
  FD_SET (sockfd, &mySet);
  delay.tv_sec  = MVP_DELAY_S;
  delay.tv_usec = MVP_DELAY_U;

  /*
    If there was nothing to read from sock_fd, return
  */
  if (!select (FD_SETSIZE, &mySet, NULL, NULL, &delay) ) {
    if ( local_debug ) {
      printf("  Source_ReadMVP: Read timed out in %d miliseconds", MVP_DELAY_U);
      fflush(stdout);
    }
    return(1);
  }

  /*
    Read reply from sockfd
  */
  buff[0] = 0;
  n = read (sockfd, buff, STRING_LENGTH);

  /*
    If the read didn't work, the connection died
  */
  if (n <= 0) return(-1);

  /*
    The return string ends usually at linefeed character ('\r' or char(10))
      => Also check for a carriage return ('\n' or char(13))
      => and NULL (char(0)).
  */
  j = 0;
  for (i = 0; (buff[j] != 0 &&  buff[j] != 10 &&  buff[j] != 13) ; i++, j++) {
    reply[i] = buff[j];
  }
  /*
    Add a NULL to the end of the returned string
  */
  reply[i] = 0;

  if ( local_debug ) {
    printf("  Source_ReadMVP: Read from %s: %s\n", Socket->name, reply);
  }

  return(0);
}
