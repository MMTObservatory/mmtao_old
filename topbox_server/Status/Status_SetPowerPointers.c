/*=======================================================================
 * Info_SetPowerPointers.c
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
int Status_SetPowerPointers( Status_Entry *List, int list_length,
			     Socket_Info *Info,
			     Device_Data *Data,
			     char *Connected_String,
			     char *Connection_Error_String,
			     char *WFSC_String,
			     char *Peltier_String,
			     char *Stella_String,
			     char *Alignment_Laser_String,
			     char *F15_Laser_String,
			     char *Indigo_String,
			     char *Spare1_String,
			     char *Spare2_String,
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
	  printf("  Status_SetPowerPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetPowerPointers: \"%s\" parameter name not found in Status_List\n",
	     Connected_String);
    }
  }

  if ( Connection_Error_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Connection_Error_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Info->error);
	if( debug ){
	  printf("  Status_SetPowerPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetPowerPointers: \"%s\" parameter name not found in Status_List\n",
	     Connection_Error_String);
    }
  }

  if ( WFSC_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( WFSC_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->relays[0]);
	if( debug ){
	  printf("  Status_SetPowerPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetPowerPointers: \"%s\" parameter name not found in Status_List\n",
	     WFSC_String );
    }
  }

  if ( Peltier_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Peltier_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->relays[1]);
	if( debug ){
	  printf("  Status_SetPowerPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetPowerPointers: \"%s\" parameter name not found in Status_List\n",
	     Peltier_String );
    }
  }

  if ( Stella_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Stella_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->relays[2]);
	if( debug ){
	  printf("  Status_SetPowerPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetPowerPointers: \"%s\" parameter name not found in Status_List\n",
	     Stella_String );
    }
  }

  if ( Alignment_Laser_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Alignment_Laser_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->relays[3]);
	if( debug ){
	  printf("  Status_SetPowerPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetPowerPointers: \"%s\" parameter name not found in Status_List\n",
	     Alignment_Laser_String );
    }
  }

  if ( F15_Laser_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( F15_Laser_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->relays[4]);
	if( debug ){
	  printf("  Status_SetPowerPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetPowerPointers: \"%s\" parameter name not found in Status_List\n",
	     F15_Laser_String );
    }
  }

  if ( Indigo_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Indigo_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->relays[5]);
	if( debug ){
	  printf("  Status_SetPowerPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetPowerPointers: \"%s\" parameter name not found in Status_List\n",
	     Indigo_String );
    }
  }

  if ( Spare1_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Spare1_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->relays[6]);
	if( debug ){
	  printf("  Status_SetPowerPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetPowerPointers: \"%s\" parameter name not found in Status_List\n",
	     Spare1_String );
    }
  }

  if ( Spare2_String ) {
    found=0;
    for ( i=0; i<list_length; i++ ) {
      if ( !strncmp( Spare2_String, List[i].name, strlen(List[i].name)) ) {
	List[i].i_ptr = &(Data->relays[7]);
	if( debug ){
	  printf("  Status_SetPowerPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	}
	found=1;
      }
    }
    if ( !found ) {
      printf("    Error in Status_SetPowerPointers: \"%s\" parameter name not found in Status_List\n",
	     Spare2_String );
    }
  }

  return(0);
}
