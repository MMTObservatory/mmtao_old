/*=======================================================================
 * Info_SetFwheelPointers.c
 *=======================================================================

  Set several flag pointers in the Status structure to the correct locations

  Written  13aug07  DLM  Started with Info_SetFlagPointers.c from pcr_server

*/

#include "Topbox.h"
#include "Socket.h"
#include "Device.h"
#include "Status.h"

/*================================================================================*
 * Read an Status Parameter setup file from disk
 *================================================================================*/
int Status_SetFwheelPointers( Status_Entry *List, int list_length,
			      Socket_Info *Info,
			      Device_Data *Data,
			      char *Connected_String,
			      char *Connection_Error_String,
			      char *Home_String,
			      char *Position_String,
			      char *Name_String,
			      char *Moving_String,
			      int debug)
{

  /* local variables */

  int i, found;

  if ( Connected_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Connected_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Info->connected);
	if( debug ){
	  printf("  Status_SetFwheelPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFwheelPointers: \"%s\" parameter name not found in Status_List\n",
	     Connected_String);
    }
  }

  if ( Connection_Error_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Connection_Error_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Info->error);
	if( debug ){
	  printf("  Status_SetFwheelPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFwheelPointers: \"%s\" parameter name not found in Status_List\n",
	     Connection_Error_String);
    }
  }

  if ( Home_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Home_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->home);
	if( debug ){
	  printf("  Status_SetFwheelPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFwheelPointers: \"%s\" parameter name not found in Status_List\n",
	     Home_String );
    }
  }

  if ( Position_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Position_String, List[i].name, strlen(List[i].name)) ) {
	List[i].f_ptr = &(Data->position);
	if( debug ){
	  printf("  Status_SetFwheelPointers: %s %f\n", List[i].name, *(List[i].f_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFwheelPointers: \"%s\" parameter name not found in Status_List\n",
	     Position_String );
    }
  }

  if ( Moving_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Moving_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->moving);
	if( debug ){
	  printf("  Status_SetFwheelPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFwheelPointers: \"%s\" parameter name not found in Status_List\n",
	     Moving_String );
    }
  }

  if ( Name_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Name_String, List[i].name, strlen(List[i].name)) ) {
	List[i].s_ptr = Data->filter;
	if( debug ){
	  printf("  Status_SetFwheelPointers: %s %s\n", List[i].name, List[i].s_ptr);
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFwheelPointers: \"%s\" parameter name not found in Status_List\n",
	     Name_String );
    }
  }

  return(0);
}

/* THE END */
