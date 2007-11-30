/*===========================================================================*
**
**  Smart_IO.c
**
**  Utility routines to read from an Smart controller
**
**  Returns :  0 success
**            -1 error
**
**  Written  9sep07  tjt  Started with Stage_Read/Write.c
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Status.h"
#include "Smart.h"

static void Smart_Flush_common ( Socket_Info *, int, int );

#define BROADCAST_ID	0x80
#define FOCUS_ID	0x81
#define ROTATION_ID	0x82

/* Internal access, no locks */
static int
Smart_Write ( Socket_Info *Socket, int id, char *request, int debug)
{
  char buf[STRING_LENGTH];
  int nbuf;

  if ( debug )
    printf("  Smart_Write: Writing \"%s\" to %s\n", request, Socket->name);

  snprintf ( buf, STRING_LENGTH, "%c%s\n", id, request );
  nbuf = strlen ( buf );

  Smart_Flush_common ( Socket, debug, 0 );

  if ( Socket_StringWrite( Socket->sockfd, buf, nbuf ) != nbuf ) {
    if ( debug ) {
      printf("  Smart_Write: Error writing \"%s\" request to %s\n", request, Socket->name);
      fflush(stdout);
    }
    return -1;
  }

  return 0;
}

static void
Smart_Flush_common ( Socket_Info *Socket, int debug, int wait)
{
  struct timeval delay;
  fd_set mySet;
  int n;
  char buf[STRING_LENGTH];

  FD_ZERO (&mySet);
  FD_SET (Socket->sockfd, &mySet);
  delay.tv_sec  = wait;
  delay.tv_usec = 0;

  while ( select (FD_SETSIZE, &mySet, NULL, NULL, &delay ) ) {
    n = read (Socket->sockfd, buf, STRING_LENGTH);
    if ( debug ) {
      printf("  Smart_Flush: flushing %d characters\n", n );
      fflush(stdout);
    }
  }
}

/* external interface */
void
Smart_Flush ( Socket_Info *Socket, int debug )
{
    pthread_mutex_lock( &Socket->lock );
    Smart_Flush_common ( Socket, debug, 0 );
    pthread_mutex_unlock( &Socket->lock );
}

static void
Smart_FlushEncoder ( Socket_Info *Socket, int debug )
{
    Smart_Flush_common ( Socket, debug, 1 );
}

static void
Smart_fixbuf ( char *buf )
{
  char *p, *q;

  /* Skip the echo of what we sent (ends at newline) */
  for ( p=buf; *p && *p != '\n'; p++ )
  	;

  if ( *p == '\n' )
    p++;

  /* Now, find the end of the reply string
   * slide the string left in the buffer.
   */
  q = buf;
  while ( *p && *p != '\n' && *p != '\r' && *p != '\f' )
	*q++ = *p++;

  *q = '\0';
}

/* Internal access, no locks */
static int
Smart_Read( Socket_Info *Socket, char *reply, int debug)
{
  struct timeval delay;
  fd_set mySet;
  int n;
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
      printf("  Smart_Read: Read timed out in %d milliseconds\n", timeout);
      fflush(stdout);
    }
    Socket->error = 1;
    return 1;
  }

  reply[0] = '\0';
  n = read (Socket->sockfd, reply, STRING_LENGTH);

  if (n <= 0)
      return -1;

  reply[n] = '\0';

  Smart_fixbuf ( reply );
  Smart_Flush_common ( Socket, debug, 0 );

  if ( debug )
    printf("  Smart_Read: Read from %s: %s\n", Socket->name, reply);

  return 0;
}

/* The Smart Encoder has its own port and socket.
 * It just chatters away incessantly spitting out
 * values at some interval, you never know when you
 * hook up if you will be in midstream, so the first
 * "string" read ought to be just discarded.
 */
int
Smart_ReadEncoder ( int *position, int debug )
{
    char reply[STRING_LENGTH];
    struct timeval delay;
    fd_set mySet;
    int n;
    char *p;
    int debug_local = 0;
    int status;

    Socket_Info Encoder_Socket;
    Socket_Info *Socket;

    Socket = &Encoder_Socket;
    status = Socket_Setup( Socket, "Encoder", debug);
    if ( status ) {
      printf("  Smart ReadEncoder: Error in Socket_Setup for Encoder\n");
      fflush(stdout);
      return -1;
    }

    status =  Socket_OpenClient ( Socket, debug );
    if ( status ) {
      printf("  Smart_ReadEncoder: Error connecting to the %s, %s:%d in Socket_OpenClient\n",
	     Socket->name, Socket->address, Socket->port);
      fflush(stdout);
    }

    /* sleep(1); */

    /* Two of these 1 second flushes */
    Smart_FlushEncoder ( Socket, debug );
    Smart_FlushEncoder ( Socket, debug );

    /* a 20 second timeout
     * (this thing talks about once every 5 seconds)
     * we get something like O=582936
     */
    FD_ZERO (&mySet);
    FD_SET (Socket->sockfd, &mySet);
    delay.tv_sec  = 20;
    delay.tv_usec = 0;

    if ( !select (FD_SETSIZE, &mySet, NULL, NULL, &delay ) ) {
      if ( debug ) {
        printf("  Smart_ReadEncoder: Read timed out after 20 seconds !!\n" );
        fflush(stdout);
      }
      Socket->error = 1;
      (void) Socket_Close( Socket, debug_local );
      return -1;
    }

    reply[0] = '\0';
    n = read (Socket->sockfd, reply, STRING_LENGTH);

    if (n <= 0) {
      (void) Socket_Close( Socket, debug_local );
      return -1;
    }

    reply[n] = '\0';

    if ( debug_local )
      printf("  Smart_ReadEncoder: Read from %s: %s\n", Socket->name, reply);

    (void) Socket_Close( Socket, debug_local );

    for ( p=reply; *p && *p != '='; p++ )
	;
    if ( *p == '=' )
	p++;

    *position = atol(p) - ROTATION_ABSOLUTE_ZERO;

    return 0;
}

/* internal write, followed by read */
static int
Smart_Cmd_int ( Socket_Info *Socket, int id, char *cmd, char *reply, int debug )
{
    int rv;
    rv = Smart_Write ( Socket, id, cmd, debug );
    if ( rv )
	return -1;

    rv = Smart_Read ( Socket, reply, debug );
    if ( rv )
	return -1;

    return 0;
}

/* Many interactions with the smart motor controller do a write then a read. */
int
Smart_Cmd ( Socket_Info *Socket, Device_Data *Data, char *cmd, char *reply, int debug )
{
    int rv;
    int id;

    id = FOCUS_ID;
    if ( strcmp ( Data->name, "Rotation" ) == 0 )
      id = ROTATION_ID;

    pthread_mutex_lock( &Socket->lock );
    rv = Smart_Cmd_int ( Socket, id, cmd, reply, debug );
    if ( rv ) {
	(void) Socket_Close ( Socket, 0 );
	(void) Status_ZeroFlags ( Data );
	pthread_mutex_unlock( &Socket->lock );
	return -1;
    }

    pthread_mutex_unlock( &Socket->lock );
    return 0;
}

static int
Smart_Cmdw_int ( Socket_Info *Socket, Device_Data *Data, int id, char *cmd, int debug )
{
    int rv;

    rv = Smart_Write ( Socket, id, cmd, debug );
    if ( rv ) {
	(void) Socket_Close ( Socket, 0 );
	(void) Status_ZeroFlags ( Data );
	return -1;
    }

    return 0;
}

int
Smart_Cmdw ( Socket_Info *Socket, Device_Data *Data, char *cmd, int debug )
{
    int rv;
    int id;

    id = FOCUS_ID;
    if ( strcmp ( Data->name, "Rotation" ) == 0 )
      id = ROTATION_ID;

    pthread_mutex_lock( &Socket->lock );
    rv = Smart_Cmdw_int ( Socket, Data, id, cmd, debug );
    pthread_mutex_unlock( &Socket->lock );

    return rv;
}

/* The initialize sets the addresses for both devices on
 * the smart daisy chain.  Sending with a zero address
 * is a "broadcast".  This gets called when we establish
 * a socket connection the first time, and handles the
 * setup of both devices.
 * The entire sequence is done holding the lock.
 */
static int
Smart_Init_common ( Socket_Info *Socket, int debug)
{
    char reply[STRING_LENGTH];
    int id;

    /* Assign Addresses */
    if ( Smart_Write ( Socket, BROADCAST_ID, "ECHO_OFF", debug ) ) return -1;
    if ( Smart_Write ( Socket, BROADCAST_ID, "SADDR1", debug ) ) return -2;
    if ( Smart_Write ( Socket, FOCUS_ID, "ECHO", debug ) ) return -3;
    if ( Smart_Write ( Socket, FOCUS_ID, "SLEEP", debug ) ) return -4;
    if ( Smart_Write ( Socket, BROADCAST_ID, "SADDR2", debug ) ) return -5;
    if ( Smart_Write ( Socket, ROTATION_ID, "ECHO", debug ) ) return -6;
    if ( Smart_Cmd_int ( Socket, FOCUS_ID, "WAKE", reply, debug ) ) return -7;

    id = FOCUS_ID;
    if ( Smart_Cmd_int   ( Socket, id, "A=100", reply, debug ) ) return -8;
    if ( Smart_Cmd_int ( Socket, id, "V=100000", reply, debug ) ) return -9;

    if ( Smart_Write ( Socket, id, "LIMD", debug ) ) return -10;
    if ( Smart_Cmd_int ( Socket, id, "Rh", reply, debug ) ) return -11;

    id = ROTATION_ID;
    if ( Smart_Cmd_int   ( Socket, id, "A=100", reply, debug ) ) return -12;
    if ( Smart_Cmd_int ( Socket, id, "V=30000", reply, debug ) ) return -13;
    if ( Smart_Write ( Socket, id, "KP=300", debug ) ) return -14;
    if ( Smart_Write ( Socket, id, "F", debug ) ) return -15;

    if ( Smart_Write ( Socket, id, "LIMD", debug ) ) return -16;
    if ( Smart_Cmd_int ( Socket, id, "Rh", reply, debug ) ) return -17;

    return 0;
}

#define MAX_INIT_TRIES	13

int
Smart_Init ( Socket_Info *Socket, int debug)
{
    int rv;
    int tries;

    /* XXX */

    for ( tries = 0; tries < MAX_INIT_TRIES; ++tries ) {
	if ( ! Socket->connected ) {
	    printf("  Smart_Init: No connection\n" );
	    return -1;
	}

	pthread_mutex_lock( &Socket->lock );
	rv = Smart_Init_common ( Socket, debug );
	pthread_mutex_unlock( &Socket->lock );

	if ( rv == 0  ) {
	    if ( tries > 0 )
	    	printf ("Got it!\n");
	    return 0;
	}

        printf("  Smart_Init: Initialization fails, code %d (%s)\n", -rv, Socket->name);
        fflush(stdout);

	debug = 1;
	printf("  Try again (attempt %d)\n", tries+1 );

    }

    printf("  Smart_Init: Giving up\n" );
    return -1;
}

/* THE END */
