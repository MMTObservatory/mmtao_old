/*===========================================================================*
**
**  Server_Open.c
**
**  Open a socket connection.  This is a generic routine to open
**    a socket connection to the requested host a port (included
**    in the Info structure.
**
**  Returns :  0 success
**            -1 error (local problem)
**            -2 error connecting to server (maybe server problem)
**
**  Written  28dec04  DLM  Started with FSM_Init.  Made this a generic
**                           open socket routine.  Used the method found
**                           on the web  at 
**      http://www.linuxsa.org.au/pipermail/linuxsa/1998-September/002571.html
**                           written by Mark Pulford at kyne.com.au
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "DClient.h"

/*=========================================
 * function: Server_Open
 *=========================================*/

int Server_Open (Server_Info *Info, int debug)
{
  int sockfd = 0;
  struct hostent *serv_ent;
  struct sockaddr_in  serv_addr;
  extern int h_errno;
  int status;

  /*
    Variables for setting a timeout for the connection to
      a host
  */
  struct timeval delay;
  int saveflags;
  fd_set mySet;
  uint len;
  int ret;

  /*
    Look up the server name.  We need the "dot" notation.
  */
  h_errno = 0;
  serv_ent = gethostbyname (Info->address);
  /*
  if ( h_errno ) {
    printf("  Server_Open: Error looking up hostname %s\n", Info->address);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }
  */
  /*
    Fill in the serv_addr structure
  */
  bzero (&serv_addr, sizeof (serv_addr));
  bcopy (serv_ent->h_addr, (char *)(& serv_addr.sin_addr), serv_ent->h_length);
  serv_addr.sin_port   = htons (Info->port);
  serv_addr.sin_family = AF_INET;

  if ( debug ) {
    printf("  Server_Open: Opening %s:%s socket: %s, %d\n",
	   Info->name, Info->address, inet_ntoa(serv_addr.sin_addr), Info->port);
  }

  /*
    Check if there is an old socket connection.  If so, close it.
  */
  if ( Info->connected || Info->fd != 0 ) {
    if ( debug ) printf("  Server_Open: Closing old socket to %s\n",
	   Info->name);
    close(Info->fd);
    Info->connected = 0;
    Info->error = 1;
    Info->fd = 0;
    usleep(500000);
  }

  /*
    Open a TCP socket (an Internet stream socket)
  */
  sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("  Server_Open: Can't open stream socket to %s\n", Info->name);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }

  /*
    Set sockfd as non-blocking.  In this way we can see if the motor
      host (ao-topbox.mmto.arizona.edu) connects immediately.  If
      not, the host is most likely not available.  28dec04 DLM

    First, save the current flags for sockfd
  */
  saveflags=fcntl(sockfd,F_GETFL,0);
  if(saveflags<0) {
    printf("  Server_Open: Error saving original configuration of sockfd\n");
    return(-1);
  }

  /*
    Now set this socket as non-blocking
  */
  if(fcntl(sockfd,F_SETFL,saveflags|O_NONBLOCK)<0) {
    printf("  Server_Open: Error setting sockfd to non-blocking\n");
    return(-1);
  }

  /*
    Connect to the server.  This will return immediately.
  */
  status = connect (sockfd, (struct sockaddr *)(& serv_addr), sizeof (serv_addr));

  /*
    Restore original flags to sockfd
  */
  if(fcntl(sockfd,F_SETFL,saveflags)<0) {
    printf("  Server_Open: Error restoring original configuration of sockfd\n");
    return(-1);
  }

  /*
    If connection was not made then use select to wait until connect
      is made.
  */
  FD_ZERO(&mySet);
  FD_SET(sockfd,&mySet);
  delay.tv_sec  = SERVER_DELAY_S;
  delay.tv_usec = SERVER_DELAY_U;

  status=select( FD_SETSIZE, NULL, &mySet, NULL, &delay);
  if(status<0) {
    if ( debug ) printf("  Server_Open: Can't connect to %s server\n", Info->name);
    Info->connected = 0;
    Info->error = 1;
    return(-12);
  }

  /*
    Get the connect return code and check and see if the connection
      was properly established (ret = 0)
  */
  len=sizeof(ret);
  status=getsockopt( sockfd, SOL_SOCKET, SO_ERROR, &ret, &len);
  if(status<0) {
    if ( debug ) printf("  Server_Open: Error getting connection return code with getsockopt\n");
    return(-12);
  }

  if ( ret ) {
    if ( debug ) printf("  Server_Open: Could not connect to %s\n", Info->name);
    return(-12);
  }

  /*
    Fill in the Info structure
  */

  Info->fd = sockfd;
  Info->connected = 1;
  Info->error = 0;

  return(0);
}
