/*
  PGplot_SpotsPattern.c

  This routine creates a 2-D plot with the given parameters.

  Written  16may07  DLM

*/

#include "PCR_Wish.h"

#include "PGplot.h"

int PGplot_SpotsPattern( PGplot_Info *Info, short **WFSC_Array, int debug)
{

  int i;

  /*
    Check to see if memory has already been allocated.  If so, free it
  */
  if ( *WFSC_Array != NULL ) {
    free(*WFSC_Array);
    printf("  PGplot_SpotsPattern: WFSC_Array memory freed\n");
  }

  /*
    Allocate the needed memory.  We create a short array here so
      that the pattern is the same as the array returned by
      DClient_WFSC_Single().
  */
  if (( *WFSC_Array = (short *)malloc(Info->spots->values*Info->spots->values*sizeof(short)) ) == NULL ) {
    printf("  PGplot_SpotsPattern: Error allocating WFSC_Array\n");
    return(-1);
  }

  /*
    Create an image
  */
  for(i=0; i<Info->spots->values*Info->spots->values; i+=2) {
    //(*WFSC_Array)[i] = 600.;
    (*WFSC_Array)[i] = i;
    //printf("%d\n", (*WFSC_Array)[i]);
  }
  for(i=1; i<Info->spots->values*Info->spots->values; i+=2) {
    //(*WFSC_Array)[i] = 600.;
    (*WFSC_Array)[i] = Info->spots->values*Info->spots->values - i;
    //printf("%d\n", (*WFSC_Array)[i]);
  }

  return(0);
}
