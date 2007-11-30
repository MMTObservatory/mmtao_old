/*
  PCR_SendCmd.c

  This routine connects to the PC-Reconstructor (PCR), send a 
  request, and does not expect a reply

  Written  31jan07  DLM  Started with PCR_SendCmd.c

*/

#include "PCR.h"
#include "DServ.h"
#include "PCR_Cmd.h"

/*================================================================================*
 * Send a request to the PCR, with on reply expected
 *================================================================================*/
int PCR_SendCmd_NoReply( DServ_Info *Info, char *request, int sockfd, int *debug )
{
  int status;

  /*
    All commands require a connection to the PCR server, so every time
      PCR_SendCmd_NoReply is called, we will check to make sure we are connected, or
      will initiate this connection
  */
  if ( !Info->socket->connected ) {

    /*
      Open a socket to the PCR server
    */
    status =  Socket_OpenClient ( (Socket_Info *)Info->socket, *debug);
    if ( status ) {
      printf("  PCR_SendCmd_NoReply: Error in Socket_OpenClient\n");
      fflush(stdout);
      return(-1);
    }
  }

  /*
    Send the request to the PCR server
  */
  strcat( request, "\n");
  if ( Socket_StringWrite( Info->socket->sockfd, request, strlen(request)) != strlen(request) ) {
    if ( *debug ) {
      printf("  PCR_SendCmd_NoReply: Error writing start_WFSC in Socket_StringWrite\n");
      fflush(stdout);
    }
    return(-1);
  }

  return(0);

}
