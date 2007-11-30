/*
  PCR_SendCmd_DM.c

  This routine request the PC-Reconstructor (PCR) to save DM data.
    When we have set Save_DM_On, the PC will save all the DM data
    (WFSC, Slopes, RTR, Cmd, Cur, Pos) in file name with the time
    and an increament, each with 4000 frames.  This continues until
    a Save_DM_Off is sent.

  Written  4sep06  DLM  Started with PCR_SendCmd.c

*/

#include "PCR.h"
#include "DServ.h"

/*================================================================================*
 * Request the DM server on the PCR to save data
 *================================================================================*/
int PCR_SendCmd_DM( DServ_Info *Info, int sockfd, int *debug )
{
  char request[STRING_LENGTH];

  /*
    Read an instruction from the client
  */
  if ( Socket_StringRead( sockfd, request) ) {
    printf("  PCR_SendCmd_DM: Error reading request in Socket_StringRead\n");
    fflush(stdout);
    return(-1);
  }

  /*
    Check that a request was sent
  */
  if ( strlen(request) <= 1 ) {
    printf("  DServ_Process: Warning: request of zero length recieved\n");
    fflush(stdout);
    return(-1);
  }

  if ( *debug ) {
    printf("  PCR_SendCmd_DM: Request = %s\n", request);
    fflush(stdout);
  }

  /*
    Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
    => Case is important to PCR
  */
  //    status = Socket_StringLowerCase(request);

  /*
    All commands require a connection to the PCR server, so every time
      PCR_SendCmd_DM is called, we will check to make sure we are connected, or
      will initiate this connection
  */
  if ( !Info->socket->connected ) {

    /*
      PCR_GetDM must already be connected to the DM data port.  If not,
        send back an error (-2).
    */
    printf("  PCR_SendCmd_DM: DServ needs to already be connected to send the Save_DM_* request\n");
    fflush(stdout);
    return(-2);

  }

  /*
    Send the request to the PCR Cmd server
  */
  strcat( request, "\n");
  if ( Socket_StringWrite( Info->socket->sockfd, request, strlen(request)) != strlen(request) ) {
    if ( *debug ) {
      printf("  PCR_SendCmd_DM: Error writing Save_DM_* in Socket_StringWrite\n");
      fflush(stdout);
    }
    return(-1);
  }

  return(-1);

}
