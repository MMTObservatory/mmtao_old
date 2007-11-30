/*
  DClient_Spots_Init.c

  Routine to request open a socket to the DClient server on the VME, start
    the requested type of WFSC server (Single, Save) and allocate the memory
    needed to transfer and manipulate the requested data.

  An actual Spots frame is not retrieved here

  Written  23mar05  DLM  Combined DClient_WFSC_Single, DClient_WFSC_Init,
                           and added Server_SetupState and Server_Open to make
			   the one routine.

			 This can be called by either the Single or the Save routines

*/

#include "PCR_Wish.h"
#include "DClient.h"
#include "WFSC.h"

int DClient_Spots_Init( DClient_Info *Client_Info, PGplot_Info *Spots_Info, char *name,
			short **WFSC_Array, int debug, char *errorMsg)
{

  int status;
  char msg[STRING_LENGTH];
  char request[STRING_LENGTH];

  int sub_offset, pixel_offset;
  int h, i, j, k;
  int sub_count;
  float column, row, distance;

  /*
    Setup Server information in structure, if not already connected
  */
  if ( !Client_Info->server.connected ) {

    status = Server_SetupState( (Server_Info *)Client_Info, name, debug);
    if ( status ) {
      strcpy( errorMsg, "DClient_Spots_Init: Error in Server_SetupState");
      if ( debug ) printf("  %s\n", errorMsg);
      return(status);
    }

    status = Server_Open( (Server_Info *)Client_Info, debug);
    if ( status == -12 ) {
      sprintf( errorMsg, "DClient_Spots_Init: Could not connect to %s server", Client_Info->server.name);
      if ( debug ) printf("  %s\n", errorMsg);
      return(status);
    } else if ( status ) {
      sprintf( errorMsg, "DClient_Spots_Init: Error connecting to %s server", Client_Info->server.name);
      if ( debug ) printf("  %s\n", errorMsg);
      return(status);
    }

  }

  /*
    When information about the Spot array has returned from the WFC server,
      local arrays and parameters are set.  These setting do not change while
      spot data is being retrieved by this process.  In this way, if the
      Spots image is changed from 24x24 to 72x72, the next client and server
      are setup with the proper parameters
  */

  strcpy( request, name);
  status = Server_StringLowerCase(request);

  if ( !strncmp( request, "spots", strlen(request)) ) {

    status = DClient_WriteString( Client_Info->server.fd, "Size", msg);
    if ( status ) {
      sprintf( errorMsg, "DClient_Spots_Init: Could not initialize %s server", Client_Info->server.name);
      printf("%s\n", errorMsg);
      return(status);
    }

    Client_Info->server.nBytes = DClient_ReadLong( Client_Info->server.fd, Client_Info->server.name, msg);

  } else {

    Client_Info->server.nBytes = DClient_ReadLong( Client_Info->server.fd, "Size", msg);

  }

  if ( debug ) {
    printf("  DClient_Spots_Init: nBytes = %d\n", Client_Info->server.nBytes);
  }

  if ( Client_Info->server.nBytes == 1152 ) {
    Spots_Info->spots->values = 24;
    Spots_Info->spots->pixels_per_sub = 2;
  } else if ( Client_Info->server.nBytes == 10368 ) {
    Spots_Info->spots->values = 72;
    Spots_Info->spots->pixels_per_sub = 6;
  } else {
    printf("  DClient_Spots_Init: Unknown nBytes = %d\n", Client_Info->server.nBytes);
    printf("                     Expect 1152 or 10368\n");
    return(-1);
  }

  /*
    Check to see if memory has already been allocated.  If so, free it
  */
  if ( Client_Info->server.in_line != NULL) {
    if ( debug ) printf("  DClient_Spots_Init: Freeing Client_Info->server.in_line\n");
    free(Client_Info->server.in_line);
  }

  if (( Client_Info->server.in_line = (char *) malloc (Client_Info->server.nBytes) ) == NULL) {
    strcpy( errorMsg, "DClient_Spots_Init: Error allocating Client_Info->server.in_line");
    printf("  %s\n", errorMsg);
    return(-1);
  }
  memset( Client_Info->server.in_line, 0, Client_Info->server.nBytes );

  strcpy( Client_Info->server.in_line, "Hello\n");
 
  /*
    Check to see if memory has already been allocated.  If so, free it
  */
  if ( *WFSC_Array != NULL) {
    if ( debug ) printf("  DClient_Spots_Init: Freeing WFSC_Array\n");
    free(*WFSC_Array);
  }

  if (( *WFSC_Array = (short *) malloc (Client_Info->server.nBytes) ) == NULL) {
    strcpy( errorMsg, "DClient_Spots_Init: Error allocating WFSC_Array");
    printf("  %s\n", errorMsg);
    return(-1);
  }
  memset( *WFSC_Array, 0, Client_Info->server.nBytes );

  /*
    Allocate float arrays needed by the PGplot routines
  */
  if ( debug ) printf("  DClient_Spots_Init: Allocating Spots_Info->spots->current\n");
  if ( Spots_Info->spots->current != NULL ) {
    free(Spots_Info->spots->current);
    printf("  DClient_Spots_Init: Spots_Info->spots->current memory freed\n");
  }

  if (( Spots_Info->spots->current = (float *)malloc(Spots_Info->spots->values*Spots_Info->spots->values*sizeof(float)) ) == NULL ) {
    printf("  DClient_Spots_Init: Error allocating Spots_Info->spots->current\n");
    return(-1);
  }
  memset( Spots_Info->spots->current, 0, Spots_Info->spots->values*Spots_Info->spots->values*sizeof(float) );

  /*
    Array for the running average
  */
  if ( debug ) printf("  DClient_Spots_Init: Allocating Spots_Info->spots->average\n");
  if ( Spots_Info->spots->average != NULL ) {
    free(Spots_Info->spots->average);
    printf("  DClient_Spots_Init: Spots_Info->spots->average memory freed\n");
  }

  if (( Spots_Info->spots->average = (float *)malloc(Spots_Info->spots->values*Spots_Info->spots->values*sizeof(float)) ) == NULL ) {
    printf("  DClient_Spots_Init: Error allocating Spots_Info->spots->average\n");
    return(-1);
  }
  memset( Spots_Info->spots->average, 0, Spots_Info->spots->values*Spots_Info->spots->values*sizeof(float) );

  /*
    Fractional values of current frame to add to the running average.  Here we
      set it to 1.0 so the first time through the runnning average is the current frame.
    This value is reset to a reasonable value (~0.2) in PGplot_SpotsCalcualte()
  */
  Spots_Info->spots->average_fraction = 1.0;

  /*
    Array to hold the pupil image.  The average value of the four quadrants is displayed
  */
  if ( debug ) printf("  DClient_Spots_Init: Allocating Spots_Info->spots->pupil\n");
  if ( Spots_Info->spots->pupil != NULL ) {
    free(Spots_Info->spots->pupil);
    printf("  DClient_Spots_Init: Spots_Info->spots->pupil memory freed\n");
  }

  if (( Spots_Info->spots->pupil = (float *)malloc(SUB_APPS*SUB_APPS*sizeof(float)) ) == NULL ) {
    printf("  DClient_Spots_Init: Error allocating Spots_Info->spots->pupil\n");
    return(-1);
  }
  memset( Spots_Info->spots->pupil, 0, SUB_APPS*SUB_APPS*sizeof(float) );

  /*
    Allocate integer array that holds which sub-app the specified pixel belongs (24x24 or 72x72 long)
  */
  if ( debug ) printf("  DClient_Spots_Init: Allocating Spots_Info->spots->slope_grid\n");
  if ( Spots_Info->spots->slope_grid != NULL ) {
    free(Spots_Info->spots->slope_grid);
    printf("  DClient_Spots_Init: Spots_Info->spots->slope_grid memory freed\n");
  }

  if (( Spots_Info->spots->slope_grid = (int *)malloc(Spots_Info->spots->values*Spots_Info->spots->values*sizeof(int)) ) == NULL ) {
    printf("  DClient_Spots_Init: Error allocating Spots_Info->spots->slope_grid\n");
    return(-1);
  }
  memset( Spots_Info->spots->slope_grid, 0, Spots_Info->spots->values*Spots_Info->spots->values*sizeof(int) );

  /*
    Allocate integer array that holds the factor needed in the X slope calculation (24x24 or 72x72 long)
  */
  if ( debug ) printf("  DClient_Spots_Init: Allocating Spots_Info->spots->x_slopes_factor\n");
  if ( Spots_Info->spots->x_slopes_factor != NULL ) {
    free(Spots_Info->spots->x_slopes_factor);
    printf("  DClient_Spots_Init: Spots_Info->spots->x_slopes_factor memory freed\n");
  }

  if (( Spots_Info->spots->x_slopes_factor = (float *)malloc(Spots_Info->spots->values*Spots_Info->spots->values*sizeof(float)) ) == NULL ) {
    printf("  DClient_Spots_Init: Error allocating Spots_Info->spots->x_slopes_factor\n");
    return(-1);
  }
  memset( Spots_Info->spots->x_slopes_factor, 0, Spots_Info->spots->values*Spots_Info->spots->values*sizeof(float) );

  /*
    Allocate integer array that holds the factor needed in the Y slope calculation (24x24 or 72x72 long)
  */
  if ( debug ) printf("  DClient_Spots_Init: Allocating Spots_Info->spots->y_slopes_factor\n");
  if ( Spots_Info->spots->y_slopes_factor != NULL ) {
    free(Spots_Info->spots->y_slopes_factor);
    printf("  DClient_Spots_Init: Spots_Info->spots->y_slopes_factor memory freed\n");
  }

  if (( Spots_Info->spots->y_slopes_factor = (float *)malloc(Spots_Info->spots->values*Spots_Info->spots->values*sizeof(float)) ) == NULL ) {
    printf("  DClient_Spots_Init: Error allocating Spots_Info->spots->y_slopes_factor\n");
    return(-1);
  }
  memset( Spots_Info->spots->y_slopes_factor, 0, Spots_Info->spots->values*Spots_Info->spots->values*sizeof(float) );

  /*
    Array to hold the X slopes
  */
  if ( debug ) printf("  DClient_Spots_Init: Allocating Spots_Info->spots->x_slopes\n");
  if ( Spots_Info->spots->x_slopes != NULL ) {
    free(Spots_Info->spots->x_slopes);
    printf("  DClient_Spots_Init: Spots_Info->spots->x_slopes memory freed\n");
  }

  if (( Spots_Info->spots->x_slopes = (float *)malloc(SUB_APPS*SUB_APPS*sizeof(float)) ) == NULL ) {
    printf("  DClient_Spots_Init: Error allocating Spots_Info->spots->x_slopes\n");
    return(-1);
  }
  memset( Spots_Info->spots->x_slopes, 0, SUB_APPS*SUB_APPS*sizeof(float) );

  /*
    Array to hold the Y slopes
  */
  if ( debug ) printf("  DClient_Spots_Init: Allocating Spots_Info->spots->y_slopes\n");
  if ( Spots_Info->spots->y_slopes != NULL ) {
    free(Spots_Info->spots->y_slopes);
    printf("  DClient_Spots_Init: Spots_Info->spots->y_slopes memory freed\n");
  }

  if (( Spots_Info->spots->y_slopes = (float *)malloc(SUB_APPS*SUB_APPS*sizeof(float)) ) == NULL ) {
    printf("  DClient_Spots_Init: Error allocating Spots_Info->spots->y_slopes\n");
    return(-1);
  }
  memset( Spots_Info->spots->y_slopes, 0, SUB_APPS*SUB_APPS*sizeof(float) );

  /*
    Array to hold the focus factors
  */
  if ( Spots_Info->spots->focus_sin != NULL ) {
    free(Spots_Info->spots->focus_sin);
    printf("  DClient_Spots_Init: Spots_Info->spots->focus_sin memory freed\n");
  }

  if (( Spots_Info->spots->focus_sin = (float *)malloc(SUB_APPS*SUB_APPS*sizeof(float)) ) == NULL ) {
    printf("  DClient_Spots_Init: Error allocating Spots_Info->spots->focus_sin\n");
    return(-1);
  }
  memset( Spots_Info->spots->focus_sin, 0, SUB_APPS*SUB_APPS*sizeof(float) );

  if ( Spots_Info->spots->focus_cos != NULL ) {
    free(Spots_Info->spots->focus_cos);
    printf("  DClient_Spots_Init: Spots_Info->spots->focus_cos memory freed\n");
  }

  if (( Spots_Info->spots->focus_cos = (float *)malloc(SUB_APPS*SUB_APPS*sizeof(float)) ) == NULL ) {
    printf("  DClient_Spots_Init: Error allocating Spots_Info->spots->focus_cos\n");
    return(-1);
  }
  memset( Spots_Info->spots->focus_cos, 0, SUB_APPS*SUB_APPS*sizeof(float) );

  /*
    Array to hold the x and y focus components
  */
  if ( Spots_Info->spots->x_focus != NULL ) {
    free(Spots_Info->spots->x_focus);
    printf("  DClient_Spots_Init: Spots_Info->spots->x_focus memory freed\n");
  }

  if (( Spots_Info->spots->x_focus = (float *)malloc(SUB_APPS*SUB_APPS*sizeof(float)) ) == NULL ) {
    printf("  DClient_Spots_Init: Error allocating Spots_Info->spots->x_focus\n");
    return(-1);
  }
  memset( Spots_Info->spots->x_focus, 0, SUB_APPS*SUB_APPS*sizeof(float) );

  /*
    Array to hold the x and y focus components
  */
  if ( Spots_Info->spots->y_focus != NULL ) {
    free(Spots_Info->spots->y_focus);
    printf("  DClient_Spots_Init: Spots_Info->spots->y_focus memory freed\n");
  }

  if (( Spots_Info->spots->y_focus = (float *)malloc(SUB_APPS*SUB_APPS*sizeof(float)) ) == NULL ) {
    printf("  DClient_Spots_Init: Error allocating Spots_Info->spots->y_focus\n");
    return(-1);
  }
  memset( Spots_Info->spots->y_focus, 0, SUB_APPS*SUB_APPS*sizeof(float) );

  /*
    Setup the grid and factor for this image size.
  */
  if ( debug ) printf("  DClient_Spots_Init: Setup slope grid and factor arrays\n");
  sub_count=0;
  Spots_Info->spots->used_subapps = 0;
  for ( k=0; k<SUB_APPS; k++) {
    for ( j=0; j<SUB_APPS; j++) {
      sub_offset = k*SUB_APPS*Spots_Info->spots->pixels_per_sub*Spots_Info->spots->pixels_per_sub+j*Spots_Info->spots->pixels_per_sub;
      for ( i=0; i<Spots_Info->spots->pixels_per_sub/2; i++) {
	pixel_offset = sub_offset+i*SUB_APPS*Spots_Info->spots->pixels_per_sub;
	for ( h=0; h<Spots_Info->spots->pixels_per_sub/2; h++) {
	  Spots_Info->spots->slope_grid[pixel_offset+h] = sub_count;
	  Spots_Info->spots->x_slopes_factor[pixel_offset+h] = (subapp_used[sub_count] ? -1.0:0.0);
	  Spots_Info->spots->y_slopes_factor[pixel_offset+h] = (subapp_used[sub_count] ? -1.0:0.0);
	  //	  printf( "%d  %d  %f  %f\n", pixel_offset+h, Spots_Info->spots->slope_grid[pixel_offset+h],
	  //		  Spots_Info->spots->x_slopes_factor[pixel_offset+h],  Spots_Info->spots->y_slopes_factor[pixel_offset+h]);
	}
	for ( h=Spots_Info->spots->pixels_per_sub/2; h<Spots_Info->spots->pixels_per_sub; h++) {
	  Spots_Info->spots->slope_grid[pixel_offset+h] = sub_count;
	  Spots_Info->spots->x_slopes_factor[pixel_offset+h] = (subapp_used[sub_count] ? 1.0:0.0);
	  Spots_Info->spots->y_slopes_factor[pixel_offset+h] = (subapp_used[sub_count] ? -1.0:0.0);
	  //	  printf( "%d  %d  %f  %f\n", pixel_offset+h, Spots_Info->spots->slope_grid[pixel_offset+h],
	  //		  Spots_Info->spots->x_slopes_factor[pixel_offset+h],  Spots_Info->spots->y_slopes_factor[pixel_offset+h]);
	}
      }
      for ( i=Spots_Info->spots->pixels_per_sub/2; i<Spots_Info->spots->pixels_per_sub; i++) {
	pixel_offset = sub_offset+i*SUB_APPS*Spots_Info->spots->pixels_per_sub;
	for ( h=0; h<Spots_Info->spots->pixels_per_sub/2; h++) {
	  Spots_Info->spots->slope_grid[pixel_offset+h] = sub_count;
	  Spots_Info->spots->x_slopes_factor[pixel_offset+h] = (subapp_used[sub_count] ? -1.0:0.0);
	  Spots_Info->spots->y_slopes_factor[pixel_offset+h] = (subapp_used[sub_count] ? 1.0:0.0);
	  //	  printf( "%d  %d  %f  %f\n", pixel_offset+h, Spots_Info->spots->slope_grid[pixel_offset+h],
	  //		  Spots_Info->spots->x_slopes_factor[pixel_offset+h],  Spots_Info->spots->y_slopes_factor[pixel_offset+h]);
	}
	for ( h=Spots_Info->spots->pixels_per_sub/2; h<Spots_Info->spots->pixels_per_sub; h++) {
	  Spots_Info->spots->slope_grid[pixel_offset+h] = sub_count;
	  Spots_Info->spots->x_slopes_factor[pixel_offset+h] = (subapp_used[sub_count] ? 1.0:0.0);
	  Spots_Info->spots->y_slopes_factor[pixel_offset+h] = (subapp_used[sub_count] ? 1.0:0.0);
	  //	  printf( "%d  %d  %f  %f\n", pixel_offset+h, Spots_Info->spots->slope_grid[pixel_offset+h],
	  //		  Spots_Info->spots->x_slopes_factor[pixel_offset+h],  Spots_Info->spots->y_slopes_factor[pixel_offset+h]);
	}
      }
      Spots_Info->spots->used_subapps += (subapp_used[sub_count] ? 1.0:0.0);

      /*
	Determine factors needed to determine the component of each slope vector in the
	  focus direction (in or out from the center).  The equations are

	  focus = x_slope * sin(theta) + y_slope * cos(theta)

	  x_focus = focus * sin(theta)
	  y_focus = focus * cos(theta)

          where:
	    x_slope, y_slope: the x,y slopes for a given sub-app
	          sin(theta): column/distance
		  cos(theat): row/distance
	              column: the column number of the sub-app from the center of the array
	                 row: the row number of the sub-app from the center of the array
		      distance: the distance of the sub-app from the center of the array
		                = sqrt( column^2 + row^2 )
      */
      column = (float) j - 5.5;
      row = (float) k - 5.5;
      distance = sqrt(row*row + column*column);
      Spots_Info->spots->focus_sin[sub_count] = column/distance;
      Spots_Info->spots->focus_cos[sub_count] = row/distance;

      sub_count++;
    }
  }

  return(0);
}
