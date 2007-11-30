/*
  AO_wishMotors.h

  Link global variables and new routines between Tcl and C

  The definitions in this file are needed to create global variables
    shared by Tcl and C, and to define new commands in Tcl.  Thus,
    for the wish shell used to create the MMTAO GUI all communications
    and calculations are processed by compiled C code and TckTk is only
    used to create the Graphic User Interface (GUI),

  This file is included in the **body of Tcl_AppInit()** of AO_wish.c

  Written  26aug03  DLM

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  See description in ./AO_wishGlobals.c and ../AO_wish.c

=========================================================*/

/* Link Tcl Commands to C Routines and associated global variables */

/**********************/
/* HP VoltMeter stuff */
/**********************/
Tcl_CreateCommand(interp,"VoltMeter_Cmd", (Tcl_CmdProc*) VoltMeter_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "VoltMeter_Address", (char*) &VoltMeter_Address, TCL_LINK_STRING);
Tcl_LinkVar(interp, "VoltMeter_Port", (char *)&VoltMeter_Port, TCL_LINK_INT);

Tcl_LinkVar(interp, "debug_VoltMeter", (char *)&debug_VoltMeter, TCL_LINK_INT);

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
Tcl_LinkVar(interp, "VoltMeter_Connected", (char *)&VoltMeter_Connected, TCL_LINK_INT);
Tcl_LinkVar(interp, "VoltMeter_Error", (char *)&VoltMeter_Error, TCL_LINK_INT);

Tcl_LinkVar(interp, "Resistance_01", (char *)&VoltMeter_Res_1, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_02", (char *)&VoltMeter_Res_2, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_03", (char *)&VoltMeter_Res_3, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_04", (char *)&VoltMeter_Res_4, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_05", (char *)&VoltMeter_Res_5, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_06", (char *)&VoltMeter_Res_6, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_07", (char *)&VoltMeter_Res_7, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_08", (char *)&VoltMeter_Res_8, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_09", (char *)&VoltMeter_Res_9, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_10", (char *)&VoltMeter_Res_10, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_11", (char *)&VoltMeter_Res_11, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_12", (char *)&VoltMeter_Res_12, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_13", (char *)&VoltMeter_Res_13, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_14", (char *)&VoltMeter_Res_14, TCL_LINK_DOUBLE);
Tcl_LinkVar(interp, "Resistance_15", (char *)&VoltMeter_Res_15, TCL_LINK_DOUBLE);
