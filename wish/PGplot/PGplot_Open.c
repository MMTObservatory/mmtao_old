/*
  PGplot_Open.c

  Open a pgplot device.  The id of the device is stored in the PGplot_Info
    structure.  The plot itself is create in other routines.

  Written  28mar06  Copied example program from
                      http://www.astro.caltech.edu/~tjp/pgplot/tkdriv.html

  Altered  29mar06  Modified so PGplot_Open is no long called directly from tcl
                      but a C routine that is called by PGplot_Cmd.
*/

#include "PCR_Wish.h"

#include "PGplot.h"

int PGplot_Open( PGplot_Info *Info, char *path, int debug)
{
  int id;
  /*
   * Attempt to open the PGPLOT device specified in argv[1].
   */
  id = cpgopen( path);
  if(id <= 0) {
    return(-1);
  };
  Info->id = id;
  strcpy( Info->path, path);

  if ( debug ) {
    printf("  PGplot_Open: Device id = %d\n", Info->id);
    printf("                    Path = %s\n", Info->path);
  }

  /*
   * Turn off new-page prompting.
   */
  cpgask(0);

  return(0);
}
