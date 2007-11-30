/*
  ==========================
  Client_Open.c
  ==========================

  Routine to open a client connection to the requested host.
    Socket information is returned in the Client_Info

  Written  16jun05  DLM  Started with DClient_Open.  Made this a generic
                           open socket routine.  Used the method found
                           on the web  at 
      http://www.linuxsa.org.au/pipermail/linuxsa/1998-September/002571.html
                           written by Mark Pulford at kyne.com.au

  Altered   9jul07  DLM  Added close(sockfd) to all case where an error occured
                           when making the socket connect.  By not closing the socket
                           all the available socket numbers (0-255) were being
                           used and the socke() function was returning an error.
                           This problem was found when the automatic connection to
                           the PCR_Server was installed (1 second repeat to try to connect).

*/

#include "PCR_Wish.h"
#include "Client.h"

/*================================================================================
 * Open a client connection to a server
 *================================================================================*/
int Client_Open( Client_Info *Info, int debug)
{
  /*
    Local variables
  */
  int sockfd = 0;
  struct sockaddr_in cli_addr;
  struct hostent *cli_ent;
  extern int h_errno;
  int status;

  /*
    Variables for setting a timeout for the connection to a host
  */
  struct timeval delay;
  int saveflags;
  fd_set mySet;
  int ret;
  socklen_t len;

  /*
    Look up the server name.  We need the "dot" notation.
  */
  h_errno = 0;
  cli_ent = gethostbyname (Info->address);
  /*
  if ( h_errno ) {
    printf(" Client_Open: Error looking up hostname >%s<\n", Info->address);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }
  */
  /*
    Fill in the cli_addr structure
  */
  bzero (&cli_addr, sizeof (cli_addr));
  bcopy (cli_ent->h_addr, (char *)(& cli_addr.sin_addr), cli_ent->h_length);
  cli_addr.sin_port   = htons (Info->port);
  cli_addr.sin_family = AF_INET;

  if ( debug ) {
    printf("  Client_Open: Opening %s socket: %s (%s), %d\n",
	   Info->name, Info->address, inet_ntoa(cli_addr.sin_addr), Info->port);
  }

  /*
    Check if there is an old socket connection.  If so, close it.
  */
  if ( Info->connected || Info->fd != 0 ) {
    if ( debug ) printf("Client_Open: Closing old socket to %s\n", Info->name);
    close(Info->fd);
    Info->connected = 0;
    Info->error = 1;
    Info->fd = 0;
    usleep(500000);
  }

  /*
    Open a TCP socket (an Internet stream socket)
  */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    if ( debug ) printf("  Client_Open: Can't open stream socket to %s\n", Info->name);
    close(sockfd);
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
    if ( debug ) printf("Client_Open: Error saving original configuration of sockfd\n");
    close(sockfd);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }

  /*
    Now set this socket as non-blocking
  */
  if(fcntl(sockfd,F_SETFL,saveflags|O_NONBLOCK)<0) {
    if ( debug ) printf("Client_Open: Error setting sockfd to non-blocking\n");
    close(sockfd);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }

  /*
    Connect to the server.  This will return immediately.
  */
  status = connect (sockfd, (struct sockaddr *)(& cli_addr), sizeof (cli_addr));

  /*
    Restore original flags to sockfd
  */
  if(fcntl(sockfd,F_SETFL,saveflags)<0) {
    if ( debug ) printf("Client_Open: Error restoring original configuration of sockfd\n");
    close(sockfd);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }

  /*
    If connection was not made then use select to wait until connect
      is made.  If select returns:
        -1 - error in select
         0 - connection was not successful
         1 - connection was successful and is ready
  */
  FD_ZERO(&mySet);
  FD_SET(sockfd,&mySet);
  delay.tv_sec  = CLIENT_DELAY_S;
  delay.tv_usec = CLIENT_DELAY_U;

  status=select( FD_SETSIZE, NULL, &mySet, NULL, &delay);
  if(status<=0) {
    if ( debug ) printf("  Client_Open: Error opening %s socket: %s (%s), %d\n",
	   Info->name, Info->address, inet_ntoa(cli_addr.sin_addr), Info->port);
    printf("               Is host running and network available?\n");
    close(sockfd);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }

  /*
    Get the connect return code and check and see if the connection
      was properly established (ret = 0)
  */
  len=sizeof(ret);
  status=getsockopt( sockfd, SOL_SOCKET, SO_ERROR, &ret, &len);
  if(status<0) {
    if ( debug ) printf("Client_Open: Error getting connection return code with getsockopt\n");
    close(sockfd);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }

  if ( ret ) {
    if ( debug ) printf("Client_Open: Could not connect to %s\n", Info->name);
    close(sockfd);
    Info->connected = 0;
    Info->error = 1;
    return(-1);
  }

  /*
    Fill in the Info structure
  */

  Info->fd = sockfd;
  Info->connected = 1;
  Info->error = 0;

  return(0);
}
