/*=======================================================================
 * Status_ReadLine.c
 *=======================================================================
 
 Read lines from a file
   Reads lines from a file and searches for a comment marker (#) or a blank
   line.  If any of these found, this line is ignore and another is read.
   Otherwise, this line is returned to the calling routine


  Written  13aug07  DLM  Started with Info_ReadLine.c from pcr_server

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"

/*==========================================================================
 * Read a line from file.
 *==========================================================================*/
int Status_ReadLine(FILE *f, char *header)
{
  char sbuffer[512], *ptr;
  char *save_ptr;
  while(!feof(f)){
    fgets(sbuffer, 511, f);
    strcpy(header, sbuffer);
    ptr = (char *) strtok_r(sbuffer, ";, ", &save_ptr);
    if (feof(f)) strcpy(header, "\0");
    if (ptr==NULL) continue;
    if (*ptr=='\n') continue;
    if (*ptr=='#') continue;
    return(strlen(header));
  }
  return(0);
}
