/************************************************************************

  Socket_ReadRaw.c

    Read an unknown number of bytes from the given socket

  Written  12dec05  DLM  Started with Server_Com.c
  Modified 12aug07  tjt  trivialize Socket_Read.c

************************************************************************/

#include "Topbox.h"
#include "Socket.h"

/************************************************************************
 * Read an unknown number of bytes from given socket
 ************************************************************************/

int Socket_ReadRaw( int sockfd, char *buffer, int nBuf)
{
    return read(sockfd, buffer, nBuf);
}

/* THE END */
