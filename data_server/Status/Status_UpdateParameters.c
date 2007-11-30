/*=======================================================================
 * Status_UpdateParameters.c
 *=======================================================================

  This routine returns to the calling routine a string containing the
    the name of the requested Info Parameter and it value separated by
    a space.

  Written  17may05  DLM  Started with SInfo_FileNameSave.c

  Altered  19nov06  DLM  Modified to work with DServ for the PCR

*/

#include "PCR.h"
#include "DServ.h"
#include "Status.h"

/*================================================================================*
 * Return an Info Parameter and its value
 *================================================================================*/
int Status_UpdateParameters( Info_Struct *Info, char *Status_Reply, long *Status_Position,
			     long Status_Count, long debug)
{

  /* local variables */
  int i, j, type;
  Info_Entry *List;
  long list_length;

  char reply[STATUS_REPLY_LENGTH];
  char *infoParameter = NULL;
  char *infoValue = NULL;

  /*
    Setup Info information
  */
  List = Info->list;
  list_length = Info->length;

  /*
    Copy Status_Reply into an auxilary character string because
      the call to strtok() will destroy the passed string
  */
  strcpy( reply, Status_Reply);

  /*
    Split the parameter name and the value and find the position of the
      parameter.  Save it in the Status_Position array
  */
  infoParameter = strtok(reply, " ");
  infoValue = strtok(NULL, "\n");

  for ( j=0; j<Status_Count; j++ ) {

    i = Status_Position[j];

    /*
      Make sure this passed parameter was found in InfoList.  If so, then
        put the passed value into the correct place in the structure according
	to the type of parameter, defined in InfoList.h
    */
    if ( i >= 0 ) {

      type = List[i].type[0];

      switch (type) {
      case 'i':
	List[i].i_value = atoi(infoValue);
	if( debug ){
	  printf("  Status_UpdateParameters: Integer  %s %d\n", List[i].name, List[i].i_value);
	  fflush(stdout);
	}
	break;
      case 'I':
	*(List[i].i_ptr) = atoi(infoValue);
	if( debug ){
	  printf("  Status_UpdateParameters: *Integer %s %d\n", List[i].name, *(List[i].i_ptr));
	  fflush(stdout);
	}
	break;
      case 'f':
	List[i].f_value = atof(infoValue);
	if( debug ){
	  printf("  Status_UpdateParameters: Float    %s %f\n", List[i].name, List[i].f_value);
	  fflush(stdout);
	}
	break;
      case 'F':
	*(List[i].f_ptr) = atof(infoValue);
	if( debug ){
	  printf("  Status_UpdateParameters: *Float   %s %f\n", List[i].name, *(List[i].f_ptr));
	  fflush(stdout);
	}
	break;
      case 's':
	strcpy(List[i].s_value, infoValue);
	if( debug ){
	  printf("  Status_UpdateParameters: String   %s %s\n", List[i].name, List[i].s_value);
	  fflush(stdout);
	}
	break;
      default:
	printf(" Status_UpdateParameters: Error: Parameter type not found for %s =>%s\n",
	       List[i].name, List[i].type);
	fflush(stdout);
	break;
      }
    }

    infoParameter = strtok(NULL, " ");
    infoValue = strtok(NULL, "\n");

  }

  return(0);
}
