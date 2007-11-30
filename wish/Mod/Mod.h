/*
  Definitions to be include in all Mod routines

  Written 15jan07  DLM

*/

#define NUM_SLOPES 288
#define NUM_STATIC_ZERN 7

#include "Client.h"
#include "Server.h"

/* Prototypes */
int Mod_Calc( long mode, float *matrix, float *offsets, int debug, char *replyData);
int Mod_ReadMatrix( float *mod_matrix, char *filename, int debug, char *replyData);
int Mod_OffsetsSend( Server_Info *Info, float *offsets, int debug, char *errorMsg);
int Mod_RateSend( Server_Info *Info, char *fileName, int debug, char *errorMsg);
