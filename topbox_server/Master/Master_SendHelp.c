/*
  Master_SendHelp.c

  This routine send the client a help list with possible command for this
    server.

  Written  27jan07  DLM  Started with Info_SendHelp.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Master.h"

/*================================================================================*
 * 
 *================================================================================*/
int Master_SendHelp( int sockfd)
{
  int status;
  char reply[STRING_LENGTH];

  strcpy( reply, "Possible Topbox Server requests:\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  ?, h, help    Print out possible commands\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  'device_name'             Send a request to requested device (FSM, OAP, Stage)\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "    -> 'specific device'    Send a request to requested device (Tip, Tilt, F15, Alignment\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "        -> Connect          Connect to the requested device\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "        -> Home             Home the requested device\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "        -> MoveRel          Move the requested device a relative distance\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "        -> MoveAbs          Move the requested device an absolute distance\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "        -> Abort            Send an abort to the requested device\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "        -> Reset            Send a reset signal to the requested device\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  quit          Quit this connection\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  exit          Shutdown the entire Topbox_Server\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  return(0);
}
