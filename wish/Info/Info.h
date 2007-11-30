/*
  Generic Info Client definitions

  Written  22jun05  DLM
*/

#include "Client.h"
#include "Server.h"

/* 
  Info array size.  This is used for seeing values and strehl values
*/
#define SEEING_VALUES     10

/*
  Number of connections to Info server possible
*/
#define INFO_CONNECTIONS 20

/* Prototypes */
int Info_AverageSeeing( char *Info_Reply, double *Seeing_Average, char *errorMsg, int debug);
