/*===========================================================================*
**
**  Fwheel_Read.c
**
**  Utility routines to read from an Fwheel controller
**
**  Returns :  0 success
**            -1 error
**
**  Written  29aug07  tjt  Started with Stage_Read/Write.c
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Fwheel.h"

int
Fwheel_Read( Socket_Info *Socket, char *reply, int debug)
{
  struct timeval delay;
  fd_set mySet;
  int n;
  int debug_local = 0;
  int timeout = 200000;

  /*
    Read with timeout via select.
    right now wait 0.2 seconds, but
    XXX - who knows what we really want...
  */
  FD_ZERO (&mySet);
  FD_SET (Socket->sockfd, &mySet);
  delay.tv_sec  = 0;
  delay.tv_usec = timeout;

  /*
    If there was nothing to read from sock_fd, return
  */
  if (!select (FD_SETSIZE, &mySet, NULL, NULL, &delay) ) {
    if ( debug ) {
      printf("  Fwheel_Read: Read timed out in %d milliseconds\n", timeout);
      fflush(stdout);
    }
    Socket->error = 1;
    return(1);
  }

  reply[0] = '\0';
  n = read (Socket->sockfd, reply, STRING_LENGTH);
  if (n <= 0) return(-1);

  reply[n] = '\0';

  if ( debug_local ) {
    printf("  Fwheel_Read: Read from %s: %s\n", Socket->name, reply);
  }

  return 0;
}

int
Fwheel_Write( Socket_Info *Socket, char *request, int debug)
{
  char buf[FW_MAXLINE];
  int debug_local = 0;

  if ( debug_local ) {
    printf("  Fwheel_Write: Writing \"%s\" to %s\n", request, Socket->name);
  }

  snprintf ( buf, FW_MAXLINE, "%s\r", request );

  if ( Socket_StringWrite( Socket->sockfd, buf, strlen(buf)) != strlen(buf) ) {
    if ( debug ) {
      printf("  Fwheel_Write: Error writing \"%s\" request to %s\n", request, Socket->name);
      fflush(stdout);
    }
    return -1;
  }

  return 0;
}

int
Fwheel_Send ( Socket_Info *Socket, char *msg, int debug )
{
  char buf[FW_MAXLINE];
  int rv;

  snprintf ( buf, FW_MAXLINE, "%s", msg );

  rv = Fwheel_Write ( Socket, buf, debug );
  return rv;
}

int
Fwheel_Step ( Socket_Info *Socket, int debug )
{
    if ( Fwheel_Send ( Socket, FW_STEPA, debug ) )
    	return -1;

    usleep ( FW_PULSE_DELAY );

    if ( Fwheel_Send ( Socket, FW_STEPB, debug ) )
    	return -1;

    return 0;
}

/* THE END */
