/*
  ======================
  Static_Send()
  ======================

  Written 6dec06  DLM  Started with Static_Calc.c

*/

#include "PCR_Wish.h"

#include "Client.h"
#include "Server.h"
#include "Static.h"
#include "PCR.h"

/*================================================================================*
 * Send the slope offset vector to the PCR
 *================================================================================*/
int Static_Send( Server_Info *Info, float *offsets, float *scale, int debug, char *errorMsg)
{

  int status;
  char request[STRING_LENGTH];
  char *charptr;
  int i;

  if ( !Info->connected ) {

    /*
      Open a socket to the DServ server
    */
    status = PCR_Connect( Info, debug, errorMsg);
    if ( status ) {
      strcpy( errorMsg, "Static_Send: Could not connect to PCR Cmd port");
      printf("%s\n", errorMsg);
      return (status);
    }

    if ( debug ) {
      printf("  Static_Send: Connected to DServ server\n");
    }
  }

  /*
    Send request to PCR Cmd server
  */
  strcpy( request, "slope_offset_set");
  status = PCR_Request( Info, request, errorMsg);
  if ( status ) {
    strcpy( errorMsg, "Static_Send: Error in PCR_Request sending slope_offset_set");
    printf("%s\n", errorMsg);
    return (status);
  }

  if ( debug) {
    printf("  Static_Send: %s\n", request);
  }

  for ( i=0; i<7; i++) {
    sprintf( request, "%d", (int) scale[i]);
    status = PCR_Request( Info, request, errorMsg);
    if ( status ) {
      strcpy( errorMsg, "Static_Send: Error in scale sending slope_offset_set");
      printf("%s\n", errorMsg);
      return (status);
    }
  }

  /*
    Send the vector of floats to PCR Cmd server
  */
  charptr = (char *)offsets;
  status = PCR_Write( Info->fd, charptr, 288*sizeof(float));
  if ( status ) {
    strcpy( errorMsg, "Static_Send: Error in PCR_Write sending Slope Offsets");
    printf("%s\n", errorMsg);
    return (status);
  }

  return(0);
}
