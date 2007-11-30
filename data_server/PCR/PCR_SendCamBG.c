/*
  PCR_SendCamBG.c

  This routine connects to the PC-Reconstructor (PCR) and sends
    a unsigned int array[576], in binary format, to the PCR.

  Written  22nov06  DLM  Started with PCR_SendCmd.c

*/

#include "PCR.h"
#include "DServ.h"

/*================================================================================*
 * Start server to manage data servers
 *================================================================================*/
int PCR_SendCamBG( DServ_Info *Info, int sockfd, int *debug )
{
  int status;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  char Reply[STRING_LENGTH];
  char *values;
  ushort Bg[576];
  int i, length;

  /*
    Variables needed to use the select commands to watch for data on a port
  */
  fd_set rfds;
  struct timeval tv;

  /*
    All commands require a connection to the PCR server, so every time
      PCR_SendCamBG is called, we will check to make sure we are connected, or
      will initiate this connection
  */
  if ( !Info->socket->connected ) {

    /*
      Open a socket to the PCR server
    */
    status =  Socket_OpenClient ( (Socket_Info *)Info->socket, *debug);
    if ( status ) {
      printf("  PCR_SendCamBG: Error in Socket_OpenClient\n");
      fflush(stdout);
      return(-1);
    }
  }

  /*
    Put fake values in to the Bg vector
  */
  for ( i=0; i<576; i++) {
    Bg[i]=htons(50);
  }

  /*
    Send the request to the PCR Cmd server
  */
  strcpy( request, "cam_bg_set\n");
  if ( Socket_StringWrite( Info->socket->sockfd, request, strlen(request)) != strlen(request) ) {
    if ( *debug ) {
      printf("  PCR_SendCamBG: Error writing cam_offset_set in Socket_StringWrite\n");
      fflush(stdout);
    }
    return(-1);
  }

  /*
    Send the Bg array to the PCR
  */
  values = (char *)Bg;
  length = 576*sizeof(ushort);
  if ( Socket_StringWrite( Info->socket->sockfd, values, length) != length ) {
    if ( *debug ) {
      printf("  PCR_SendCamBG: Error writing Camera Offset values in Socket_StringWrite\n");
      fflush(stdout);
    }
    return(-1);
  }

  /*
    Watch to see when there is data to be read on sockfd
  */
  FD_ZERO(&rfds);
  FD_SET(Info->socket->sockfd, &rfds);
  /* Wait up to one seconds. */
  tv.tv_sec = 1;
  tv.tv_usec = 0;
 
  status = select( (Info->socket->sockfd)+1, &rfds, NULL, NULL, &tv);

  if ( !status ) {

    /*
      If we do not receive a reply.  For now we will ignore this error,
        assuming that the PCR is not yet set up to send back replies
        **Must Fix  20nov06 DLM
    */
    printf("  PCR_SendCamBG: Reply not received\n");
    fflush(stdout);
    return(0);

  } else {

    /*
      Read an instruction from the client
    */
    if ( Socket_StringRead( Info->socket->sockfd, reply) ) {
      printf("  PCR_SendCamBG: Error reading reply in Socket_StringRead\n");
      fflush(stdout);
      return(-1);
    }

    /*
      Check that a request was sent
    */
    if ( strlen(reply) <= 1 ) {
      printf("  DServ_Process: Warning: reply of zero length recieved\n");
      fflush(stdout);
      return(-1);
    }

    if ( *debug ) {
      printf("  PCR_SendCamBG:   Reply = %s\n", reply);
      fflush(stdout);
    }

    /*
      Set request and reply to all lower case and trim control characters from 
        the end (\n, \r, \t etc)
    */
    status = Socket_StringLowerCase(request);
    strcpy( Reply, reply);
    status = Socket_StringLowerCase(reply);
    /*
    if ( !strncmp (request, "", strlen(request) ) ) { 

      if ( !strncmp (reply, "ok", strlen("ok") ) ) {

        printf("  PCR_SendCamBG: OK\n");

      } else if ( !strncmp (reply, "error", strlen("error") ) ) {

        printf("  PCR_SendCamBG: Error\n");

      } else {

        printf("  PCR_SendCamBG: Unknown reply from PCR: %s\n", Reply);
        return(-1);

      }

    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else if ( !strncmp (request, "", strlen(request) ) ) { 
    } else {
    
    }
    */
  }

  return(0);

}
