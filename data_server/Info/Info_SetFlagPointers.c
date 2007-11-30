/*=======================================================================
 * Info_SetFlagPointers.c
 *=======================================================================

  Set several flag pointers in the Info structure to the correct locations

  Written  29dec06  DLM  

*/

#include "PCR.h"
#include "Info.h"

/*================================================================================*
 * Read an Info Parameter setup file from disk
 *================================================================================*/
int Info_SetFlagPointers( Info_Entry *List, long list_length, int *PCR_Client_running,
			  int *DServ_WFSC_running, int *DServ_DDS_running, int *Info_running,
			  int *PCR_WFSC_receiving_data, int *PCR_DDS_receiving_data,
			  int *PCR_WFSC_rate, int *PCR_DDS_rate,
			  Socket_Info *PCR_CMD_Socket, Socket_Info *PCR_WFSC_Socket,
			  Socket_Info *PCR_DDS_Socket,
			  Socket_Info *PCR_Error_Socket, long debug)
{

  /* local variables */

  int i, found;

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_Client_Running", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = PCR_Client_running;
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
      break;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_Client_Running");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "DServ_WFSC_Running", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = DServ_WFSC_running;
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "DServ_WFSC_Running");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "DServ_DDS_Running", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = DServ_DDS_running;
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "DServ_DDS_Running");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "Info_Running", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = Info_running;
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "Info_Running");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_WFSC_Receiving_Data", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = PCR_WFSC_receiving_data;
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_WFSC_Receiving_Data");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_WFSC_Rate", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = PCR_WFSC_rate;
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_WFSC_Rate");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_DDS_Receiving_Data", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = PCR_DDS_receiving_data;
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_DDS_Receiving_Data");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_DDS_Rate", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = PCR_DDS_rate;
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_DDS_Rate");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_CMD_Connected", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = &(PCR_CMD_Socket->connected);
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_CMD_Connected");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_CMD_Error", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = &(PCR_CMD_Socket->error);
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_CMD_Error");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_WFSC_Connected", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = &(PCR_WFSC_Socket->connected);
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_WFSC_Connected");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_WFSC_Error", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = &(PCR_WFSC_Socket->error);
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_WFSC_Error");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_DDS_Connected", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = &(PCR_DDS_Socket->connected);
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_DDS_Connected");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_DDS_Error", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = &(PCR_DDS_Socket->error);
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_DDS_Error");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_ERROR_Connected", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = &(PCR_Error_Socket->connected);
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_ERROR_Connected");
    fflush(stdout);
  }

  found=0;
  for ( i=0; i<list_length; i++ ) {
    if ( !strncmp( "PCR_ERROR_Error", List[i].name, strlen(List[i].name)) ) {
      List[i].i_ptr = &(PCR_Error_Socket->error);
      if( debug ){
	printf("  Info_SetFlagPointers: %s %d\n", List[i].name, *(List[i].i_ptr));
	fflush(stdout);
      }
      found=1;
    }
  }
  if ( !found ) {
    printf("    Error in Info_SetFlagPointers: \"%s\" parameter name not found in Info_List\n",
	   "PCR_ERROR_Error");
    fflush(stdout);
  }

  return(0);
}
