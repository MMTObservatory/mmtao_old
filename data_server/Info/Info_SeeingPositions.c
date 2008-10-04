/*=======================================================================
 * Info_SeeingPositions.c
 *=======================================================================

  This routine finds the position of the requested Seeing Parameters in the
    Info->list and returns and array with these values along with the number
    of parameters.

  Written  30jun07  DLM  Started with Status_ParameterPositions.c

*/

#include "PCR.h"
#include "Info.h"

/*================================================================================*
 * Return an Info Parameter and its value
 *================================================================================*/
int Info_SeeingPositions( Info_Struct *Info, int **Seeing_Positions,
			       int *Seeing_Count, int debug)
{

  /* Info Parameters to send to seeing monitor */
  char *Seeing_List[] = {
    "WFSC_Period",
    "WFSC_Frequency",
    "Seeing_Modulating",
    "Seeing_Mode_File",
    "Seeing_Total_Modes",
    "Seeing_Mode",
    "Seeing_Scale_Factor",
    "Seeing_Period",
    "Seeing_Frequency",
    "Seeing_Gain_Change",
    "Seeing_Value",
    "Science_Observing",
    "Background_Dynamic",
    "Background_Static",
    "Background_File",
    "Loop_Running",
    "Loop_Gain",
    "Loop_Save_Data"};

  /* local variables */
  int i, j;
  Info_Entry *List;
  int list_length;

  /*
    Setup Info information
  */
  List = Info->list;
  list_length = Info->length;

  /*
    Determine the number of Seeing parameters to send
  */
  *Seeing_Count = sizeof(Seeing_List)/sizeof(char *);

  if( debug ) {
    printf("  Info_SeeingPositions: Number of parameters found = %d\n", *Seeing_Count);
    fflush(stdout);
  }

  /*
    Allocate the Seeing_Positions array
  */
  *Seeing_Positions = (int *)malloc( (*Seeing_Count)*sizeof(int) );
  
  /*
    Set all position values to -1 so if a parameter is not found
      it will be skipped later values are being stored
  */
  for ( j=0; j<(*Seeing_Count); j++ ) {
    (*Seeing_Positions)[j] = -1;
  }

  /*
    Split the parameter name and the value and find the position of the
      parameter.  Save it in the Seeing_Positions array
  */
  for ( j=0; j<(*Seeing_Count); j++ ) {

    /*
      Search for request in structure list.  When it is found, return its
        position to the calling routine.
    */
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Seeing_List[j], List[i].name, strlen(List[i].name) ) ) {
	(*Seeing_Positions)[j] = i;
	break;
      }
    }

    /*
      Report any parameter passed from the PCR that were not found in the 
        InfoList by checking for a -1 in the Seeing_Positions array
    */
    if ( Seeing_Positions[j] < 0 ) {
      printf("  Info_SeeingPositions: Error: Parameter `%s` not found in InfoList structure\n", Seeing_List[j]);
      fflush(stdout);
    }      

    if( debug ) {
      printf("  Info_SeeingPositions: %s\n", Seeing_List[j]);
      printf("                        Position = %d\n", (*Seeing_Positions)[j] );
      fflush(stdout);
    }

  }

  return(0);
}
