/*
  Info_SendHelp.c

  This routine send the client a help list with possible command for this
    server.

  Written  19nov06  DLM  Started with DServ_SendHelp.c

*/

#include "PCR.h"
#include "Info.h"

/*================================================================================*
 * 
 *================================================================================*/
int Info_SendHelp( int sockfd)
{
  int status;
  char reply[STRING_LENGTH];

  strcpy( reply, "Possible Info requests:\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  debug\\n\n    0/1\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  set\\n\n    'parameter'\\n\n    'value'\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  get\\n\n    'parameter'\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "      => returns 'parameter' 'value'\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  all\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "      => returns 'parameter' 'value'\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "      =>         'parameter' 'value'\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "      =>         ...\\n\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "      =>         EOF\\n\n");
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
