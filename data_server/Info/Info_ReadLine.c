/*=======================================================================
 * Info_ReadLine.c
 *=======================================================================
 
 Read lines from a file
   Reads lines from a file and searches for a comment marker (#) or a blank
   line.  If any of these found, this line is ignore and another is read.
   Otherwise, this line is returned to the calling routine


  Written  20feb02  DLM

  Altered  18may05  DLM   Renamed from Read_File_Header.c

  Altered  19nov06  DLM  Modified to work with DServ for the PCR

*/

#include "PCR.h"
#include "Info.h"

/*==========================================================================
 * Read a line from file.
 *==========================================================================*/
int Info_ReadLine(FILE *f, char *header)
{
  char sbuffer[512], *ptr;
  while(!feof(f)){
    fgets(sbuffer, 511, f);
    strcpy(header, sbuffer);
    ptr = (char *) strtok(sbuffer, ";, ");
    if (feof(f)) strcpy(header, "\0");
    if (ptr==NULL) continue;
    if (*ptr=='\n') continue;
    if (*ptr=='#') continue;
    return(strlen(header));
  }
  return(0);
}
