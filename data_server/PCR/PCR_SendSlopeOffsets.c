/*
  PCR_SendSlopeOffsets.c

  This routine connects to the PC-Reconstructor (PCR) and sends
    a float array[288], in binary format, to the PCR.

  Written  7dec06  DLM  Started with PCR_SendCamBG.c

  Altered   2apr07  DLM  Added mutex lock and unlock to keep from having
                           collisions between different threads trying to
			   use the PCR_CMD port.

*/
#include <pthread.h>

#include "PCR.h"
#include "DServ.h"

#define DEBUG 0

/*
  The mutex variable PCR_CMD_Lock is a global variable set in PCR_Server.c.  We
    must now make it visible in this routine.
*/
extern pthread_mutex_t PCR_CMD_Lock;

/*================================================================================*
 * Send command and array
 *================================================================================*/
int PCR_SendSlopeOffsets( DServ_Info *Info, Info_Entry *Static , int sockfd, int *debug )
{
  int status, i, j, ret_val;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  char Reply[STRING_LENGTH];

  float *float_vector;
  float *flipped;
  char *float_ptr;
  long vec_length = 288;
  int mode[7];

  /*
    Allocate needed memory
  */
  float_vector = (float *)malloc(vec_length*sizeof(float));
  flipped = (float *)malloc(vec_length*sizeof(float));

  /*
    Variables needed to use the select commands to watch for data on a port
  */
  fd_set rfds;
  struct timeval tv;

  /*
    Read the contribution of each of the seven modes in the static Slope Offset vector 
  */
  for ( i=0; i<7; i++) {
    if ( Socket_StringRead( sockfd, reply) ) {
      if ( *debug ) {
	printf("  PCR_Process: Error reading request in Socket_StringRead\n");
	fflush(stdout);
      }
      free(float_vector);
      free(flipped);
      return(-1);
    }
    mode[i] = atoi(reply);
  }

  /*
    Read the slope offset array from the client
  */
  float_ptr = (char *)float_vector;
  if ( Socket_Read( sockfd, float_ptr, vec_length*sizeof(float)) ) {
    printf("  PCR_SendCmd: Error reading slope offset array in Socket_Read\n");
    fflush(stdout);
    free(float_vector);
    free(flipped);
    return(-1);
  }

  if ( DEBUG ) {

    printf("\n PCR_SendSlopeOffsets\n");
    printf("---------------------------\n");

    printf("    X-Slopes\n");
    printf("    --------\n");
    for ( i=0; i<12; i++) {
      for ( j=0; j<12; j++) {
	printf("%10.2e", float_vector[i*12+j]);
      }
      printf("\n");
    }
    printf("\n    Y-Slopes\n");
    printf(  "    --------\n");
    for ( i=0; i<12; i++) {
      for ( j=0; j<12; j++) {
	printf("%10.2e", float_vector[144+i*12+j]);
      }
      printf("\n");
    }
    fflush(stdout);
  }
    
  /*
    All commands require a connection to the PCR server, so every time
      PCR_SendSlopeOffsets is called, we will check to make sure we are connected, or
      will initiate this connection
  */
  if ( !Info->socket->connected ) {

    /*
      Open a socket to the PCR server
    */
    status =  Socket_OpenClient ( (Socket_Info *)Info->socket, *debug);
    if ( status ) {
      printf("  PCR_SendSlopeOffsets: Error in Socket_OpenClient\n");
      fflush(stdout);
      free(float_vector);
      free(flipped);
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
  strcpy( request, "slope_offset_set\n");
  if ( Socket_StringWrite( Info->socket->sockfd, request, strlen(request)) != strlen(request) ) {
    if ( *debug ) {
      printf("  PCR_SendSlopeOffsets: Error writing slope_offset_set in Socket_StringWrite\n");
      fflush(stdout);
    }
    free(float_vector);
    free(flipped);
    pthread_mutex_unlock( &PCR_CMD_Lock);
    return(-1);
  }

  /*
    Write the slope offset array on the PCR_Cmd socket
  */

#ifdef MAC
  status = Socket_ConvertFloat( float_vector, flipped, vec_length);
#else
  for ( i=0; i<vec_length; i++) {
    flipped[i] = float_vector[i];
  }
#endif

  float_ptr = (char *)flipped;
  if ( Socket_Write( Info->socket->sockfd, float_ptr, vec_length*sizeof(float)) ) {
    printf("  PCR_SendCmd: Error writing float vector in Socket_Write\n");
    fflush(stdout);
    free(float_vector);
    free(flipped);
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
      If we do not receive a reply.  For now we will ignore this error,
        assuming that the PCR is not yet set up to send back replies
        **Must Fix  20nov06 DLM
    */
    printf("  PCR_SendSlopeOffsets: Reply not received\n");
    fflush(stdout);
    ret_val = -1;

  } else {

    /*
      Read an instruction from the client
    */
    if ( Socket_StringRead( Info->socket->sockfd, reply) ) {
      printf("  PCR_SendSlopeOffsets: Error reading reply in Socket_StringRead\n");
      fflush(stdout);
      free(float_vector);
      free(flipped);
      pthread_mutex_unlock( &PCR_CMD_Lock);
      return(-1);
    }

    /*
      Check that a request was sent
    */
    if ( strlen(reply) <= 1 ) {
      printf("  DServ_Process: Warning: reply of zero length recieved\n");
      fflush(stdout);
      free(float_vector);
      free(flipped);
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
        printf("  PCR_SendSlopeOffsets: Reply = %s\n", Reply);
	fflush(stdout);
      }
      for ( i=0; i<7; i++) {
	Static[i].i_value = mode[i];
      }
      ret_val = 0;

    } else if ( !strncmp (reply, "error", strlen("error") ) ) {

      if ( *debug ) {
        printf("  PCR_SendSlopeOffsets: Reply = %s\n", Reply);
	fflush(stdout);
      }
      ret_val = -1;

    } else {

      printf("  PCR_SendSlopeOffsets: Unknown reply from PCR: %s\n", Reply);
      fflush(stdout);
      ret_val = -1;

    }

  }

  free(float_vector);
  free(flipped);

  /*
    Unlock the mutex variable PCR_CMD_Lock so other threads may use the
      PCR_CMD port
  */
  pthread_mutex_unlock( &PCR_CMD_Lock);

  return(ret_val);

}
