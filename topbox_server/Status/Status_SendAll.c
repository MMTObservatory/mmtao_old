/*=======================================================================
 * Status_SendAll.c
 *=======================================================================

  This routine send the client all the Status Parameters

  Written  13aug07  DLM  Started with Info_SendAll.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"

/*================================================================================*
 * Return an Status Parameter and its value
 *================================================================================*/
int Status_SendAll( int sockfd, Status_Entry *List, int list_length)
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
      sprintf(reply, "%s %.2f\n", List[i].name, List[i].f_value );
      break;
    case 'F':
      sprintf(reply, "%s %.2f\n", List[i].name, *(List[i].f_ptr) );
      break;
    case 's':
      sprintf(reply, "%s %s\n", List[i].name, List[i].s_value);
      break;
    case 'S':
      sprintf(reply, "%s %s\n", List[i].name, List[i].s_ptr);
      break;
    }

    /*
      Return this Status Parameter
    */
    status = Socket_StringWrite( sockfd, reply, strlen(reply));
    if ( status != strlen(reply) ) {
      return(-1);
    }
  }

  /*
    Return end of Status Parameter signal
  */
  strcpy( reply, "EOF\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    return(-1);
  }

  return(0);
}
