/*===========================================================================*
**
**  Client_Close.c
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

#include "PCR_Wish.h"
#include "Client.h"

/*=========================================
 * function: Client_Close
 *=========================================*/
int Client_Close(Client_Info *Info, int debug)
{

  if ( debug ) {
    printf("  Client_Close: Closing %s socket\n", Info->name);
  }

  /*
    Close the connection
  */
  close (Info->fd);

  /*
    Fill in the Info structure
  */
  Info->fd = 0;
  Info->connected = 0;
  Info->error = 0;

  return(0);
}
