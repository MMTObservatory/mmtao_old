/*===========================================================================*
**
**  Power_IO.c
**
**  Utility routines to read and write the Power controller
** (the controller is an Ontrak ADR2205)
**
**
**  Written  29aug07  tjt  Started with Fwheel_IO.c
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Power.h"

static int
Power_Read( Socket_Info *Socket, char *reply, int debug)
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

  pthread_mutex_lock( &Socket->lock);

  /*
    If there was nothing to read from sock_fd, return
  */
  if (!select (FD_SETSIZE, &mySet, NULL, NULL, &delay) ) {
    if ( debug ) {
      printf("  Power_Read: Read timed out in %d milliseconds\n", timeout);
      fflush(stdout);
    }
    Socket->error = 1;
    pthread_mutex_unlock( &Socket->lock);
    return(1);
  }

  reply[0] = '\0';
  n = read (Socket->sockfd, reply, STRING_LENGTH);

  pthread_mutex_unlock( &Socket->lock);

  if (n <= 0) return(-1);

  reply[n] = '\0';

  if ( debug_local || debug ) {
    printf("  Power_Read: Read from %s: %s\n", Socket->name, reply);
    fflush(stdout);
  }

  return 0;
}

static int
Power_Write( Socket_Info *Socket, char *request, int debug)
{
  char buf[POWER_MAXLINE];
  int debug_local = 0;

  if ( debug_local || debug ) {
    printf("  Power_Write: Writing \"%s\" to %s\n", request, Socket->name);
    fflush(stdout);
  }

  snprintf ( buf, POWER_MAXLINE, "%s\r", request );

#ifdef notdef
  Socket_Flush ( Socket->sockfd);
#endif

  pthread_mutex_lock( &Socket->lock);

  if ( Socket_StringWrite( Socket->sockfd, buf, strlen(buf)) != strlen(buf) ) {
    if ( debug ) {
      printf("  Power_Write: Error writing \"%s\" request to %s\n", request, Socket->name);
      fflush(stdout);
    }
    pthread_mutex_unlock( &Socket->lock);
    return -1;
  }

  pthread_mutex_unlock( &Socket->lock);
  return 0;
}

/* The relay number here are 0-7 as sent to the controller */

int
Power_On ( Socket_Info *Socket, int num, int debug )
{
    char buf[8];

    if ( debug ) {
      printf ( "  Power on: %d\n", num );
      fflush(stdout);
    }

    sprintf ( buf, "SK%d", num );
    return Power_Write ( Socket, buf, debug );
}

int
Power_Off ( Socket_Info *Socket, int num, int debug )
{
    char buf[8];

    if ( debug ) {
      printf ( "  Power off: %d\n", num );
      fflush(stdout);
    }

    sprintf ( buf, "RK%d", num );
    return Power_Write ( Socket, buf, debug );
}

int
Power_Info ( Socket_Info *Socket, char *buf, int debug )
{
    if ( Power_Write ( Socket, "RPK", debug ) < 0 )
	return -1;

    return Power_Read ( Socket, buf, debug );
}

/* THE END */
