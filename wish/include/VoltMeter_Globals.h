
/*
  AO_Wish_MotorGlobals.c

  Declare global variables shared between C and Tcl/Tk.

  The definitions in this file are needed to create global variables
    shared by Tcl and C.  Thus, for the wish shell used to create the MMTAO
    GUI all communications and calculations are processed by compiled C code
    and TckTk is only used to create the Graphic User Interface (GUI),

  This file is included in the header of AO_wish.c

  Written  26aug03  DLM

======================================================================================

  Variables global to C and Tcl

  The convention used is only debug variables and a replyData string are passed
    back and forth from C to Tcl via global.  All other information is passed
    via arguments to the Tcl command and a returned status (int) from C.

  This way it is clear to the programmer what data is being passed, and decisions
    as to how to use this information (ie. automatically update displays) must and
    will be done manually in the calling Tcl code.

=========================================================*/

/*********************
 *  HP VoltMeter     *
 *********************/

char* VoltMeter_Address;
int VoltMeter_Port;

int debug_VoltMeter;

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
int VoltMeter_Connected;
int VoltMeter_Error;

double VoltMeter_Res_1;
double VoltMeter_Res_2;
double VoltMeter_Res_3;
double VoltMeter_Res_4;
double VoltMeter_Res_5;
double VoltMeter_Res_6;
double VoltMeter_Res_7;
double VoltMeter_Res_8;
double VoltMeter_Res_9;
double VoltMeter_Res_10;
double VoltMeter_Res_11;
double VoltMeter_Res_12;
double VoltMeter_Res_13;
double VoltMeter_Res_14;
double VoltMeter_Res_15;
