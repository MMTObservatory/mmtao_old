/*=======================================================================
 * Status_ReadFile.c
 *=======================================================================

  Read an Status Parameter setup file from disk and puts these default
    values into the Status structure.

  Written  13aug07  DLM  Started with Info_ReadFile.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"

/*================================================================================*
 * Read an Status Parameter setup file from disk
 *================================================================================*/
int Status_ReadFile( Status_Entry *List, long list_length, char *filename, long debug)
{

  /* local variables */

  FILE *fd;
  char *infoName;
  char *infoValue;
  int i, type, found;
  char buffer[STRING_LENGTH];

  /*
    Open file containing parameter and data file names
  */

  fd = fopen(filename, "r");
  if (!fd){
    printf("  Status_ReadFile: Could not open filename: %s\n", filename);
    return(-1);
  }

  /*
    Read in all the lines in the file.  Comments (#) and blank lines are
      ignored
  */

  while( !feof(fd) ) {

    if (Status_ReadLine(fd, buffer)==0) continue;
    infoName = (char *)strtok(buffer, " \t");
    infoValue = (char *)strtok(NULL, " ");
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
	    printf("  Status_ReadFile: Integer %s %d\n", List[i].name, List[i].i_value);
	  }
	  break;
	case 'I':
	  *(List[i].i_ptr) = atoi(infoValue);
	  if( debug ){
	    printf("  Status_ReadFile: Integer Pointer %s %d\n", List[i].name, *(List[i].i_ptr));
	  }
	  break;
	case 'f':
	  List[i].f_value = atof(infoValue);
	  if( debug ){
	    printf("  Status_ReadFile: Float %s %f\n", List[i].name, List[i].f_value);
	  }
	  break;
	case 'F':
	  *(List[i].f_ptr) = atof(infoValue);
	  if( debug ){
	    printf("  Status_ReadFile: Float %s %f\n", List[i].name, *(List[i].f_ptr));
	  }
	  break;
	case 's':
	  strcpy(List[i].s_value, infoValue);
	  if( debug ){
	    printf("  Status_ReadFile: String %s %s\n", List[i].name, List[i].s_value);
	  }
	  break;
	case 'S':
	  strcpy(List[i].s_ptr, infoValue);
	  if( debug ){
	    printf("  Status_ReadFile: String %s %s\n", List[i].name, List[i].s_ptr);
	  }
	  break;
	default:
	  printf(" Status_ReadFile: Error: Parameter type not found for %s =>%s\n",
		 List[i].name, List[i].type);
	  return(-1);
	}
	found=1;
	break;
      }
    }
    if ( !found ) {
      printf("    Error in Status_ReadFile: \"%s\" parameter name not found in Status_List\n",
	     infoName);
    }
  }

  fclose(fd);
  return(0);
}
