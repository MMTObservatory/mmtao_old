/*=======================================================================
 * Status_ParameterPositions.c
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
int Status_ParameterPositions( Info_Struct *Info, char *Status_Reply, int **Status_Position,
			       int *Status_Count, int debug)
{

  /* local variables */
  int i, j;
  Info_Entry *List;
  int list_length;

  char reply[STATUS_REPLY_LENGTH];
  char *next = NULL;
  char *save_ptr;

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
    Determine how many parameters were sent
  */
  *Status_Count = 0;
  next = strtok_r(reply, "\n", &save_ptr);
  while ( next != NULL ) {

    (*Status_Count)++;
    next = strtok_r(NULL, "\n", &save_ptr);

  }
  (*Status_Count)--;

  if( debug ) {
    printf("  Status_ParameterPositions: Number of parameters found = %d\n", *Status_Count);
    fflush(stdout);
  }

  /*
    Allocate the Status_Position array
  */
  *Status_Position = (int *)malloc( (*Status_Count)*sizeof(int) );
  
  /*
    Set all position values to -1 so if a parameter is not found
      it will be skipped later values are being stored
  */
  for ( j=0; j<(*Status_Count); j++ ) {
    (*Status_Position)[j] = -1;
  }

  /*
    Copy Status_Reply into an auxilary character string because
      the call to strtok() will destroy the passed string
  */
  strcpy( reply, Status_Reply);

  /*
    Split the parameter name and the value and find the position of the
      parameter.  Save it in the Status_Position array
  */
  i = 0;
  next = strtok_r(reply, " ", &save_ptr);
  for ( j=0; j<(*Status_Count); j++ ) {

    /*
      Search for request in structure list.  When it is found, return its
        position to the calling routine.
    */
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( next, List[i].name, strlen(List[i].name) ) ) {
	(*Status_Position)[j] = i;
	break;
      }
    }

    /*
      Report any parameter passed from the PCR that were not found in the 
        InfoList by checking for a -1 in the Status_Position array
    */
    if ( Status_Position[j] < 0 ) {
      printf("  Status_ParameterPositions: Error: Parameter `%s` not found in InfoList structure\n", next);
      fflush(stdout);
    }      

    if( debug ) {
      printf("  Status_ParameterPositions: %s\n", next );
      printf("                             Position = %d\n", (*Status_Position)[j] );
      fflush(stdout);
    }

    next = strtok_r(NULL, "\n", &save_ptr);
    next = strtok_r(NULL, " ", &save_ptr);

  }

  return(0);
}
