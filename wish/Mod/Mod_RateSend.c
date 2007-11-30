/*
  ======================
  Mod_RateSend()
  ======================

  Written 17jan06  DLM  Started with Mod_Offsets_Send.c

*/

#include "PCR_Wish.h"
#include "Mod.h"
#include "PCR.h"

/*================================================================================*
 * Send the slope offset vector to the PCR
 *================================================================================*/
int Mod_RateSend( Server_Info *Info, char *fileName, int debug, char *errorMsg)
{

  int status;
  char ascii[120];
  int i, num_values;
  char request[SERVER_LENGTH];
  char *charptr;
  float *rate;
  float *nxtfloat;
  float flt;
  FILE *fptr;

  if((fptr = fopen(fileName, "r"))){

    if(!fgets(ascii, 120, fptr)){		    /* read line, up to 120 characters. */
      return(1);
    }

    /* Read any number of comment lines */
    while(ascii[0] == '#'){
      if(!fgets(ascii, 120, fptr)){
	return(2);
      }
    }

    /* Read two integer (number of slope, number of modes) and a float (Scale factor) */

    if(sscanf(ascii, "%d", &num_values) != 1){
      return(5);
    }
    if ( debug ) {
      printf("\n Mod_RateSend\n");
      printf("---------------------------\n\n");
      printf(" file = %s\n", fileName);
      printf(" num_values = %d\n\n", num_values);
    }

    /*
      Allocate the memory need for the Modulation Rate Vector.
    */
    rate = (float *)malloc(num_values*sizeof(float));
    if ( rate == NULL ) {
      strcpy( errorMsg, "Mod_RateSend: Error allocating rate");
      printf("%s\n", errorMsg);
      free(rate);
      return(status);
    }

    nxtfloat = rate;

    for(i = 0; i < num_values; i++){

      if(!fgets(ascii, 120, fptr)){		    /* read line, up to 120 characters. */
	return(1);
      }
      sscanf(ascii, "%f", &flt);
      *nxtfloat++ = flt;

    }

    fclose(fptr);

    if ( debug ) {
      for(i = 0; i < num_values; i++){
	printf("%f\n", rate[i]);
      }
    }

  } else {

    sprintf( errorMsg, "Mod_RateSend: Could not file file\n%s", fileName);
    printf("%s\n", errorMsg);
    free(rate);
    return(status);
  }

  /*
    Send this rate vector to the PCR_Server
  */

  if ( !Info->connected ) {

    /*
      Open a socket to the DServ server
    */
    status = PCR_Connect( Info, debug, errorMsg);
    if ( status ) {
      strcpy( errorMsg, "Mod_RateSend: Could not connect to PCR Cmd port");
      printf("%s\n", errorMsg);
      free(rate);
      return(status);
    }

    if ( debug ) {
      printf("  Mod_RateSend: Connected to DServ server\n");
    }
  }

  /*
    Send request to PCR Cmd server
  */
  strcpy( request, "mod_rate_set");
  status = PCR_Request( Info, request, errorMsg);
  if ( status ) {
    strcpy( errorMsg, "Mod_RateSend: Error in PCR_Request sending mod_rate_set");
    printf("%s\n", errorMsg);
    free(rate);
    return (status);
  }

  if ( debug) {
    printf("  Mod_RateSend: %s\n", request);
  }

  /*
    Send vector length to PCR Cmd server
  */
  sprintf( request, "%d", num_values);
  status = PCR_Request( Info, request, errorMsg);
  if ( status ) {
    strcpy( errorMsg, "Mod_RateSend: Error in PCR_Request sending mod_rate_set");
    printf("%s\n", errorMsg);
    free(rate);
    return (status);
  }

  if ( debug) {
    printf("  Mod_RateSend: %s\n", request);
  }

  /*
    Send the vector of floats to PCR Cmd server
  */
  charptr = (char *)rate;
  status = PCR_Write( Info->fd, charptr, num_values*sizeof(float));
  if ( status ) {
    strcpy( errorMsg, "Mod_RateSend: Error in PCR_Write sending rate");
    printf("%s\n", errorMsg);
    free(rate);
    return (status);
  }

  free(rate);
  return(0);
}
