/*
  PCR_SendIntVector.c

  This routine connects to the PC-Reconstructor (PCR) and sends
    a integer array[], in binary format, to the PCR.

  Written  7dec06  DLM  Started with PCR_SendFloatVector.c

  Altered   2apr07  DLM  Added mutex lock and unlock to keep from having
                           collisions between different threads trying to
			   use the PCR_CMD port.

*/
#include <pthread.h>

#include "PCR.h"
#include "DServ.h"
#include "PCR_Cmd.h"

#define DEBUG 0

/*
  The mutex variable PCR_CMD_Lock is a global variable set in PCR_Server.c.  We
    must now make it visible in this routine.
*/
extern pthread_mutex_t PCR_CMD_Lock;

/*================================================================================*
 * Send command and array
 *================================================================================*/
int PCR_SendIntVector( DServ_Info *Info, int sockfd, char *name, int *debug )
{
  int status, i, j, ret_val;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  char Reply[STRING_LENGTH];
  long vec_length;

  int *int_vector;
  int *net_vector;
  char *int_ptr;

  /*
    Read the length of the vector to read and send to the PCR
  */

  if ( Socket_StringRead( sockfd, request) ) {
    if ( *debug ) {
      printf("  PCR_SendIntVector: Error reading vector length in Socket_StringRead\n");
      fflush(stdout);
    }
  }
  vec_length = atol(request);

  /*
    Allocate needed memory
  */
  int_vector = (int *)malloc(vec_length*sizeof(int));
  net_vector = (int *)malloc(vec_length*sizeof(int));

  /*
    Variables needed to use the select commands to watch for data on a port
  */
  fd_set rfds;
  struct timeval tv;

  /*
    Read the slope offset array from the client
  */
  int_ptr = (char *)int_vector;
  if ( Socket_Read( sockfd, int_ptr, vec_length*sizeof(int)) ) {
    printf("  PCR_SendIntVector: Error reading integer array in Socket_Read\n");
    fflush(stdout);
    free(int_vector);
    free(net_vector);
    return(-1);
  }

  if ( DEBUG ) {

    printf("\n PCR_SendIntVector\n");
    printf("---------------------------\n");

    if ( !strncmp ( name, "cam_bg_set", strlen("cam_bg_set") ) ) {

      printf("    Camera Background\n");
      printf("    -----------------\n");
      for ( i=0; i<24; i++) {
	for ( j=0; j<24; j++) {
	  printf("%5d", int_vector[i*24+j]);
	}
	printf("\n");
      }

    } else if ( !strncmp ( name, "cmd_offset_set", strlen("cmd_offset_set") ) ) {

      printf("    Command Offset\n");
      printf("    --------------\n");
      for ( i=0; i<10; i++) {
	for ( j=0; j<36; j++) {
	  printf("%5d", int_vector[i*10+j]);
	}
	printf("\n");
      }

    }
    fflush(stdout);
  }

  if ( vec_length != 576 ) {

    printf("  PCR_SendIntVector: Incorrect number of pixel values read\n");
    printf("                     Found %ld\n", vec_length);
    printf("                     Expected 576\n");
    fflush(stdout);
    return(-1);

  }
    
 /*
    All commands require a connection to the PCR server, so every time
      PCR_SendIntVector is called, we will check to make sure we are connected, or
      will initiate this connection
  */
  if ( !Info->socket->connected ) {

    /*
      Open a socket to the PCR server
    */
    status =  Socket_OpenClient ( (Socket_Info *)Info->socket, *debug);
    if ( status ) {
      printf("  PCR_SendIntVector: Error in Socket_OpenClient\n");
      fflush(stdout);
      free(int_vector);
      free(net_vector);
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
  if ( !strncmp ( name, "cam_bg_set", strlen("cam_bg_set") ) ) {
    strcpy( request, "cam_bg_set\n");
    if ( Socket_StringWrite( Info->socket->sockfd, request, strlen(request)) != strlen(request) ) {
      if ( *debug ) {
	printf("  PCR_SendIntVector: Error writing cam_bg_set in Socket_StringWrite\n");
	fflush(stdout);
      }
      free(int_vector);
      free(net_vector);
      pthread_mutex_unlock( &PCR_CMD_Lock);
      return(-1);
    }

  } else if ( !strncmp ( name, "cmd_offset_set", strlen("cmd_offset_set") ) ) {
    strcpy( request, "cmd_offset_set\n");
    if ( Socket_StringWrite( Info->socket->sockfd, request, strlen(request)) != strlen(request) ) {
      if ( *debug ) {
	printf("  PCR_SendIntVector: Error writing cmd_offset_set in Socket_StringWrite\n");
	fflush(stdout);
      }
      free(int_vector);
      free(net_vector);
      pthread_mutex_unlock( &PCR_CMD_Lock);
      return(-1);
    }

  } else {

    printf("  PCR_SendIntVector: Unknown request %s\n", name);
    fflush(stdout);
    free(int_vector);
    free(net_vector);
    pthread_mutex_unlock( &PCR_CMD_Lock);
    return(-1);

  }

  /*
    Convert the integer array to network format
  */
  for ( i=0; i<vec_length; i++) {
    net_vector[i] = htons(int_vector[i]);
  }

  /*
    Write the integer array on the PCR_Cmd socket
  */
  int_ptr = (char *)net_vector;
  if ( Socket_Write( Info->socket->sockfd, int_ptr, vec_length*sizeof(float)) ) {
    printf("  PCR_SendIntVector: Error writing float vector in Socket_Write\n");
    fflush(stdout);
    free(int_vector);
    free(net_vector);
    pthread_mutex_unlock( &PCR_CMD_Lock);
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
      If we do not receive a reply then probably the PCR is not yet
        set up to send back replies for this command.
    */
    printf("  PCR_SendIntVector: Reply not received\n");
    fflush(stdout);
    ret_val = -1;

  } else {

    /*
      Read an instruction from the client
    */
    if ( Socket_StringRead( Info->socket->sockfd, reply) ) {
      printf("  PCR_SendIntVector: Error reading reply in Socket_StringRead\n");
      fflush(stdout);
      free(int_vector);
      free(net_vector);
      pthread_mutex_unlock( &PCR_CMD_Lock);
      return(-1);

    }

    /*
      Check that a request was sent
    */
    if ( strlen(reply) <= 1 ) {
      printf("  DServ_Process: Warning: reply of zero length recieved\n");
      fflush(stdout);
      free(int_vector);
      free(net_vector);
      pthread_mutex_unlock( &PCR_CMD_Lock);
      return(-1);
    }

    /*
      Set request and reply to all lower case and trim control characters from 
        the end (\n, \r, \t etc)
    */
    status = Socket_StringLowerCase(request);
    strcpy( Reply, reply);
    status = Socket_StringLowerCase(reply);

    if ( !strncmp (reply, "ok", strlen("ok") ) ) {

      if ( *debug ) {
        printf("  PCR_SendIntVector: Reply = %s\n", Reply);
	fflush(stdout);
      }
      ret_val = 0;

    } else if ( !strncmp (reply, "error", strlen("error") ) ) {

      if ( *debug ) {
        printf("  PCR_SendIntVector: Reply = %s\n", Reply);
	fflush(stdout);
      }
      ret_val = -1;

    } else {

      printf("  PCR_SendIntVector: Unknown reply from PCR: %s\n", Reply);
      fflush(stdout);
      ret_val = -1;

    }

  }

  free(int_vector);
  free(net_vector);

  /*
    Unlock the mutex variable PCR_CMD_Lock so other threads may use the
      PCR_CMD port
  */
  pthread_mutex_unlock( &PCR_CMD_Lock);

  return(ret_val);

}
