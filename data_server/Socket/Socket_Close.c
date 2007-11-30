/*===========================================================================*
**
**  Socket_Close.c
**
**  Close a socket connection.
**
**  Returns :  0 success
**            -1 error
**
**  Written  28dec04  DLM  Started with FSM_Init.  Made it a generic socket
**                           close routine.
**
**===========================================================================*/

#include "PCR.h"
#include "Socket.h"

/*=========================================
 * function: Socket_Close
 *=========================================*/

int Socket_Close (Socket_Info *Info, int debug)
{

  if ( debug ) {
    printf("  Socket_Close: Closing %s socket\n", Info->name);
    fflush(stdout);
  }

  /*
    Close the connection
  */
  close (Info->sockfd);

  /*
    Fill in the Info structure
  */
  Info->sockfd = -1;
  Info->connected = 0;
  Info->error = 0;

  return(0);
}
