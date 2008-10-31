
/*
  Client_Globals.c

  Declare global variables shared between C and Tcl/Tk.

  The definitions in this file are needed to create global variables
    shared by Tcl and C.  Thus, for the wish shell used to create the MMTAO
    GUI all communications and calculations are processed by compiled C code
    and TckTk is only used to create the Graphic User Interface (GUI),

  This file is included in the header of AO_wish.c

  Written  26aug03  DLM

  Altered   2feb07  DLM  Put BG globals in Cmd_Globals.h for PCR_Wish

======================================================================================

  Variables global to C and Tcl

  The convention used is only debug variables and a replyData string are passed
    back and forth from C to Tcl via global.  All other information is passed
    via arguments to the Tcl command and a returned status (int) from C.

  This way it is clear to the programmer what data is being passed, and decisions
    as to how to use this information (ie. automatically update displays) must and
    will be done manually in the calling Tcl code.

=========================================================*/

/*************************
 *   PCR Server  *
 *************************/

Server_Info *PCR_Info;

int debug_PCR;

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
int PCR_Connected;
int PCR_Error;

/*********************
 *   DClient         *
 *********************/

char *reply_data;

/*********************
 *   DClient         *
 *********************/

int debug_DClient;
int debug_DServ;

/*********************
 *   Info Server      *
 *********************/
int debug_Info;

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
int Info_Connected;
int Info_Error;

/*
  A very long string to send Info string from C to Tcl
*/
char *Info_Reply;

/*
  Send back the average seeing value
*/
double Seeing_Average;

/*************************
 *   Topbox Server  *
 *************************/

Client_Info *Topbox_Info;

int debug_Topbox;

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
int Topbox_Connected;
int Topbox_Error;

/*********************
 *   Status Server      *
 *********************/
int debug_Status;

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
int Status_Connected;
int Status_Error;

/*
  A very long string to send Status string from C to Tcl
*/
char *Status_Reply;

/*********************
 *   Science Server      *
 *********************/
int debug_Science;

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
int Science_Connected;
int Science_Error;

/*
  A very long string to send Science string from C to Tcl
*/
char *Science_Reply;

/*********************
 *   TCS Server      *
 *********************/

char *TCS_Address;
int TCS_Port;

int debug_TCS;

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
int TCS_Connected;
int TCS_Error;

/*
  A very long string to send TCS string from C to Tcl
*/
char *TCS_Reply;

/*********************
 *   Hexapod Server  *
 *********************/

char *Hex_Address;
int Hex_Port;

int debug_Hex;

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
int Hex_Connected;
int Hex_Error;

/*
  A very long string to send Hexapod string from C to Tcl
*/
char *Hex_Reply;
