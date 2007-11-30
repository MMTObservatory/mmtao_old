/*===========================================================================*
**
**  Socket_OpenServer.c
**
**  Open a socket connection.  This is a generic routine to open
**    a socket connection to the requested host and port (included
**    in the Info structure.
**
**  Returns :  0 success
**            -1 error
**
**  Written  28dec04  DLM  Started with FSM_Init.  Made this a generic
**                           open socket routine.  Used the method found
**                           on the web  at 
**      http://www.linuxsa.org.au/pipermail/linuxsa/1998-September/002571.html
**                           written by Mark Pulford at kyne.com.au
**
**  Altered  12oct05  DLM  Added usleep(500000) after socket is opened so the
**                           the socket is ready for communiction.
**
**===========================================================================*/

#include "PCR.h"
#include "Socket.h"

/*=========================================
 * function: Socket_OpenServer
 *=========================================*/

int Socket_OpenServer (Socket_Info *Info, int debug)
{
  int sockfd = 0;
  struct sockaddr_in  serv_addr;
  int x;

  /*
    Fill in the serv_addr structure
  */
  bzero( (char *) &serv_addr, sizeof (serv_addr));
  serv_addr.sin_family      = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port        = htons (Info->port);

  if ( debug ) {
    printf("  Socket_OpenServer: Opening %s socket on port %d\n",
	   Info->name, Info->port);
    fflush(stdout);
  }

  /*
    Open a TCP socket (an Internet stream socket)
  */
  sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("  Socket_OpenServer: Can't open stream socket to %s, port %d\n",
	   Info->name, Info->port);
    fflush(stdout);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }

  /*
    Allow socket to be reused after this process exits.  In this
      way we can stop and restart this server.
  */

  x = 1;
  setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR,
	      (char *) &x,  sizeof (x));
 
  /*
   * Bind our local address so that the client can send to us.
   */
  if( bind( sockfd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) {
    printf("  Socket_OpenServer: can't bind local address\n" );
    printf("    Another process must be using port %d\n", Info->port);
    fflush(stdout);
    return(-2);
  }

  /*
    Listen to this socket.  This function returns when a client
    has connected.  This is an example of a concurrent server.
  */
  listen( sockfd, 5 );

  /*
    Fill in the Info structure
  */

  Info->sockfd = sockfd;
  Info->connected = 1;
  Info->error = 0;

  return(0);
}
