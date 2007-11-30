
/*
  AO_Wish_SkyLink.h

  Link global variables and new routines between Tcl and C

  The definitions in this file are needed to create global variables
    shared by Tcl and C, and to define new commands in Tcl.  Thus,
    for the wish shell used to create the MMTAO GUI all communications
    and calculations are processed by compiled C code and TckTk is only
    used to create the Graphic User Interface (GUI),

  This file is included in the **body of Tcl_AppInit()** of AO_wish.c

  Written  26aug03  DLM

  Altered   2feb07  DLM  Removed Sky links and added BG links for PCR_Wish

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  See description in ./AO_wishGlobals.c and ../AO_wish.c

=========================================================*/

/* Link Tcl Commands to C Routines and associated global variables */

/* Static Aberration stuff */
Tcl_CreateCommand(interp,"Static_Cmd", (Tcl_CmdProc*) Static_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "debug_Static", (char *)&debug_Static, TCL_LINK_INT);
Tcl_LinkVar(interp, "Static_Matrix_Read", (char *)&Static_Matrix_Read, TCL_LINK_INT);

/* Slope Modulation stuff */
Tcl_CreateCommand(interp,"Mod_Cmd", (Tcl_CmdProc*) Mod_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "debug_Mod", (char *)&debug_Mod, TCL_LINK_INT);
Tcl_LinkVar(interp, "Mod_Matrix_Read", (char *)&Mod_Matrix_Read, TCL_LINK_INT);

/* Background stuff */
Tcl_CreateCommand(interp,"BG_Cmd", (Tcl_CmdProc*) BG_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "debug_BG", (char *)&debug_BG, TCL_LINK_INT);

/* Utility stuff */
Tcl_CreateCommand(interp,"Utility_Cmd", (Tcl_CmdProc*) Utility_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "debug_Utility", (char *)&debug_Utility, TCL_LINK_INT);
