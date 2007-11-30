/*=======================================================================
 * Info_SendAll.c
 *=======================================================================

  This routine send the client all the Info Parameters

  Written  20may05  DLM  Started with Info_Get.c

  Altered  19nov06  DLM  Modified to work with DServ for the PCR

*/

#include "PCR.h"
#include "Info.h"

/*================================================================================*
 * Return an Info Parameter and its value
 *================================================================================*/
int Info_SendAll( int sockfd, Info_Entry *List, long list_length)
{

  /* local variables */

  char reply[STRING_LENGTH];
  int i, status;

  /*
    Search for request in structure list.  When it is found, return the
      value to the calling routine.
  */
  for ( i=0; i<list_length; i++ ) {

    /*
      Create each reply string containing the parameter and its value
    */
    switch (List[i].type[0]) {
    case 'i':
      sprintf(reply, "%s %d\n", List[i].name, List[i].i_value );
      break;
    case 'I':
      sprintf(reply, "%s %d\n", List[i].name, *(List[i].i_ptr) );
      break;
    case 'f':
      sprintf(reply, "%s %f\n", List[i].name, List[i].f_value );
      break;
    case 'F':
      sprintf(reply, "%s %f\n", List[i].name, *(List[i].f_ptr) );
      break;
    case 's':
      sprintf(reply, "%s %s\n", List[i].name, List[i].s_value);
      break;
    }

    /*
      Return this Info Parameter
    */
    status = Socket_StringWrite( sockfd, reply, strlen(reply));
    if ( status != strlen(reply) ) {
      return(-1);
    }
  }

  /*
    Return end of Info Parameter signal
  */
  strcpy( reply, "EOF\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  return(0);
}
