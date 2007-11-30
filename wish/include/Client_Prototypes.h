
/*
  Client_Prototypes.c

  Declare new routines in Tcl/Tk.

  The definitions in this file are needed to define new commands in Tcl.  Thus,
    for the wish shell used to create the MMTAO GUI all communications
    and calculations are processed by compiled C code and TckTk is only
    used to create the Graphic User Interface (GUI),

  This file is included in the header of AO_wish.c

  Written  26aug03  DLM

  Altered   2feb07  DLM  Put BG_Cmd in Cmd_Prototype.h for PCR_Wish

======================================================================================

  Prototypes of new Tcl commands.

  In Tcl, these new commands have the form:

    Tcl_Command argv[0] argv[1] argv[2]...

  In C, the requested routine is called and the Tcl arguments are available
    in:
      char **argv
    with the number of passed arguments in
      int argc

  The number of arguements is predetermined and should be checked in the
    called routine.  If an incorrect number of arguments are found, an error
    is returned to the calling Tcl code and an error message in replyData

  Each of the new routines listed below can be found in the corresponding directory.
    For example:

      FSM_Cmd  --> ../FSM/FSM_Cmd.c

=========================================================*/

/* Prototypes of New Tcl Commands */

int DClient_Cmd (ClientData client_data, Tcl_Interp* interp, int argc, char **argv);
int Info_Cmd (ClientData client_data, Tcl_Interp* interp, int argc, char **argv);
int PCR_Cmd (ClientData client_data, Tcl_Interp* interp, int argc, char **argv);
int TCS_Cmd (ClientData client_data, Tcl_Interp* interp, int argc, char **argv);
int Hex_Cmd (ClientData client_data, Tcl_Interp* interp, int argc, char **argv);
int Status_Cmd (ClientData client_data, Tcl_Interp* interp, int argc, char **argv);
int Topbox_Cmd (ClientData client_data, Tcl_Interp* interp, int argc, char **argv);
