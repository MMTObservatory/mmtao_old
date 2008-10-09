/*===========================================================================*
**
**  Socket_OpenClient.c
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
**  Altered   9jul07  DLM  Added close(sockfd) to all case where an error occured
**                           when making the socket connect.  By not closing the socket
**                           all the available socket numbers (0-255) were being
**                           used and the socke() function was returning an error.
**                           This problem was found when the automatic connection to
**                           the PCR was installed (1 second repeat to try to connect).
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"

/*=========================================
 * function: Socket_OpenClient
 *=========================================*/

int Socket_OpenClient (Socket_Info *Info, int debug)
{
  int sockfd = -1;
  struct hostent *serv_ent;
  struct sockaddr_in  serv_addr;
  int status;

  /*
    Variables for setting a timeout for the connection to
      a host
  */
  struct timeval delay;
  int saveflags;
  fd_set mySet;
  int ret;
  socklen_t len;

  struct hostent host_entry;
  struct hostent *result;
  char hostbuf[500];
  int   h_err;

  /*
    Fill in the serv_addr structure
  */
  serv_ent = &host_entry;
  if (gethostbyname_r (Info->address,
    serv_ent, hostbuf, sizeof(hostbuf),
    &result, &h_err)) {
      Info->error = 1;
      return(-1);
  }
  if (h_err != 0 || result == NULL) {
      Info->error = 1;
      return(-1);
  }
  bzero (&serv_addr, sizeof (serv_addr));
  bcopy (serv_ent->h_addr, (char *)(& serv_addr.sin_addr), serv_ent->h_length);
  serv_addr.sin_port   = htons (Info->port);
  serv_addr.sin_family = AF_INET;

  if ( debug ) {
    printf("  Socket_OpenClient: Opening %s socket: %s, %d\n",
	   Info->name, Info->address, Info->port);
    fflush(stdout);
  }

  /*
    Check if there is an old socket connection.  If so, close it.
  */
  if ( Info->connected || Info->sockfd >= 0 ) {
    if ( debug ) {
      printf("  Socket_OpenClient: Closing old socket to %s\n",
	     Info->name);
      fflush(stdout);
    }
    close(Info->sockfd);
    Info->connected = 0;
    Info->error = 1;
    Info->sockfd = 0;
    usleep(500000);
  }

  /*
    Open a TCP socket (an Internet stream socket)
  */
  sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    if ( debug ) {
      printf("  Socket_OpenClient: Can't open stream socket to %s\n", Info->name);
      fflush(stdout);
    }
    close(sockfd);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }

  /*
    Set sockfd as non-blocking.  In this way we can see if the
      host (ao-topbox.mmto.arizona.edu) connects immediately.  If
      not, the host is most likely not available.  28dec04 DLM

    First, save the current flags for sockfd
  */
  saveflags=fcntl(sockfd,F_GETFL,0);
  if(saveflags<0) {
    if ( debug ) {
      printf("  Socket_OpenClient: Error saving original configuration of sockfd\n");
      fflush(stdout);
    }
    close(sockfd);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }

  /*
    Now set this socket as non-blocking
  */
  if(fcntl(sockfd,F_SETFL,saveflags|O_NONBLOCK)<0) {
    if ( debug ) {
      printf("  Socket_OpenClient: Error setting sockfd to non-blocking\n");
      fflush(stdout);
    }
    close(sockfd);
    Info->connected = 0;
    Info->error = 1;
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
    if ( debug ) {
      printf("  Socket_OpenClient: Error restoring original configuration of sockfd\n");
      fflush(stdout);
    }
    close(sockfd);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }

  /*
    If connection was not made then use select to wait until connect
      is made.
  */
  FD_ZERO(&mySet);
  FD_SET(sockfd,&mySet);
  delay.tv_sec  = SOCKET_DELAY_S;
  delay.tv_usec = SOCKET_DELAY_U;

  status=select( FD_SETSIZE, NULL, &mySet, NULL, &delay);
  if(status<0) {
    if ( debug ) {
      printf("  Socket_OpenClient: Can't connect to %s server\n", Info->name);
      fflush(stdout);
    }
    close(sockfd);
    Info->connected = 0;
    Info->error = 0;
    return(-1);
  }

  /*
    Get the connect return code and check and see if the connection
      was properly established (ret = 0)
  */
  len=sizeof(ret);
  status=getsockopt( sockfd, SOL_SOCKET, SO_ERROR, &ret, &len);
  if(status<0) {
    if ( debug ) {
      printf("  Socket_OpenClient: Error getting connection return code with getsockopt\n");
      fflush(stdout);
    }
    close(sockfd);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }

  if ( ret ) {
    if ( debug ) {
      printf("  Socket_OpenClient: Could not connect to %s\n", Info->name);
      fflush(stdout);
    }
    close(sockfd);
    Info->connected = 0;
    Info->error = 0;
    return(-1);
  }

  /*
    Pause for a bit till socket is ready for comminication
  */
  usleep(500000);

  /*
    Fill in the Info structure
  */

  Info->sockfd = sockfd;
  Info->connected = 1;
  Info->error = 0;

  return(0);
}
