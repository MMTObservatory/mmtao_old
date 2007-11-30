/*=======================================================================
 * Error_PlaySound.c
 *=======================================================================

  This routine plays the requested sound (may be just bells) to inform
    the user of a change in state of the system.

  Written  29mar07  DLM

*/

#include "PCR.h"

/*================================================================================*
 * Play the sound
 *================================================================================*/
int Error_PlaySound( long debug)
{

  /* local variables */

  printf("\a");
  fflush(stdout);
  usleep(500000);
  printf("\a");
  fflush(stdout);
  usleep(500000);
  printf("\a");
  fflush(stdout);
  usleep(200000);
  printf("\a");
  fflush(stdout);
  usleep(200000);
  printf("\a");
  fflush(stdout);
  usleep(200000);
  printf("\a");
  fflush(stdout);
  usleep(200000);
  printf("\a");
  fflush(stdout);
  

  return(0);
}
