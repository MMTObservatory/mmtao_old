/*===========================================================================*
**
**  MVP_Write.c
**
**  Utility routines to write to an MVP device controller
**
**  Returns :  0 success
**            -1 error
**
**  Written  17aug07  DLM  Started with MVP_Read
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "MVP.h"

/*=========================================
 * function: MVP_Write
 *=========================================*/

int MVP_Write( Socket_Info *Socket, char *request, int debug)
{
  int debug_local = 0;

  if ( debug_local || debug ) {
    printf("  MVP_Write: Writing \"%s\\r\" to %s\n", request, Socket->name);
  }

  /* 
     Flush any unexpected input
  */
  Socket_Flush ( Socket->sockfd);

  /*
    Add linefeed (\r) to request
  */
  strcat( request, "\r");

  /*
    Send the request
  */
  if ( Socket_StringWrite( Socket->sockfd, request, strlen(request)) != strlen(request) ) {
    if ( debug ) {
      printf("  MVP_Write: Error writing \"%s\" request to %s\n", request, Socket->name);
      fflush(stdout);
    }
    return(-1);
  }

  return(0);
}
