/*
  DServ_SendHelp.c

  This routine send the client a help list with possible command for this
    server.

  Written  16jul06  DLM

*/

#include "PCR.h"
#include "DServ.h"

/*================================================================================*
 * 
 *================================================================================*/
int DServ_SendHelp( int sockfd)
{
  int status;
  char reply[STRING_LENGTH];

  strcpy( reply, "Possible DServ requests:\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  size\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "      => returns long (nBytes)\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  single\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "      => returns long (nBytes)\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "      =>         long (frame number)\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "      =>         many longs (values)\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  stream\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "      => returns continous single (above)\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  ?\\n -or- h\\n -or- help\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "      => returns this help menu\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  quit\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "      => closes this client connection (server continues running)\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  return(0);
}
