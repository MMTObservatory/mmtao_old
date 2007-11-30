/*
  PCR_Request.c

  This routine sets the debug flag to the requested value for the requested
    server.

  Written  29mar05  DLM

*/

#include "PCR_Wish.h"
#include "Client.h"
#include "Server.h"
#include "PCR.h"

int PCR_Request( Server_Info *Info, char *request, char *errorMsg)
{

  int status;

  sprintf( Info->out_line, "%s\n", request);

  status = Server_Write( (Server_Info *)Info);
  if ( status ) {
    strcpy( errorMsg, "PCR_Request: Error in Server_Write sending Single request");
    printf("%s\n", errorMsg);
    return(status);
  }

  return(0);
}
