/*
  DServ_InitDM.c

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
int DServ_InitDM( DServ_Data *Data, int debug)
{
  int i;

  Data->num_frames = NUM_DM_FRAMES;
  Data->header_bytes = HEADER_BYTES;
  Data->wfsc_offset = WFSC_OFFSET;
  Data->wfsc_bytes = WFSC_BYTES;
  Data->slope_offset = SLOPE_OFFSET;
  Data->slope_bytes = SLOPE_BYTES;
  Data->rtr_offset = RTR_OFFSET;
  Data->rtr_bytes = RTR_BYTES;
  Data->cmd_offset = CMD_OFFSET;
  Data->cmd_bytes = CMD_BYTES;
  Data->cur_offset =CUR_OFFSET;
  Data->cur_bytes = CUR_BYTES;
  Data->pos_offset = POS_OFFSET;
  Data->pos_bytes = POS_BYTES;
  Data->all_offset = ALL_OFFSET;
  Data->all_bytes = ALL_BYTES;
  Data->total_bytes = TOTAL_DM_BYTES;


  Data->buffer = (char *)malloc( Data->num_frames * Data->total_bytes);
  Data->framePtr = (char **)malloc( Data->num_frames*sizeof(char *) );
  Data->nextFrame = (int *)malloc( Data->num_frames*sizeof(int) );

  Data->framePtr[0] = Data->buffer;

  if ( debug ) {
    printf("  Debug information from DServ_InitDM\n");
    printf("    Data->num_frames   = %d\n", Data->num_frames);
    printf("    Data->header_bytes = %d\n", Data->header_bytes);
    printf("    Data->wfsc_offset  = %d\n", Data->wfsc_offset);
    printf("    Data->wfsc_bytes   = %d\n", Data->wfsc_bytes);
    printf("    Data->slope_offset = %d\n", Data->slope_offset);
    printf("    Data->slope_bytes  = %d\n", Data->slope_bytes);
    printf("    Data->rtr_offset   = %d\n", Data->rtr_offset);
    printf("    Data->rtr_bytes    = %d\n", Data->rtr_bytes);
    printf("    Data->cmd_offset   = %d\n", Data->cmd_offset);
    printf("    Data->cmd_bytes    = %d\n", Data->cmd_bytes);
    printf("    Data->cur_offset   = %d\n", Data->cur_offset);
    printf("    Data->cur_bytes    = %d\n", Data->cur_bytes);
    printf("    Data->pos_offset   = %d\n", Data->pos_offset);
    printf("    Data->pos_bytes    = %d\n", Data->pos_bytes);
    printf("    Data->all_offset   = %d\n", Data->all_offset);
    printf("    Data->all_bytes    = %d\n", Data->all_bytes);
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
