/*===========================================================================*
**
**  Stage_Read.c
**
**  Utility routines to read from an Stage device controller
**
**  Returns :  0 success
**            -1 error
**
**  Modified  24aug07  tjt  Started with MVP_Read.c
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Stage.h"

/*=========================================
 * function: Stage_Read
 *=========================================*/

int Stage_Read( Socket_Info *Socket, char *reply, int debug)
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
  delay.tv_sec  = STAGE_DELAY_S;
  delay.tv_usec = STAGE_DELAY_U;

  /*
    If there was nothing to read from sock_fd, return
  */
  if (!select (FD_SETSIZE, &mySet, NULL, NULL, &delay) ) {
    if ( debug ) {
      printf("  Stage_Read: Read timed out in %d miliseconds\n", STAGE_DELAY_U);
      fflush(stdout);
    }
    Socket->error = 1;
    return(1);
  }

  /*
    Read reply from sockfd
  */
  buff[0] = '\0';
  n = read (Socket->sockfd, buff, STRING_LENGTH);

  /*
    If the read didn't work, the connection died
  */
  if (n <= 0) return(-1);

  /* The Stage devices echo commands, so we need to skip all the
   * reply up to the newline (which is just the echo), then the next
   * line is what we really want.  We are relying on delays for both
   * the echo and response to be received, then get them both with
   * the single read above.  This could probably be changed to use
   * a pair of Socket_Read_string type calls, the first for the echo,
   * and the second for the response, and the delays eliminated.
  */

  j = 0;
  for (j = 0; (buff [j] != '\n' && j < STRING_LENGTH - 1) ; j++);
  j++;

  /*
    The return string ends usually at linefeed character ('\r' or char(10))
      => Also check for a carriage return ('\n' or char(13))
      => and NULL (char(0)).
  */
  for (i = 0; (buff[j] != '\0' &&  buff[j] != 12 && buff[j] != '\n' &&  buff[j] != '\r' && j < STRING_LENGTH - 1) ; i++, j++) {
    reply[i] = buff[j];
  }

  /*
    Add a NULL to the end of the returned string
  */
  reply[i] = '\0';

  if ( debug_local || debug ) {
    printf("  Stage_Read: Read from %s: %s\n", Socket->name, reply);
  }

  /* tjt - to keep with prior Stage_Read() */
  //  Socket_Flush ( Socket->sockfd);

  return(0);
}

/* THE END */
