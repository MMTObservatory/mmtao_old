/*=======================================================================
 * Info_FindParameter.c
 *=======================================================================

  This routine returns to the calling routine a string containing the
    the name of the requested Info Parameter and it value separated by
    a space.

  Written  17may05  DLM  Started with SInfo_FileNameSave.c

  Altered  19nov06  DLM  Modified to work with DServ for the PCR

*/

#include "PCR.h"
#include "Info.h"

/*================================================================================*
 * Return an Info Parameter and its value
 *================================================================================*/
int Info_FindParameter( Info_Entry *List, int list_length, char *request, int *position, int debug)
{

  /* local variables */

  int i;

  /*
    Search for request in structure list.  When it is found, return its
      position to the calling routine.
  */
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( request, List[i].name, strlen(List[i].name) ) ) {
      *position = i;
      return(0);
    }
  }

  if( debug ){
    printf(" Info_FindParameter: Error: Info Parameter `%s` not found in InfoList structure\n", request);
    fflush(stdout);
  }

  return(-1);
}
