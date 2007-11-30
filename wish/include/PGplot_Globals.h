
/*
  PGplot_Globals.c

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

/* Global Variables */

int debug_PGplot;

/* Structure holding information about the spots PGplot /xtk window */
PGplot_Info *WFSC_Info;
PGplot_Info *Spots_Info;
PGplot_Info *Slopes_Info;

/* Flags for Spots display */
int Spots_Number;
int New_Spot;
int Spots_Type;
int Spots_Slopes;
int Spots_Arrow;

/* Flags for Slopes display */
int Slopes_Number;
int New_Slope;
int Slopes_Type;
int Slopes_Arrow;
