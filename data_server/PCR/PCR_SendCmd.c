/*
  PCR_SendCmd.c

  This routine connects to the PC-Reconstructor (PCR), request a
    WFSC frame data stream and puts these data in the the proper
    memory.

  Written  17jun06  DLM  Started with DServ_GetWFSC.c

  Altered  19nov06  DLM  Added reading a reply from the PCR

  Altered   2apr07  DLM  Added mutex lock and unlock to keep from having
                           collisions between different threads trying to
			   use the PCR_CMD port.
                         Removed sockfd from the client from the argument list.
*/

#include <pthread.h>

#include "PCR.h"
#include "DServ.h"

/*
  The mutex variable PCR_CMD_Lock is a global variable set in PCR_Server.c.  We
    must now make it visible in this routine.
*/
extern pthread_mutex_t PCR_CMD_Lock;

/*================================================================================*
 * Start server to manage data servers
 *================================================================================*/
int PCR_SendCmd( DServ_Info *Info, char *request, int *debug )
{
  int status, ret_val;
  char reply[STRING_LENGTH];
  char Reply[STRING_LENGTH];

  /*
    Variables needed to use the select commands to watch for data on a port
  */
  fd_set rfds;
  struct timeval tv;

  /*
    All commands require a connection to the PCR server, so every time
      PCR_SendCmd is called, we will check to make sure we are connected, or
      will initiate this connection
  */
  if ( !Info->socket->connected ) {

    /*
      Open a socket to the PCR server
    */
    status =  Socket_OpenClient ( (Socket_Info *)Info->socket, *debug);
    if ( status ) {
      printf("  PCR_SendCmd: Error in Socket_OpenClient\n");
      fflush(stdout);
      return(-1);
    }
  }

  /*
    Lock the mutex variable PCR_CMD_Lock so other threads do not try
      to read or write while this thread is retrieving status information.
    If another thread has the variable PCR_CMD_Lock locked, this process
      is blocked until the mutex is unlocked.  Likewise, if the mutex
      variable is locked here, other processed are blocked until the mutex
      is unlock by this thread.
    In all other routines that communication on the PCR_CMD port the
      same mutex lock function is called.
  */
  pthread_mutex_lock( &PCR_CMD_Lock);

  /*
    Send the request to the PCR Cmd server
  */
  strcat( request, "\n");
  if ( Socket_StringWrite( Info->socket->sockfd, request, strlen(request)) != strlen(request) ) {
    if ( *debug ) {
      printf("  PCR_SendCmd: Error writing start_WFSC in Socket_StringWrite\n");
      fflush(stdout);
    }
    pthread_mutex_unlock( &PCR_CMD_Lock);
    return(-1);
  }

  /*
    Trim control characters from the end (\n, \r, \t etc)
    => Case is important to PCR
  */
  status = Socket_StringTrim(request);

  /*
    Watch to see when there is data to be read on sockfd
  */
  FD_ZERO(&rfds);
  FD_SET(Info->socket->sockfd, &rfds);
  /* Wait up to one seconds. */
  tv.tv_sec = 6;
  tv.tv_usec = 0;
 
  status = select( (Info->socket->sockfd)+1, &rfds, NULL, NULL, &tv);

  if ( !status ) {

    /*
      If we do not receive a reply.  For now we will ignore this error,
        assuming that the PCR is not yet set up to send back replies
        **Must Fix  20nov06 DLM
    */
    printf("  PCR_SendCmd: Reply not received\n");
    printf("  PCR_SendCmd: unlocking mutex\n\n");
    fflush(stdout);
    pthread_mutex_unlock( &PCR_CMD_Lock);
    return(-1);

  } else {

    /*
      Read an instruction from the client
    */
    if ( Socket_StringRead( Info->socket->sockfd, reply) ) {
      printf("  PCR_SendCmd: Error reading reply in Socket_StringRead\n");
      printf("  PCR_SendCmd: unlocking mutex\n\n");
      fflush(stdout);
      Socket_Close( (Socket_Info *)Info->socket, *debug);
      pthread_mutex_unlock( &PCR_CMD_Lock);
      return(-1);
    }

    /*
      Check that a request was sent
    */
    if ( strlen(reply) <= 1 ) {
      printf("  DServ_Process: Warning: reply of zero length recieved\n");
      printf("  PCR_SendCmd: unlocking mutex\n\n");
      fflush(stdout);
      pthread_mutex_unlock( &PCR_CMD_Lock);
      return(-1);
    }

    if ( *debug ) {
      printf("  PCR_SendCmd:   Reply = %s\n", reply);
    }

    /*
      Set request and reply to all lower case and trim control characters from 
        the end (\n, \r, \t etc)
    */
    status = Socket_StringLowerCase(request);
    strcpy( Reply, reply);
    status = Socket_StringLowerCase(reply);

    if ( !strncmp (reply, "ok", strlen("ok") ) ) {

      ret_val = 0;

    } else if ( !strncmp (reply, "error", strlen("error") ) ) {

      printf("  PCR_SendCmd: %s\n", Reply);
      fflush(stdout);
      ret_val = -1;

    } else {

      printf("  PCR_SendCmd: Unknown reply from PCR: %s\n", Reply);
      fflush(stdout);
      ret_val = -1;

    }

  }

  /*
    Unlock the mutex variable PCR_CMD_Lock so other threads may use the
      PCR_CMD port
  */
  pthread_mutex_unlock( &PCR_CMD_Lock);

  return(ret_val);

}
