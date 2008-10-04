/*
  Background Routine Definitions

  Written  6feb07  DLM
*/

/* Prototypes */
int BG_Acquire( DClient_Info *BG_Info, char *fname, int frames, int debug, char *errorMsg);
int BG_Send( Server_Info *Info, char *fileName, int debug, char *errorMsg);
