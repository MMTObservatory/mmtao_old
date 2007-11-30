/*
  DServ_InitWFSC.c

  This routine initializes the DServ_Data structure need to collect
    and pass data from the PC-Reconstructor (PCR) and allocate the
    memory needed for the data storage buffer.

  Written  20nov05  DLM

  Altered   2may06  DLM  Added frameSize and frameBytes to the DServ_Data structure

  Altered  12jul06  DLM  Changed name from DServ_DataInit.c
*/

#include "PCR.h"
#include "DServ.h"

/*================================================================================*
 * Set up data structure and memory
 *================================================================================*/
int DServ_InitWFSC( DServ_Data *Data, int debug )
{
  int i;

  Data->num_frames = NUM_WFSC_FRAMES;
  Data->header_bytes = HEADER_BYTES;
  Data->wfsc_offset = WFSC_OFFSET;
  /*
    These last two parameteres are filled once the number of bytes (nBytes) is read
      from the PCR server.  From this value we can determine the binning of the camera,
      the size of the frame we will recieve from the PCR WFSC server (Data->total_bytes)
      and how much data we will save in the DServ memery (Data->wfsc_bytes).
  */
  Data->wfsc_bytes = 0;
  Data->total_bytes = 0;

  /*
    Create a memory buffer large enough to hold the largest possible WFSC
      frame (72x72)
  */
  Data->buffer = (char *)malloc( Data->num_frames*TOTAL_WFSC_BYTES );
  Data->framePtr = (char **)malloc( Data->num_frames*sizeof(char *) );
  Data->nextFrame = (int *)malloc( Data->num_frames*sizeof(int) );

  Data->framePtr[0] = Data->buffer;

  if ( debug ) {
    printf("  Debug information from DServ_InitWFSC\n");
    printf("    Data->num_frames = %d\n", Data->num_frames);
    printf("    Data->header_bytes = %d\n", Data->header_bytes);
    printf("    Data->wfsc_offset  = %d\n", Data->wfsc_offset);
    printf("    Data->wfsc_bytes   = %d\n", Data->wfsc_bytes);
    printf("    Data->total_bytes  = %d\n", Data->total_bytes);
    fflush(stdout);
  }

  for ( i=1; i<Data->num_frames; i++) {
    Data->nextFrame[i-1] = i;
    Data->framePtr[i] = Data->framePtr[i-1] + TOTAL_WFSC_BYTES;
  }
  Data->nextFrame[i-1] = 0;

  Data->active = 0;
  Data->dataPtr = Data->framePtr[Data->active];
  Data->last = -1;

  return(0);
}
