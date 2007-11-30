/*===========================================================================*

  PGplot_SetupInfo.c

  State all the state values in the structure Info to zero and
    create the TclTk variable names need to update shared varibles.

  Written   29mar06  DLM  Started with Client_SetupInfo.c


**===========================================================================*/

#include "PCR_Wish.h"

#include "PGplot.h"

/*=================================================
 * PGplot_SetupInfo: Setup Info
 *=================================================*/
int PGplot_SetupInfo( PGplot_Info *Info, char *name, int debug)
{

  /*
    Allocate memory needed for each "Type" of data
  */
  if ( !strncmp( name, "Spots", strlen("Spots")) ) {

    /*
      Allocate Info->spots structure only.  The arrays that are needed are
        allocated in DClient_Spots_Init()
    */
    if ( Info->spots != NULL ) {
      free(Info->spots);
      printf("  PGplot_SetupInfo: Spots_Info->spots memory freed\n");
    }

    if (( Info->spots = (PGplot_Spots_Data *)malloc(sizeof(PGplot_Spots_Data) )) == NULL ) {
      printf("  PGplot_SetupInfo: Error allocating Spots_Info->spots\n");
      return(-1);
    }

    /*
      Initialize all pointers to NULL so when we check to see if memory for these pointers
        we will see that no memory has been allocated and we do not have to perform a free()
      This is not needed under Mac OSX but is for Linux
    */
    Info->spots->current = NULL;
    Info->spots->average = NULL;
    Info->spots->pupil = NULL;
    Info->spots->slope_grid = NULL;
    Info->spots->x_slopes_factor = NULL;
    Info->spots->x_slopes = NULL;
    Info->spots->y_slopes_factor = NULL;
    Info->spots->y_slopes = NULL;
    Info->spots->focus_sin = NULL;
    Info->spots->focus_cos = NULL;
    Info->spots->x_focus = NULL;
    Info->spots->y_focus = NULL;

    /*
      Set initial values

      Image display
    */
    Info->spots->min = 32768.0;
    Info->spots->max = 0.0;
    Info->spots->min_lower = 32768.0;
    Info->spots->max_lower = 0.0;
    Info->spots->min_upper = 32768.0;
    Info->spots->max_upper = 0.0;

    /*
      Pupil display
    */
    Info->spots->min_pupil = 32768.0;
    Info->spots->max_pupil = 0.0;
    Info->spots->min_pupil_lower = 32768.0;
    Info->spots->max_pupil_lower = 0.0;
    Info->spots->min_pupil_upper = 32768.0;
    Info->spots->max_pupil_upper = 0.0;

    if ( debug ) {
      printf("  PGplot_SetupInfo: Spots_Info->spots stucture allocated\n");
    }

  } else if ( !strncmp( name, "Slopes", strlen("Slopes")) ) {

    /*
      Allocate Info->slopes structure only.  The arrays that are needed are
        allocated in DClient_Slopes_Init()
    */
    if ( Info->slopes != NULL ) {
      free(Info->slopes);
      printf("  PGplot_SetupInfo: Slopess_Info->slopes memory freed\n");
    }

    if (( Info->slopes = (PGplot_Slopes_Data *)malloc(sizeof(PGplot_Slopes_Data) )) == NULL ) {
      printf("  PGplot_SetupInfo: Error allocating Slopes_Info->slopes\n");
      return(-1);
    }

    /*
      Initialize all pointers to NULL so when we check to see if memory for these pointers
        we will see that no memory has been allocated and we do not have to perform a free()
      This is not needed under Mac OSX but is for Linux
    */
    Info->slopes->current_x = NULL;
    Info->slopes->current_y = NULL;
    Info->slopes->average_x = NULL;
    Info->slopes->average_y = NULL;

    /*
      Set initial values

      Image display
    */
    Info->slopes->min = 32768.0;
    Info->slopes->max = 0.0;
 
    if ( debug ) {
      printf("  PGplot_SetupInfo: Slopes_Info->slopes stucture allocated\n");
    }

  }

  return(0);
}
