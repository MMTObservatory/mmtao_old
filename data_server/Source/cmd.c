
#include "PCR.h"

int Source_Start( char *name, char *type, int size, int debug );

int main()
{
  int status;
  status = Source_Start( "CMD", "none", 0, 1);
  return (0);
}
