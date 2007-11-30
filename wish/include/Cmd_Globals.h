
/*
  Cmd_Globals.c

  Declare global variables shared between C and Tcl/Tk.

  The definitions in this file are needed to create global variables
    shared by Tcl and C.  Thus, for the wish shell used to create the MMTAO
    GUI all communications and calculations are processed by compiled C code
    and TckTk is only used to create the Graphic User Interface (GUI),

  This file is included in the header of AO_wish.c

  Written  26aug03  DLM

  Altered   2feb07  DLM  Removed Sky globals and added BG globals for PCR_Wish

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

/* Static Stuff */
int debug_Static;

int Static_Matrix_Read;

/* Modulation Stuff */
int debug_Mod;

int Mod_Matrix_Read;

/* Background Stuff */
int debug_BG;

/* Utility Stuff */
int debug_Utility;
