
/*
  PGplot_Link.h

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

/* PGplot stuff */

Tcl_CreateCommand(interp,"PGplot_Cmd", (Tcl_CmdProc*) PGplot_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "debug_PGplot", (char *)&debug_PGplot, TCL_LINK_INT);

/*
  This structure that holds information about the Spots PGplot /xtk window
    so it is created here so other routines can use it 
    (eg extern PGlot_Info *Spots_Info)
*/
WFSC_Info = (PGplot_Info *)malloc(sizeof(PGplot_Info));
Spots_Info = (PGplot_Info *)malloc(sizeof(PGplot_Info));

/* Flags for Spots display */
Tcl_LinkVar(interp, "Spots_Number", (char *)&Spots_Number, TCL_LINK_INT);
Tcl_LinkVar(interp, "New_Spot", (char *)&New_Spot, TCL_LINK_INT);
Tcl_LinkVar(interp, "Spots_Type", (char *)&Spots_Type, TCL_LINK_INT);
Tcl_LinkVar(interp, "Spots_Slopes", (char *)&Spots_Slopes, TCL_LINK_INT);
Tcl_LinkVar(interp, "Spots_Arrow", (char *)&Spots_Arrow, TCL_LINK_INT);

/*
  This structure that holds information about the Slopes PGplot /xtk window
    so it is created here so other routines can use it 
    (eg extern PGlot_Info *Slopes_Info)
*/
Slopes_Info = (PGplot_Info *)malloc(sizeof(PGplot_Info));

/* Flags for Slopes display */
Tcl_LinkVar(interp, "Slopes_Number", (char *)&Slopes_Number, TCL_LINK_INT);
Tcl_LinkVar(interp, "New_Slope", (char *)&New_Slope, TCL_LINK_INT);
Tcl_LinkVar(interp, "Slopes_Type", (char *)&Slopes_Type, TCL_LINK_INT);
Tcl_LinkVar(interp, "Slopes_Arrow", (char *)&Slopes_Arrow, TCL_LINK_INT);
