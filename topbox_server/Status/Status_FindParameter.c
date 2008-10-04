/*=======================================================================
 * Status_FindParameter.c
 *=======================================================================

  This routine returns to the calling routine a string containing the
    the name of the requested Status Parameter and it value separated by
    a space.

  Written  13aug07  DLM  Started with Info_FileParameter.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"

/*================================================================================*
 * Return an Status Parameter and its value
 *================================================================================*/
int Status_FindParameter( Status_Entry *List, int list_length, char *request, int *position, int debug)
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
    printf(" Status_FindParameter: Error: Status Parameter `%s` not found in StatusList structure\n", request);
  }

  return(-1);
}
