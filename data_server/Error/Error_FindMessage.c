/*=======================================================================
 * Error_FindMessage.c
 *=======================================================================

  This routine returns the associated error message of the passed error number
    in the ErrorTable structure.  This error message can be to display the
    appropriate error message to the user.

  The ErrorTable must be consistent with the Error Numbers used by the
    the PCR.  Thus, Vidhya provides update to the ErrorTable and hash
    table.

  Written  27mar07  DLM  Started with Info_FindParameter

*/

#include "PCR.h"
#include "PCR_Error.h"
#include "PCR_ErrorTable.h"

/*================================================================================*
 * Return the position of the error number in the ErrorTable struct
 *================================================================================*/
int Error_FindMessage( int errorNumber, char *errorMessage, int debug)
{

  /* local variables */

  int i;
  int ErrorTableLen = sizeof(ErrorTable)/sizeof(ErrorList);

  /*
    Search for requested errror nubmer in structure list.  When it is found, return the
      associated error message to the calling routine.
  */
  for ( i=0; i<ErrorTableLen; i++ ) {
    if ( errorNumber == ErrorTable[i].pcr_code ) {
      strcpy( errorMessage, ErrorTable[i].errmsg);
      if( debug ) {
	printf("  Error_FindMessage: Error Number %d: %s\n", errorNumber, errorMessage);
	fflush(stdout);
      }
      return(0);
    }
  }

  printf("  Error_FindMessage: Error Number %d not found in ErrorTable structure\n", errorNumber);
  fflush(stdout);

  return(-1);
}
