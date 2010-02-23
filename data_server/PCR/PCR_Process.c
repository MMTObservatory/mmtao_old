/*
  PCR_Process.c

  This routine starts a data server for either WFSC or DM Data

  Written  27nov05  DLM  Started with DServ.c

*/

#include "PCR.h"
#include "DServ.h"
#include "PCR_Cmd.h"

/*================================================================================*
 * Accepts requests to start a data server
 *================================================================================*/
void *PCR_Process( void *Passed_Info)
{

  /*
    Local pointers to varible passed from the process which created this thread
   */
  PCR_Thread_Info *Main_Info;
  DServ_Info *CMD_Info, *WFSC_Info, *DM_Info, *Error_Info;
  Info_Struct *Info;
  Info_Entry *List;
  int list_length;
  PCR_Thread_Client WFSC_Thread_Client, DM_Thread_Client;
  pthread_t pcr_wfscThread, pcr_dmThread;

  int sockfd;
  int *DServ_continueRunning;
  int *debug;

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  /*
    Local variables
  */
  int status;
  char request[STRING_LENGTH];
  char Request[STRING_LENGTH];
  char name[STRING_LENGTH];
  int continueRunning = 1;
  //  float temp[2];
  //  char *offsets_ptr;
  int quad, value, rate;
  float gain;
  float pgain;
  float igain;
  float dgain;

  /*
    Pointers to the needed Info structures
  */
  int Static_Position;
  int WFSC_Initialized_Position;
  int WFSC_Running_Position;
  int WFSC_Size_Position;
  int WFSC_Source_Position;
  int WFSC_Bias_Position;
  int WFSC_Frequency_Position;
  int WFSC_Period_Position;
  int Background_Static_Position;
  int Background_Dynamic_Position;
  int Seeing_Gain_Change_Position;
  int Loop_Running_Position;
  int Loop_Gain_Position;
  int Loop_Save_Data_Position;
  int Recon_Name_Position;

  /*
    Put the passed variables in to the local variables
  */
  Main_Info = (PCR_Thread_Info *)Passed_Info;
  CMD_Info = Main_Info->cmd;
  WFSC_Info = Main_Info->wfsc;
  DM_Info = Main_Info->dm;
  Error_Info = Main_Info->error;
  Info = Main_Info->info;
  List = Info->list;
  list_length = Info->length;
  sockfd = Main_Info->sockfd;
  DServ_continueRunning = Main_Info->DServ_continueRunning;
  debug = Main_Info->DServ_debug;

  /*********************************************************************
    Determine the positions of the needed InfoList structures.
   *********************************************************************/

  /*
    Only need the postion of the first offset value, Static_Astig_45, because the other
      three offsets at the next three variables in InfoList.
  */
  status = Info_FindParameter( List, list_length, "Static_Astig_45", &Static_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "WFSC_Initialized", &WFSC_Initialized_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "WFSC_Running", &WFSC_Running_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "WFSC_Size", &WFSC_Size_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "WFSC_Source", &WFSC_Source_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "WFSC_Bias_0", &WFSC_Bias_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "WFSC_Frequency", &WFSC_Frequency_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "WFSC_Period", &WFSC_Period_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "Background_Static", &Background_Static_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "Background_Dynamic", &Background_Dynamic_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "Seeing_Gain_Change", &Seeing_Gain_Change_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "Loop_Running", &Loop_Running_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "Loop_Gain", &Loop_Gain_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "Loop_Save_Data", &Loop_Save_Data_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  status = Info_FindParameter( List, list_length, "Recon_Name", &Recon_Name_Position, 1);
  if ( status ) {
    printf("  PCR_Process: Error in Info_FindParamter\n");
    fflush(stdout);
    continueRunning = 0;
  }

  while ( continueRunning ) {

    /*
      Read an instruction from the client
    */

    if ( Socket_StringRead( sockfd, request) ) {
      if ( *debug ) {
	printf("  PCR_Process: Error reading request in Socket_StringRead\n");
	fflush(stdout);
      }
      continueRunning = 0;
      continue;
    }

    /*
      Check that a request was sent
    */
    if ( strlen(request) <= 1 ) {
      printf("  PCR_Process: Warning: request of zero length recieved\n");
      fflush(stdout);
      continue;
    }

    if ( *debug ) {
      printf("  PCR_Process: Request = %s\n", request);
      fflush(stdout);
    }

    /*
      Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
    */
    status = Socket_StringLowerCase(request);

    if ( !strncmp (request, "?", strlen(request) ) ||
	 !strncmp (request, "h", strlen(request) ) ||
	 !strncmp (request, "help", strlen(request) ) ) {

      status = PCR_SendHelp( sockfd);
      if ( status ) {
	printf("  PCR_Process: Error returning help options\n");
	fflush(stdout);
      }

    } else if ( !strncmp (request, "cam_test", strlen(request) ) ) {

      /*
        Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
	List[WFSC_Initialized_Position].i_value = 0;
      } else {
	strcpy(List[WFSC_Source_Position].s_value, "Test");
	List[WFSC_Initialized_Position].i_value = 1;

	if ( !strncmp (List[WFSC_Size_Position].s_value, "--", strlen("--") ) ) {
	  /*
	    If this is the first time the camera has been initialized then set
	      the camera into default mode, which is binned mode (already set by
	      cam_test) and 550 Hz (done below).

	    Send the camera size
	  */
	  strcpy(Request, "cam_bin");
	  status = PCR_SendCmd( CMD_Info, Request, debug);
	  if ( status ) {
	    printf("PCR_Process: Error applying %s request\n", Request);
	    fflush(stdout);
	  } else {
	    strcpy(List[WFSC_Size_Position].s_value, "Bin");
	  }
	}
      }

    } else if ( !strncmp (request, "cam_image", strlen(request) ) ) {

      /*
        Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
	List[WFSC_Initialized_Position].i_value = 0;
      } else {
	strcpy(List[WFSC_Source_Position].s_value, "Camera");
	List[WFSC_Initialized_Position].i_value = 1;

	if ( !strncmp (List[WFSC_Size_Position].s_value, "Full", strlen("Full") ) ) {
	  /*
	    Send camera bias levels for full mode
	    Removed 27sep07 DLM  Takes too much time and Vidhya now sets on pcr
	  sprintf( request, "cam_offset_set 0 %d", WFSC_BIAS_FULL_0);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position].i_value = WFSC_BIAS_FULL_0;

	  sprintf( request, "cam_offset_set 1 %d", WFSC_BIAS_FULL_1);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position + 1].i_value = WFSC_BIAS_FULL_1;

	  sprintf( request, "cam_offset_set 2 %d", WFSC_BIAS_FULL_2);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position + 2].i_value = WFSC_BIAS_FULL_2;

	  sprintf( request, "cam_offset_set 3 %d", WFSC_BIAS_FULL_3);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position + 3].i_value = WFSC_BIAS_FULL_3;
	  */

	} else {
	  /*
	    Send default camera bias levels for binned mode
	    Removed 27sep07 DLM  Takes too much time and Vidhya now sets on pcr
	  sprintf( request, "cam_offset_set 0 %d", WFSC_BIAS_BIN_0);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position].i_value = WFSC_BIAS_BIN_0;

	  sprintf( request, "cam_offset_set 1 %d", WFSC_BIAS_BIN_1);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position + 1].i_value = WFSC_BIAS_BIN_1;

	  sprintf( request, "cam_offset_set 2 %d", WFSC_BIAS_BIN_2);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position + 2].i_value = WFSC_BIAS_BIN_2;

	  sprintf( request, "cam_offset_set 3 %d", WFSC_BIAS_BIN_3);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position + 3].i_value = WFSC_BIAS_BIN_3;
	  */

	  if ( strncmp (List[WFSC_Size_Position].s_value, "Bin", strlen("Bin") ) ) {
	    /*
	      If this is the first time the camera has been initialized then set
	        the camera into default mode, which is binned mode (already set by
		cam_image) and 550 Hz (done below).

		Send the camera size
	    */
	    strcpy(Request, "cam_bin");
	    status = PCR_SendCmd( CMD_Info, Request, debug);
	    if ( status ) {
	      printf("PCR_Process: Error applying %s request\n", Request);
	      fflush(stdout);
	    } else {
	      strcpy(List[WFSC_Size_Position].s_value, "Bin");
	    }

	    /*
	      Send the camera rate
	    */
	    rate = 550;
	    strcpy(Request, "cam_rate_set 550");
	    status = PCR_SendCmd( CMD_Info, Request, debug);
	    if ( status ) {
	      printf("PCR_Process: Error applying %s request\n", Request);
	      fflush(stdout);
	    } else {
	      List[WFSC_Frequency_Position].i_value = rate;
	      List[WFSC_Period_Position].f_value = 1000.0/(float)rate;
	    }
	  }
	}
      }

    } else if ( !strncmp (request, "cam_full", strlen(request) ) ) {

      /*
        Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
      } else {
	strcpy(List[WFSC_Size_Position].s_value, "Full");

	if ( !strncmp (List[WFSC_Source_Position].s_value, "Camera", strlen("Camera") ) ) {
	  /*
	    If the camera is in image mode then send bias levels for full mode
	    Removed 27sep07 DLM  Takes too much time and Vidhya now sets on pcr
	  sprintf( request, "cam_offset_set 0 %d", WFSC_BIAS_FULL_0);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position].i_value = WFSC_BIAS_FULL_0;

	  sprintf( request, "cam_offset_set 1 %d", WFSC_BIAS_FULL_1);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position + 1].i_value = WFSC_BIAS_FULL_1;

	  sprintf( request, "cam_offset_set 2 %d", WFSC_BIAS_FULL_2);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position + 2].i_value = WFSC_BIAS_FULL_2;

	  sprintf( request, "cam_offset_set 3 %d", WFSC_BIAS_FULL_3);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position + 3].i_value = WFSC_BIAS_FULL_3;
	  */
	}

      }

    } else if ( !strncmp (request, "cam_bin", strlen(request) ) ) {

      /*
        Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
      } else {
	strcpy(List[WFSC_Size_Position].s_value, "Bin");

	if ( !strncmp (List[WFSC_Source_Position].s_value, "Camera", strlen("Camera") ) ) {
	  /*
	    If the camera is in image mode then send bias levels for bin mode
	    Removed 27sep07 DLM  Takes too much time and Vidhya now sets on pcr
	  sprintf( request, "cam_offset_set 0 %d", WFSC_BIAS_BIN_0);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position].i_value = WFSC_BIAS_BIN_0;

	  sprintf( request, "cam_offset_set 1 %d", WFSC_BIAS_BIN_1);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position + 1].i_value = WFSC_BIAS_BIN_1;

	  sprintf( request, "cam_offset_set 2 %d", WFSC_BIAS_BIN_2);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position + 2].i_value = WFSC_BIAS_BIN_2;

	  sprintf( request, "cam_offset_set 3 %d", WFSC_BIAS_BIN_3);
	  status = PCR_SendCmd( CMD_Info, request, debug);
	  List[WFSC_Bias_Position + 3].i_value = WFSC_BIAS_BIN_3;
	  */
	}

      }

    } else if ( !strncmp (request, "cam_bias_set", strlen(request) ) ) {

      /*
	Determine which quadrant is to be set and its value
      */
      if ( Socket_StringRead( sockfd, request) ) {
	if ( *debug ) {
	  printf("  PCR_Process: Error reading Camera Quadrant in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }
      quad = atoi(request);

      if ( Socket_StringRead( sockfd, request) ) {
	if ( *debug ) {
	  printf("  PCR_Process: Error reading Camera Bias Value in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }
      value = atoi(request);

      /*
	Create request to send to the PCR
      */
      sprintf(Request, "cam_offset_set %d %d", quad, value);
 
      /*
        Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      } else {
	List[WFSC_Bias_Position + quad].i_value = value;
      }

    } else if ( !strncmp (request, "cam_rate_set", strlen(request) ) ) {

      /*
	Determine which quadrant is to be set and its value
      */
      if ( Socket_StringRead( sockfd, request) ) {
	if ( *debug ) {
	  printf("  PCR_Process: Error reading Camera Quadrant in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }
      rate = atoi(request);

      /*
	Create request to send to the PCR
      */
      sprintf(Request, "cam_rate_set %d", rate);

      /*
        Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      } else {
	List[WFSC_Frequency_Position].i_value = rate;
	List[WFSC_Period_Position].f_value = 1000.0/(float)rate;
      }

    } else if ( !strncmp (request, "recon_set", strlen(request) ) ) {

      /*
	Read the name of the reconstructor file to send to the PCR
      */
      if ( Socket_StringRead( sockfd, request) ) {
	if ( *debug ) {
	  printf("  PCR_Process: Error reading Reconstructor Name in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }

      /*
	Create request to send to the PCR
      */
      sprintf(Request, "recon_mode %s", request);

      /*
        Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      } else {
	strcpy(List[Recon_Name_Position].s_value, request);
      }

    } else if ( !strncmp (request, "cam_start", strlen(request) ) ) {

      /*
        Send a request to the PCR
      */
      strcpy( request, "wfsc_start");
      status = PCR_SendCmd( CMD_Info, request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
      } else {
	List[WFSC_Running_Position].i_value = 1;
      }

    } else if ( !strncmp (request, "cam_stop", strlen(request) ) ) {

      /*
        If the loop is running, then we must open it
      */
      if ( List[Loop_Running_Position].i_value ) {
	strcpy( request, "loop_open");
	status = PCR_SendCmd( CMD_Info, request, debug);
	if ( status ) {
	  printf("PCR_Process: Error applying %s request\n", request);
	  fflush(stdout);
	} else {
	  List[Loop_Running_Position].i_value = 0;
	}
      }
	
      /*
        Send a request to the PCR
      */
      strcpy( request, "wfsc_stop");
      status = PCR_SendCmd( CMD_Info, request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
      } else {
	List[WFSC_Running_Position].i_value = 0;
      }

      /*
	Reset the PCR_WFSC Client thread by setting the reset flag to 1
      */
      *Main_Info->PCR_WFSC_reset = 1;

    } else if ( !strncmp (request, "loop_close", strlen(request) ) ) {

      /*
	Create request to set the loop gain to 0.0
      */
      strcpy(Request, "loop_gain 0.0");

      /*
	Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
      } else {
	List[Loop_Gain_Position].f_value = 0.0;
	strcpy(List[Seeing_Gain_Change_Position].s_value, "1");
      }

      /*
        Send a close loop request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
      } else {
	List[Loop_Running_Position].i_value = 1;
      }

    } else if ( !strncmp (request, "loop_open", strlen(request) ) ) {

      /*
        Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
      } else {
	List[Loop_Running_Position].i_value = 0;

	/*
	  Create request to set the loop gain to 0.0
	*/
	strcpy(Request, "loop_gain 0.0");

	/*
	  Send a request to the PCR
	*/
	status = PCR_SendCmd( CMD_Info, Request, debug);
	if ( status ) {
	  printf("PCR_Process: Error applying %s request\n", request);
	  fflush(stdout);
	} else {
	  List[Loop_Gain_Position].f_value = 0.0;
	  strcpy(List[Seeing_Gain_Change_Position].s_value, "1");
	}
      }

      /*
	Reset the PCR_DM Client thread by setting the reset flag to 1
      */
      *Main_Info->PCR_DM_reset = 1;

    } else if ( !strncmp (request, "loop_gain", strlen(request) ) ) {

      /*
	Read the request gain value
      */
      if ( Socket_StringRead( sockfd, request) ) {
	if ( *debug ) {
	  printf("  PCR_Process: Error reading Camera Quadrant in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }
      gain = atof(request);

      /*
	Check that the requested gain is within range
      */
      if ( gain < 0.0 || gain > 1.0 ) {
	printf("PCR_Process: Loop gain is out of range [0-1], request = %f\n", gain);
	fflush(stdout);
	continue;
      }

      /*
	Create request to send to the PCR
      */
      sprintf(Request, "loop_gain %.3f", gain);

      /*
        Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
      } else {
	List[Loop_Gain_Position].f_value = gain;
	strcpy(List[Seeing_Gain_Change_Position].s_value, "1");
      }

    } else if ( !strncmp (request, "loop_pgain", strlen(request) ) ) {

      /*
	Read the request pgain value
      */
      if ( Socket_StringRead( sockfd, request) ) {
	if ( *debug ) {
	  printf("  PCR_Process: Error reading PGain in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }
      pgain = atof(request);

      /*
	Check that the requested pgain is within range
      */
      if ( pgain < 0.0 || pgain > 10.0 ) {
	printf("PCR_Process: Loop pgain is out of range [0-10], request = %f\n", pgain);
	fflush(stdout);
	continue;
      }

      /*
	Create request to send to the PCR
      */
      sprintf(Request, "set_proportional_gain %.3f", pgain);

      /*
        Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
      } else {
        ;
      }

    } else if ( !strncmp (request, "loop_igain", strlen(request) ) ) {

      /*
	Read the request igain value
      */
      if ( Socket_StringRead( sockfd, request) ) {
	if ( *debug ) {
	  printf("  PCR_Process: Error reading IGain in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }
      igain = atof(request);

      /*
	Check that the requested igain is within range
      */
      if ( igain < 0.0 || igain > 10.0 ) {
	printf("PCR_Process: Loop igain is out of range [0-10], request = %f\n", igain);
	fflush(stdout);
	continue;
      }

      /*
	Create request to send to the PCR
      */
      sprintf(Request, "set_integral_gain %.3f", igain);

      /*
        Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
      } else {
        ;
      }

    } else if ( !strncmp (request, "loop_dgain", strlen(request) ) ) {

      /*
	Read the request dgain value
      */
      if ( Socket_StringRead( sockfd, request) ) {
	if ( *debug ) {
	  printf("  PCR_Process: Error reading DGain in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }
      dgain = atof(request);

      /*
	Check that the requested dgain is within range
      */
      if ( dgain < 0.0 || dgain > 10.0 ) {
	printf("PCR_Process: Loop dgain is out of range [0-10], request = %f\n", dgain);
	fflush(stdout);
	continue;
      }

      /*
	Create request to send to the PCR
      */
      sprintf(Request, "set_derivative_gain %.3f", dgain);

      /*
        Send a request to the PCR
      */
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", request);
	fflush(stdout);
      } else {
        ;
      }

    } else if ( !strncmp (request, "set_pid_off", strlen(request) ) ) {

      /*
        Send a request to turn pid off
      */
      strcpy(Request, "set_pid_off");
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      }

    } else if ( !strncmp (request, "set_pid_on", strlen(request) ) ) {

      /*
        Send a request to turn pid on
      */
      strcpy(Request, "set_pid_on");
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      }

    } else if ( !strncmp (request, "slope_offset_on", strlen(request) ) ) {
      strcpy(Request, "slope_offset_on");
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      }
    } else if ( !strncmp (request, "slope_offset_off", strlen(request) ) ) {
      strcpy(Request, "slope_offset_off");
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      }
    } else if ( !strncmp (request, "mod_offset_on", strlen(request) ) ) {
      strcpy(Request, "mod_offset_on");
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      }
    } else if ( !strncmp (request, "mod_offset_off", strlen(request) ) ) {
      strcpy(Request, "mod_offset_off");
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      }

      //    } else if ( !strncmp (request, "cam_temp_get", strlen(request) ) ) {

      /*
        Send a request to the PCR
      */
      //      status = PCR_SendCmd_Old( CMD_Info, sockfd, debug);

      //      printf("geting cam_temp\n");
      /*
	Read the slope offset array from the client
      */
      //      offsets_ptr = (char *)temp;
      //      if ( Socket_Read( CMD_Info->socket->sockfd, offsets_ptr, 2*sizeof(float)) ) {
      //	printf("  PCR_SendCmd: Error reading slope offset array in Socket_Read\n");
      //      } else {
      //	printf("    %f  %f\n", temp[0], temp[1]);
      //      }
      
    } else if ( !strncmp (request, "cam_bg_set", strlen(request) ) ) {

      /*
        Send a WFSC background frame to the PCR
      */
      status = PCR_SendShortVector( CMD_Info, sockfd, "cam_bg_set", debug);

      /*
        Send a WFSC background frame to the PCR
      */
      strcpy(Request, "cam_bg_on");
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      } else {
	List[Background_Static_Position].i_value = 1;
      }

    } else if ( !strncmp (request, "cam_bg_on", strlen(request) ) ) {

      /*
        Send a request to turn on background subtraction in the PCR
      */
      strcpy(Request, "cam_bg_on");
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      } else {
	List[Background_Static_Position].i_value = 1;
      }

    } else if ( !strncmp (request, "cam_bg_off", strlen(request) ) ) {

      /*
        Send a request to turn off background subtraction in the PCR
      */
      strcpy(Request, "cam_bg_off");
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      } else {
	List[Background_Static_Position].i_value = 0;
      }

    } else if ( !strncmp (request, "cam_dynamic_on", strlen(request) ) ) {

      /*
        Send a request to turn on background subtraction in the PCR
      */
      strcpy(Request, "cam_dynamic_on");
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      } else {
	List[Background_Dynamic_Position].i_value = 1;
      }

    } else if ( !strncmp (request, "cam_dynamic_off", strlen(request) ) ) {

      /*
        Send a request to turn off background subtraction in the PCR
      */
      strcpy(Request, "cam_dynamic_off");
      status = PCR_SendCmd( CMD_Info, Request, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      } else {
	List[Background_Dynamic_Position].i_value = 0;
      }

    } else if ( !strncmp (request, "slope_offset_set", strlen(request) ) ) {

      /*
        Send a Slope Offsets vector to the PCR
      */
      status = PCR_SendSlopeOffsets( CMD_Info, &List[Static_Position], sockfd, debug);

    } else if ( !strncmp (request, "mod_offset_set", strlen(request) ) ) {

      /*
        Send a Slope Offsets vector to the PCR
      */
      status = PCR_SendFloatVector( CMD_Info, sockfd, "mod_offset_set", debug);

    } else if ( !strncmp (request, "mod_rate_set", strlen(request) ) ) {

      /*
        Send a Slope Offsets vector to the PCR
      */
      status = PCR_SendFloatVector( CMD_Info, sockfd, "mod_rate_set", debug);

    } else if ( !strncmp (request, "dm_cmd", strlen(request) ) ) {

      /*
	Read the request to send to the DDS server
      */
      if ( Socket_StringRead( sockfd, request) ) {
	if ( *debug ) {
	  printf("  PCR_Process: Error reading dm_cmd in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }

      /*
	Check that the request is valid and determine its value
      */
      if ( !strncmp (request, "Save_Data_On", strlen(request) ) ) {
	value = 1;
      } else if ( !strncmp (request, "Save_Data_Off", strlen(request) ) ) {
	value = 0;
      } else {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
	continue;
      }
	  
      /*
        Send a request to the PCR via the DM data port
      */
      status = PCR_SendCmd_NoReply( DM_Info, request, sockfd, debug);
      if ( status ) {
	printf("PCR_Process: Error applying %s request\n", Request);
	fflush(stdout);
      } else {
	  List[Loop_Save_Data_Position].i_value = value;
      }

    } else if ( !strncmp (request, "connect", strlen(request) ) ) {

      /*
	Read which PCR server to connect to.
      */
      if ( Socket_StringRead( sockfd, name) ) {
	if ( *debug ) {
	  printf("  PCR_Process: Error reading request in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }

      /*
	Check that a request was sent
      */
      if ( strlen(name) <= 1 ) {
	printf("  PCR_Process: Warning: request of zero length recieved\n");
	fflush(stdout);
	continue;
      }

      if ( *debug ) {
	printf("  PCR_Process: Request = %s\n", name);
	fflush(stdout);
      }

      if ( !strncmp (name, "cmd", strlen(name) ) ) {

	/*
	  Check to see if connection to PCR_Cmd has already been make
	*/
	if ( !CMD_Info->socket->connected ) {

	  /*
	    Open a socket to the PCR server
	  */
	  status =  Socket_OpenClient ( (Socket_Info *)CMD_Info->socket, *debug);
	  if ( status ) {
	    printf("  PCR_Process: Error in Socket_OpenClient\n");
	    fflush(stdout);
	  }
	}

      } else if ( !strncmp (name, "wfsc", strlen(name) ) ) {

	/*
	  Check to see if the PCR_WFSC Client is already running
	*/
	if ( *Main_Info->PCR_WFSC_running ) {

	  /*
	    This server is running, so don't start another thread
	  */
	  printf("  PCR_WFSC Client thread already running\n");
	  fflush(stdout);

	} else {

	  /*
	    Set running flags
	  */
	  *Main_Info->PCR_WFSC_running = 1;
	  *Main_Info->PCR_WFSC_continueRunning = 2;

	  /*
	    Create the required pointer to data memory
	  */
	  WFSC_Thread_Client.info = WFSC_Info;
	  WFSC_Thread_Client.continueRunning = Main_Info->PCR_WFSC_continueRunning;
	  WFSC_Thread_Client.running = Main_Info->PCR_WFSC_running;
	  WFSC_Thread_Client.reset = Main_Info->PCR_WFSC_reset;
	  WFSC_Thread_Client.debug = Main_Info->PCR_WFSC_debug;

	  /*
	    Start PCR_WFSC Thread
	  */
	  status = pthread_create( &pcr_wfscThread, &attr, PCR_GetWFSC, (void *)&WFSC_Thread_Client);
	  if ( debug ) {
	    printf("  PCR_Process: Starting PCR_WFSC thread\n");
	    fflush(stdout);
	  }
	  if ( status ) {
	    printf("  PCR_Process: Error starting PCR_WFSC thread\n");
	    fflush(stdout);
	  }
	}

      } else if ( !strncmp (name, "dm", strlen(name) ) ) {

	/*
	  Check to see if the PCR_DM Client is already running
	*/
	if ( *Main_Info->PCR_DM_running ) {

	  /*
	    This server is running, so don't start another thread
	  */
	  printf("  PCR_DM Client thread already running\n");
	  fflush(stdout);

	} else {

	  /*
	    Set running flag
	  */
	  *Main_Info->PCR_DM_running = 1;
	  *Main_Info->PCR_DM_continueRunning = 1;

	  /*
	    Create the required pointer to data memory
	  */
	  DM_Thread_Client.info = DM_Info;
	  DM_Thread_Client.continueRunning = Main_Info->PCR_DM_continueRunning;
	  DM_Thread_Client.running = Main_Info->PCR_DM_running;
	  DM_Thread_Client.reset = Main_Info->PCR_DM_reset;
	  DM_Thread_Client.debug = Main_Info->PCR_DM_debug;

	  /*
	    Start PCR_DM Thread
	  */
	  status = pthread_create( &pcr_dmThread, &attr, PCR_GetDM, (void *)&DM_Thread_Client);
	  if ( debug ) {
	    printf("  PCR_Process: Starting PCR_DM thread\n");
	    fflush(stdout);
	  }
	  if ( status ) {
	    printf("  PCR_Process: Error starting PCR_DM thread\n");
	    fflush(stdout);
	  }
	}

      } else if ( !strncmp (name, "error", strlen(name) ) ) {

	/*
	  Check to see if connection to PCR_Cmd has already been make
	*/
	if ( !Error_Info->socket->connected ) {

	  /*
	    Open a socket to the PCR server
	  */
	  status =  Socket_OpenClient ( (Socket_Info *)Error_Info->socket, *debug);
	  if ( status ) {
	    printf("  PCR_Process: Error in Socket_OpenClient\n");
	    fflush(stdout);
	  }
	}
      }

    } else if ( !strncmp (request, "stop", strlen(request) ) ) {

      /*
	Read which PCR server to stop.
      */
      if ( Socket_StringRead( sockfd, name) ) {
	if ( *debug ) {
	  printf("  PCR_Process: Error reading request in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }

      /*
	Check that a request was sent
      */
      if ( strlen(name) <= 1 ) {
	printf("  PCR_Process: Warning: request of zero length recieved\n");
	fflush(stdout);
	continue;
      }

      if ( *debug ) {
	printf("                          %s\n", name);
	fflush(stdout);
      }

      /*
	Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
      */
      status = Socket_StringLowerCase(name);

      if ( !strncmp (name, "wfsc", strlen(name) ) ) {

	/*
	  Stop the PCR_WFSC Client thread by setting the continueRunning flag to 0
	*/
	*Main_Info->PCR_WFSC_continueRunning = 0;

      } else if ( !strncmp (name, "dm", strlen(name) ) ) {

	/*
	  Stop the PCR_DM Client thread by setting the continueRunning flag to 0
	*/
	*Main_Info->PCR_DM_continueRunning = 0;

      }

    } else if ( !strncmp (request, "reset", strlen(request) ) ) {

      /*
	Read which PCR server to stop.
      */
      if ( Socket_StringRead( sockfd, name) ) {
	if ( *debug ) {
	  printf("  PCR_Process: Error reading request in Socket_StringRead\n");
	  fflush(stdout);
	}
	continue;
      }

      /*
	Check that a request was sent
      */
      if ( strlen(name) <= 1 ) {
	printf("  PCR_Process: Warning: request of zero length recieved\n");
	fflush(stdout);
	continue;
      }

      if ( *debug ) {
	printf("                          %s\n", name);
	fflush(stdout);
      }

      /*
	Set request to all lower case and trim control characters from the end (\n, \r, \t etc)
      */
      status = Socket_StringLowerCase(name);

      if ( !strncmp (name, "wfsc", strlen(name) ) ) {

	/*
	  Reset the PCR_WFSC Client thread by setting the reset flag to 1
	*/
	*Main_Info->PCR_WFSC_reset = 1;

      } else if ( !strncmp (name, "dm", strlen(name) ) ) {

	/*
	  Reset the PCR_DM Client thread by setting the reset flag to 1
	*/
	*Main_Info->PCR_DM_reset = 1;

      }

    } else if ( !strncmp (request, "quit", strlen(request) ) ) {

      /*
        Stop this server
      */
      continueRunning = 0;

    } else if ( !strncmp (request, "exit", strlen(request) ) ) {

      /*
        Stop this all threads
      */
      *DServ_continueRunning = 0;

    } else {

      printf("  PCR_Process: Unknown request = %s\n", request);
      fflush(stdout);

    }

    request[0] = 0;

  }

  if ( *debug ) {
    printf("  PCR_Process: Shutdown\n");
    fflush(stdout);
  }

  /*
    Set sockfd to -1 to indicate to mother process that this Thread_Info
      structure can be reused
  */
  close( Main_Info->sockfd);
  Main_Info->sockfd = -1;
  pthread_exit(NULL);

  return( (void *)0);
}
