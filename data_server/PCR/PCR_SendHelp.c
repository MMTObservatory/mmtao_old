/*
  PCR_SendHelp.c

  This routine send the client a help list with possible command for this
    server.

  Written  27jan07  DLM  Started with Info_SendHelp.c

*/

#include "PCR.h"
#include "DServ.h"
#include "PCR_Cmd.h"

/*================================================================================*
 * 
 *================================================================================*/
int PCR_SendHelp( int sockfd)
{
  int status;
  char reply[STRING_LENGTH];

  strcpy( reply, "Possible Info requests:\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  ?, h, help    Print out possible commands\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  cmd           Send a request to the Cmd port of the PCR\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  connect       Connect to either the WFSC or DM port on the PCR\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "    -> wfsc      Start thread to receive and store WFSC image from the PCR\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "    -> dm        Start thread to receive and store DM frames from the PCR\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  stop       Stop one of the PCR Clients\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "    -> wfsc      Stop the PCR WFSC Client\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "    -> dm        Stop the PCR DM Client\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  reset       Reset one of the PCR Clients\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "    -> wfsc      Reset the PCR WFSC Client\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "    -> dm        Reset the PCR DM Client\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  quit          Quit this connection\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  strcpy( reply, "  exit          Shutdown the entire PCR_Server\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  return(0);
}
