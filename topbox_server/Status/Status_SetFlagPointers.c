/*=======================================================================
 * Info_SetFlagPointers.c
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
int Status_SetFlagPointers( Status_Entry *List, long list_length,
			    Socket_Info *Info,
			    Device_Data *Data,
			    char *Connected_String,
			    char *Connection_Error_String,
			    char *Home_String,
			    char *Position_String,
			    char *Moving_String,
			    char *In_Position_String,
			    char *Error_String,
			    char *Enabled_String,
			    char *Negative_String,
			    char *Positive_String,
			    char *Event_2_String,
			    long debug)
{

  /* local variables */

  int i, found;

  if ( Connected_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Connected_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Info->connected);
	if( debug ){
	  printf("  Status_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFlagPointers: \"%s\" parameter name not found in Status_List\n",
	     Connected_String);
    }
  }

  if ( Connection_Error_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Connection_Error_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Info->error);
	if( debug ){
	  printf("  Status_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFlagPointers: \"%s\" parameter name not found in Status_List\n",
	     Connection_Error_String);
    }
  }

  if ( Home_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Home_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->home);
	if( debug ){
	  printf("  Status_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFlagPointers: \"%s\" parameter name not found in Status_List\n",
	     Home_String );
    }
  }

  if ( Position_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Position_String, List[i].name, strlen(List[i].name)) ) {
	List[i].f_ptr = &(Data->position);
	if( debug ){
	  printf("  Status_SetFlagPointers: %s %f\n", List[i].name, *(List[i].f_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFlagPointers: \"%s\" parameter name not found in Status_List\n",
	     Position_String );
    }
  }

  if ( Moving_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Moving_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->moving);
	if( debug ){
	  printf("  Status_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFlagPointers: \"%s\" parameter name not found in Status_List\n",
	     Moving_String );
    }
  }

  if ( In_Position_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( In_Position_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->in_position);
	if( debug ){
	  printf("  Status_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFlagPointers: \"%s\" parameter name not found in Status_List\n",
	     In_Position_String );
    }
  }

  if ( Error_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Error_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->error);
	if( debug ){
	  printf("  Status_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFlagPointers: \"%s\" parameter name not found in Status_List\n",
	     Error_String );
    }
  }

  if ( Enabled_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Enabled_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->enabled);
	if( debug ){
	  printf("  Status_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFlagPointers: \"%s\" parameter name not found in Status_List\n",
	     Enabled_String );
    }
  }

  if ( Negative_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Negative_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->negative);
	if( debug ){
	  printf("  Status_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFlagPointers: \"%s\" parameter name not found in Status_List\n",
	     Negative_String );
    }
  }

  if ( Positive_String ){
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Positive_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->positive);
	if( debug ){
	  printf("  Status_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFlagPointers: \"%s\" parameter name not found in Status_List\n",
	     Positive_String );
    }
  }

  if ( Event_2_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Event_2_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->event_2);
	if( debug ){
	  printf("  Status_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetFlagPointers: \"%s\" parameter name not found in Status_List\n",
	     Event_2_String );
    }
  }

  return(0);
}
