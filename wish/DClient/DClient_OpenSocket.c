/*
  DClient_OpenSocket.c

  This routine connects to the request host and port.  The file
    descripter is returned to the calling routine.

  Written  27aug03  DLM

*/

#include "PCR_Wish.h"
#include "DClient.h"

int DClient_OpenSocket( char *host_address, short host_port, char *msg)
{
  int sockfd;
  struct sockaddr_in serv_addr;
  struct hostent *serv_ent;
  extern int h_errno;

  /*
    Look up the server name.  We need the "dot" notation.
  */
  h_errno = 0;
  serv_ent = gethostbyname (host_address);
  if ( h_errno ) {
    sprintf(msg, "DClient_OpenSocket: Error looking up hostname %s\n", host_address);
    return(-1);
  }

  /*
    Fill in the serv_addr structure
  */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  bcopy (serv_ent->h_addr, (char *)(& serv_addr.sin_addr), serv_ent->h_length);
  serv_addr.sin_port = htons(host_port);
  serv_addr.sin_family = AF_INET;

  /*
    Create socket
  */
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    sprintf(msg, "DClient_OpenSocket: can't open stream socket");
    return(-2);
  }

  /*
    Connect this socket to the request host and port
  */
  if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    sprintf(msg, "DClient_OpenSocket: can't connect to server %s:%d", host_address, host_port);
    return(-3);
  }

  return(sockfd);
}
