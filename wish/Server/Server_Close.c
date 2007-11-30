/*===========================================================================*
**
**  Server_Close.c
**
**  Close a socket connection.
**
**  Written  22apr05  DLM  Started with Server_Open.c
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "DClient.h"

/*=========================================
 * function: Server_Close
 *=========================================*/

int Server_Close (Server_Info *Info, int debug)
{

  if ( debug ) {
    printf("  Server_Close: Closing connecting\n");
  }

  close(Info->fd);

  /*
    Fill in the Info structure
  */
  Info->connected = 0;
  Info->fd = 0;

  return(0);
}
