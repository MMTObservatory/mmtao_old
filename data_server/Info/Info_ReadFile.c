/*=======================================================================
 * Info_ReadFile.c
 *=======================================================================

  Read an Info Parameter setup file from disk and puts these default
    values into the Info structure.

  Written  17may05  DLM  Started with SInfo_FileNameRead.c

  Altered  19nov06  DLM  Modified to work with DServ for the PCR

*/

#include "PCR.h"
#include "Info.h"

/*================================================================================*
 * Read an Info Parameter setup file from disk
 *================================================================================*/
int Info_ReadFile( Info_Entry *List, int list_length, char *filename, int debug)
{

  /* local variables */

  FILE *fd;
  char *infoName;
  char *infoValue;
  int i, type, found;
  char buffer[STRING_LENGTH];
  char *save_ptr;

  /*
    Open file containing parameter and data file names
  */

  fd = fopen(filename, "r");
  if (!fd){
    printf("  Info_ReadFile: Could not open filename: %s\n", filename);
    fflush(stdout);
    return(-1);
  }

  /*
    Read in all the lines in the file.  Comments (#) and blank lines are
      ignored
  */

  while( !feof(fd) ) {

    if (Info_ReadLine(fd, buffer)==0) continue;
    infoName = (char *)strtok_r(buffer, " \t", &save_ptr);
    infoValue = (char *)strtok_r(NULL, " ", &save_ptr);
    if ( infoName[strlen(infoName)-1] == '\n' ) infoName[strlen(infoName)-1] = '\0';
    infoValue += strspn(infoValue, " \t");
    if ( infoValue[strlen(infoValue)-1] == '\n' ) infoValue[strlen(infoValue)-1] = '\0';
    /*
      Search for infoName in structure list.  When it is found, put the new
        infoValue into the structure.
    */

    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( infoName, List[i].name, strlen(List[i].name)) ) {
	type = List[i].type[0];
	switch (type) {
	case 'i':
	  List[i].i_value = atoi(infoValue);
	  if( debug ){
	    printf("  Info_ReadFile: Integer %s %d\n", List[i].name, List[i].i_value);
	    fflush(stdout);
	  }
	  break;
	case 'I':
	  *(List[i].i_ptr) = atoi(infoValue);
	  if( debug ){
	    printf("  Info_ReadFile: Integer Pointer %s %d\n", List[i].name, *(List[i].i_ptr));
	    fflush(stdout);
	  }
	  break;
	case 'f':
	  List[i].f_value = atof(infoValue);
	  if( debug ){
	    printf("  Info_ReadFile: Float %s %f\n", List[i].name, List[i].f_value);
	    fflush(stdout);
	  }
	  break;
	case 'F':
	  *(List[i].f_ptr) = atof(infoValue);
	  if( debug ){
	    printf("  Info_ReadFile: Float %s %f\n", List[i].name, *(List[i].f_ptr));
	    fflush(stdout);
	  }
	  break;
	case 's':
	  strcpy(List[i].s_value, infoValue);
	  if( debug ){
	    printf("  Info_ReadFile: String %s %s\n", List[i].name, List[i].s_value);
	    fflush(stdout);
	  }
	  break;
	default:
	  printf(" Info_ReadFile: Error: Parameter type not found for %s =>%s\n",
		 List[i].name, List[i].type);
	  fflush(stdout);
	  return(-1);
	}
	found=1;
	break;
      }
    }
    if ( !found ) {
      printf("    Error in Info_ReadFile: \"%s\" parameter name not found in Info_List\n",
	     infoName);
      fflush(stdout);
    }
  }

  fclose(fd);
  return(0);
}
