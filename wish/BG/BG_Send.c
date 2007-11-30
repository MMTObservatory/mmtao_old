/*
  ======================
  BG_Send()
  ======================

  Written  6apr07  DLM  Started with Mod_RateSend.c

*/

#include "PCR_Wish.h"
#include "Client.h"
#include "Server.h"
#include "PCR.h"

/*================================================================================*
 * Send the slope offset vector to the PCR
 *================================================================================*/
int BG_Send( Server_Info *Info, char *fileName, int debug, char *errorMsg)
{

  int status;
  char ascii[120];
  int i;
  int num_values;
  char request[SERVER_LENGTH];
  char *charptr;
  short *bg;
  short *bg_net;
  short *nxtShort;
  short short_value;
  int int_value;
  FILE *fptr;

  debug = 0;

  if((fptr = fopen(fileName, "r"))){

    //if(!fgets(ascii, 120, fptr)){		    /* read line, up to 120 characters. */
    //  return(1);
    // }

    /* Read any number of comment lines
    while(ascii[0] == '#'){
      if(!fgets(ascii, 120, fptr)){
	return(2);
      }
    }
    */
    /* Read the number of pixel values to read
    if(sscanf(ascii, "%d", &num_values) != 1){
      return(5);
    }
    */

    num_values = 576;

    if ( debug ) {
      printf("\n BG_Send\n");
      printf("---------------------------\n\n");
      printf(" file = %s\n", fileName);
      printf(" num_values = %d\n\n", num_values);
    }

    /*
      Allocate the memory need for the Background Vector.
    */
    bg = (short *)malloc(num_values*sizeof(short));
    if ( bg == NULL ) {
      strcpy( errorMsg, "BG_Send: Error allocating bg");
      printf("%s\n", errorMsg);
      return(status);
    }

    bg_net = (short *)malloc(num_values*sizeof(short));
    if ( bg_net == NULL ) {
      strcpy( errorMsg, "BG_Send: Error allocating bg_net");
      printf("%s\n", errorMsg);
      free(bg);
      return(status);
    }

    nxtShort = bg;

    for(i = 0; i < num_values; i++){

      if(!fgets(ascii, 120, fptr)){		    /* read line, up to 120 characters. */
	return(1);
      }
      sscanf(ascii, "%d", &int_value);
      short_value = int_value;
      *nxtShort++ = htons(short_value);

    }

    fclose(fptr);

    if ( debug ) {
      for(i = 0; i < num_values; i++){
	printf("%d\n", ntohs(bg[i]));
      }
    }

  } else {

    sprintf( errorMsg, "BG_Send: Could not file file\n%s", fileName);
    printf("%s\n", errorMsg);
    free(bg);
    free(bg_net);
    return(status);
  }

  /*
    Send this background vector to the PCR_Server
  */

  if ( !Info->connected ) {

    /*
      Open a socket to the DServ server
    */
    status = PCR_Connect( Info, debug, errorMsg);
    if ( status ) {
      strcpy( errorMsg, "BG_Send: Could not connect to PCR Cmd port");
      printf("%s\n", errorMsg);
      free(bg);
      free(bg_net);
      return(status);
    }

    if ( debug ) {
      printf("  BG_Send: Connected to DServ server\n");
    }
  }

  /*
    Send request to PCR Cmd server
  */
  strcpy( request, "cam_bg_set");
  status = PCR_Request( Info, request, errorMsg);
  if ( status ) {
    strcpy( errorMsg, "  BG_Send: Error in PCR_Request sending cam_bg_set");
    printf("%s\n", errorMsg);
    free(bg);
    free(bg_net);
    return (status);
  }

  if ( debug) {
    printf("  BG_Send: Request = %s\n", request);
  }

  /*
    Send vector length to PCR Cmd server
  */
  sprintf( request, "%d", num_values);
  status = PCR_Request( Info, request, errorMsg);
  if ( status ) {
    strcpy( errorMsg, "  BG_Send: Error in PCR_Request sending cam_bg_set");
    printf("%s\n", errorMsg);
    free(bg);
    free(bg_net);
    return (status);
  }

  if ( debug) {
    printf("  BG_Send: Number of Pixels = %s\n", request);
  }

  /*
    Send the vector of shorts in network format to PCR Cmd server
  */
  charptr = (char *)bg;
  status = PCR_Write( Info->fd, charptr, num_values*sizeof(short));
  if ( status ) {
    strcpy( errorMsg, "  BG_Send: Error in PCR_Write sending background");
    printf("%s\n", errorMsg);
    free(bg);
    free(bg_net);
    return (status);
  }

  free(bg);
  free(bg_net);

  return(0);
}
