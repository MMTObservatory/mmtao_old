/*
  Definitions to be include in all Sky routines

  Written 13apr04  DLM

*/

#define NUM_SLOPES 288
#define NUM_STATIC_ZERN 7

/* Prototypes */
int Static_Calc( float *scale, float *matrix, float *offsets, int debug, char *replyData);
int Static_ReadMatrix( float *soff_matrix, char *filename, int debug, char *replyData);
int Static_Send( Server_Info *Info, float *offsets, float *scale, int debug, char *errorMsg);
