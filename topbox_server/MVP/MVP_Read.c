/*===========================================================================*
**
**  MVP_Read.c
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

/*=========================================
 * function: MVP_Read
 *=========================================*/

int MVP_Read( Socket_Info *Socket, char *reply, int debug)
{
  struct timeval delay;
  fd_set mySet;
  char buff[STRING_LENGTH];
  int n, i, j;
  int debug_local = 0;

  /*
    Read data from the motor (open socket => Socket->sockfd)
      => select is used to wait for the requested data to become
         available.
      => if nothing to read, return(1)
  */
  FD_ZERO (&mySet);
  FD_SET (Socket->sockfd, &mySet);
  delay.tv_sec  = MVP_DELAY_S;
  delay.tv_usec = MVP_DELAY_U;

  /*
    If there was nothing to read from sock_fd, return
  */
  if (!select (FD_SETSIZE, &mySet, NULL, NULL, &delay) ) {
    if ( debug ) {
      printf("  MVP_Read: Read timed out in %d miliseconds\n", MVP_DELAY_U);
      fflush(stdout);
    }
    Socket->error = 1;
    return(1);
  }

  /*
    Read reply from sockfd
  */
  buff[0] = 0;
  n = read (Socket->sockfd, buff, STRING_LENGTH);

  /*
    If the read didn't work, the connection died
  */
  if (n <= 0) return(-1);

  /*
    The return string begins after the newline/linefeed ('\n' or char(13))
  */
  j = 0;
  for (j = 0; buff[j] != 10 && j < STRING_LENGTH - 1; j++);
    /*
 {
    printf("%d  %d  %c\n", j, buff[j], buff[j]);
  }
    */
  j++;

  /*
    The return string ends usually with a CR ('\r' or char(10))
      => Also check for a NL/LF ('\n' or char(13))
      => and NULL ('\0' or char(0)).
  */
  for (i = 0; (buff[j] != 0 &&  buff[j] != 10 &&  buff[j] != 13 && j < STRING_LENGTH - 1) ; i++, j++) {
    reply[i] = buff[j];
  }

  /*
    Add a NULL to the end of the returned string
  */
  reply[i] = 0;

  if ( debug_local || debug ) {
    printf("  MVP_Read: Read from %s: %s\n", Socket->name, reply);
  }

  return(0);
}
