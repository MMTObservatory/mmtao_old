/*
  DServ_InitSlopes.c

  This routine initializes the DServ_Data structure need to collect
    and pass data from the PC-Reconstructor (PCR) and allocate the
    memory needed for the data storage buffer.

  Written  20nov05  DLM

  Altered   2may06  DLM  Added frameSize and frameBytes to the DServ_Data structure

  Altered  29jun06  DLM  Changed DServ.h to include DM data sizes and offsets
                         Started with generic DServ_DataInit.c, which was used for
			   for both WFSC and DM data.  This routine is specifically
                           for DM data.
*/

#include "PCR.h"
#include "DServ.h"

/*================================================================================*
 * Set up data structure and memory for DM data
 *================================================================================*/
int DServ_InitSlopes( DServ_Data *Data, int debug)
{
  int i;

  Data->num_frames = NUM_SLOPES_FRAMES;
  Data->header_bytes = HEADER_BYTES;
  Data->slope_offset = HEADER_BYTES;
  Data->slope_bytes = SLOPE_BYTES;
  Data->total_bytes = HEADER_BYTES + SLOPE_BYTES;


  Data->buffer = (char *)malloc( Data->num_frames * Data->total_bytes);
  Data->framePtr = (char **)malloc( Data->num_frames*sizeof(char *) );
  Data->nextFrame = (int *)malloc( Data->num_frames*sizeof(int) );

  Data->framePtr[0] = Data->buffer;

  if ( debug ) {
    printf("  Debug information from DServ_InitSlopes\n");
    printf("    Data->num_frames   = %d\n", Data->num_frames);
    printf("    Data->header_bytes = %d\n", Data->header_bytes);
    printf("    Data->slope_offset = %d\n", Data->slope_offset);
    printf("    Data->slope_bytes  = %d\n", Data->slope_bytes);
    printf("    Data->total_bytes  = %d\n", Data->total_bytes);
    fflush(stdout);
  }

  for ( i=1; i<Data->num_frames; i++) {
    Data->nextFrame[i-1] = i;
    Data->framePtr[i] = Data->framePtr[i-1] + Data->total_bytes;
  }
  Data->nextFrame[i-1] = 0;

  Data->active = 0;
  Data->dataPtr = Data->framePtr[Data->active];
  Data->last = -1;

  return(0);
}
