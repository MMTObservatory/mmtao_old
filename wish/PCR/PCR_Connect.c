/*
  PCR_Connect.c

  Routine to request open a socket to the PCR (DServ) server

  Written  17jun06  DLM  Started with DClient_WFSC_Init.c

*/

#include "PCR_Wish.h"
#include "Client.h"
#include "Server.h"
#include "PCR.h"

int PCR_Connect( Server_Info *PCR_Info, int debug, char *errorMsg)
{

  int status;

  /*
    Setup Server information in structure, if not already connected
  */
  if ( !PCR_Info->connected ) {

    status = Server_SetupState( PCR_Info, "PCR_Server", debug);
    if ( status ) {
      strcpy( errorMsg, "PCR_Connect: Error in Server_SetupState");
      if ( debug ) printf("  %s\n", errorMsg);
      return(status);
    }

    status = Server_Open( PCR_Info, debug);
    if ( status == -12 ) {
      sprintf( errorMsg, "%s%s\n%s%s%s",
	       "Could not connect to ", PCR_Info->name,
	       "Check that ", PCR_Info->type, " is running");
      if ( debug ) printf("  %s\n", errorMsg);
      return(status);
    } else if ( status ) {
      sprintf( errorMsg, "%s\n%s%s\n",
	       "Error setting up connection", " to ", PCR_Info->name);
      if ( debug ) printf("  %s\n", errorMsg);
      return(status);
    }

  }

  return(0);
}
