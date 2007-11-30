/*
  ======================
  Mod_OffsetsSend()
  ======================

  Written 6dec06  DLM  Started with Static_Calc.c

*/

#include "PCR_Wish.h"
#include "Mod.h"
#include "PCR.h"

/*================================================================================*
 * Send the slope offset vector to the PCR
 *================================================================================*/
int Mod_OffsetsSend( Server_Info *Info, float *offsets, int debug, char *errorMsg)
{

  int status;
  char request[SERVER_LENGTH];
  char *charptr;

  if ( !Info->connected ) {

    /*
      Open a socket to the DServ server
    */
    status = PCR_Connect( Info, debug, errorMsg);
    if ( status ) {
      strcpy( errorMsg, "Mod_OffsetsSend: Could not connect to PCR Cmd port");
      printf("%s\n", errorMsg);
      return (status);
    }

    if ( debug ) {
      printf("  Mod_OffsetsSend: Connected to DServ server\n");
    }
  }

  /*
    Send request to PCR Cmd server
  */
  strcpy( request, "mod_offset_set");
  status = PCR_Request( Info, request, errorMsg);
  if ( status ) {
    strcpy( errorMsg, "Mod_OffsetsSend: Error in PCR_Request sending mod_offset_set");
    printf("%s\n", errorMsg);
    return (status);
  }

  if ( debug) {
    printf("  Mod_OffsetsSend: %s\n", request);
  }

  /*
    Send the vector of floats to PCR Cmd server
  */
  charptr = (char *)offsets;
  status = PCR_Write( Info->fd, charptr, 288*sizeof(float));
  if ( status ) {
    strcpy( errorMsg, "Mod_OffsetsSend: Error in PCR_Write sending Slope Offsets");
    printf("%s\n", errorMsg);
    return (status);
  }

  return(0);
}
