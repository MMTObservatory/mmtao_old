/*===========================================================================*
**
**  Stage_Write.c
**
**  Utility routines to write to an Stage device controller
**
**  Returns :  0 success
**            -1 error
**
**  Modified  24aug07  tjt  Started with MVP_Write.c
**
**===========================================================================*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Stage.h"

/*=========================================
 * function: Stage_Write
 *=========================================*/

int Stage_Write( Socket_Info *Socket, char *request, int debug)
{
  int debug_local = 0;

  if ( debug_local || debug ) {
    printf("  Stage_Write: Writing \"%s\\r\" to %s\n", request, Socket->name);
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
      printf("  Stage_Write: Error writing \"%s\" request to %s\n", request, Socket->name);
      fflush(stdout);
    }
    return(-1);
  }

  return(0);
}

/* THE END */
