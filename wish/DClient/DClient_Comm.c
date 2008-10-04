/************************************************************************

  DClient_Comm.c
  --------------

    DClient_ReadString.c
    DClient_ReadLong.c
    DClient_WriteLong.c
    DClient_WriteString.c

  Written  22mar05  DLM  Grouped all the Read and Write routines together

************************************************************************/

#include "PCR_Wish.h"
#include "DClient.h"

/************************************************************************

  DClient_ReadString.c

  Read the requested number of bytes from the given socket

  Written  26aug03  DLM

  Altered  17mar06  DLM  Added select so read will timeout if there is
                           a problem or not enough data were read
                         Renamed from DClient_ReadChar

************************************************************************/

int DClient_ReadString(register int fd, register char *ptr, register int nbytes, char *msg)
{
  int nleft, nread;
  fd_set rfds;
  struct timeval tv;
  int status;

  /*
    Watch to see when there is data to be read on sockfd
  */
  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  /* Wait up to one seconds. */
  tv.tv_sec = 1;
  tv.tv_usec = 0;
 
  nleft = nbytes;
  while (nleft > 0) {

    /*
      Wait a given amount of time till data is available on the socket
    */
    status = select( fd+1, &rfds, NULL, NULL, &tv);

    /*
      If data does not show up, or not enough data, then return and error string
        to the calling routine
    */
    if ( !status ) {
      sprintf( msg, "DClient_ReadString: Timed out waiting for data\n  %d bytes requested\n  %d bytes read\n",
	      nbytes, nbytes-nleft);
      return( -1 * nleft);
    }

    nread = read(fd, ptr, nleft);
    if (nread < 0)return(nread);
    else if (nread == 0)break;

    nleft -= nread;
    ptr += nread;
  }
  return(-1 * nleft);
}

/************************************************************************

  DClient_ReadLong.c

  Read a single long values from the Data Server running on the VME
    Master.  A request is sent (eg "size") and a single long value
    is returned.  This value, post ntohl(), is return to the calling
    routine.

  Written  26aug03  DLM

************************************************************************/

int DClient_ReadLong( int sockfd, char *request, char *msg)
{

  char requestData[MAX_REQUEST];
  char returnData[MAX_VALUES];
  int longValue;
  int n, num, status;

  strcpy(requestData, request);
  strcat(requestData, "\n");
  n = strlen(requestData);

  num = write(sockfd, requestData, n);
  if (num != n) {
    printf(msg, "DClient_ReadLong: Write error on socket\n");
    return(-1);
  }
  
  n = sizeof(int);
  status = DClient_ReadString(sockfd, returnData, n, msg);
  if ( status ) {
    printf(msg, "DClient_ReadLong: Read error on socket\n");
    return(-1);
  }
  longValue = ntohl(*(int *)returnData);

  return(longValue);
}

/************************************************************************

  Write a single long values to the Data Server running on the WFC.
    Master.

  Written  5sept03  DLM

************************************************************************/

int DClient_WriteLong( int sockfd, int longValue, char *msg)
{

  char requestData[MAX_REQUEST];
  int n, num;

  sprintf(requestData, "%d\n", longValue);
  n = strlen(requestData);

  num = write(sockfd, requestData, n);
  if (num != n) {
    sprintf(msg, "DClient_WriteLong: Write error on socket");
    return(-1);
  }
  
  return(0);
}

/************************************************************************
  DClient_WriteString.c

  Send a request string to the Data Server running on the WFC.
    A request is sent (eg "size") with a long reply indicating
    sucess (0) or failure (-1) of the requested command.

  Written  27aug03  DLM

************************************************************************/

int DClient_WriteString( int sockfd, char *request, char *msg)
{

  char requestData[MAX_REQUEST];
  int n, num;

  strcpy(requestData, request);
  strcat(requestData, "\n");
  n = strlen(requestData);

  num = write(sockfd, requestData, n);
  if (num != n) {
    sprintf(msg, "DClient_WriteString:Write error on socket\n");
    return(-1);
  }

  return(0);
}
