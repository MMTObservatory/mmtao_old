
/*
  Server_Globals.c

  Declare global variables shared between C and Tcl/Tk.

  The definitions in this file are needed to create global variables
    shared by Tcl and C.  Thus, for the wish shell used to create the MMTAO
    GUI all communications and calculations are processed by compiled C code
    and TclTk is only used to create the Graphic User Interface (GUI),

  This file is included in the header of PCR_Wish.c

  Written   1feb07  DLM  Started with Client_Globals.h

=========================================================*/

/*************************
 *   PCR Server  *
 *************************/

char *PCR_Server_Address;
char *PCR_Address;

/*************************
 *   Topbox Server  *
 *************************/
char *Topbox_Server_Address;

/*************************
 *   Science Server  *
 *************************/
char *Science_Server_Address;
