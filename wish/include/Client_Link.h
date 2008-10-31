/*
  AO_Wish_WFCLink.h

  Link global variables and new routines between Tcl and C

  The definitions in this file are needed to create global variables
    shared by Tcl and C, and to define new commands in Tcl.  Thus,
    for the wish shell used to create the MMTAO GUI all communications
    and calculations are processed by compiled C code and TckTk is only
    used to create the Graphic User Interface (GUI),

  This file is included in the **body of Tcl_AppInit()** of AO_wish.c

  Written  26aug03  DLM

  Altered   2feb07  DLM  Put BG links in Cmd_Globals.h for PCR_Wish

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  See description in ./Client_Globals.c and ../PCR_Wish.c

=========================================================*/

/* Link Tcl Commands to C Routines and associated global variables */

/*******************************
 * PCR Server Stuff *
 *******************************/
Tcl_CreateCommand(interp,"PCR_Cmd", (Tcl_CmdProc*) PCR_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "debug_PCR", (char *)&debug_PCR, TCL_LINK_INT);

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
Tcl_LinkVar(interp, "PCR_Connected", (char *)&PCR_Connected, TCL_LINK_INT);
Tcl_LinkVar(interp, "PCR_Error", (char *)&PCR_Error, TCL_LINK_INT);

/*
  This structure that holds information about the connection to the PCR_Server
    is created here so it can be used as other routines 
    (eg external Server_Info *PCR_Info)
*/
PCR_Info = (Server_Info *)malloc(sizeof(Server_Info));

/*******************************************************
 * String to send a reply back to the Tcl routines *
 *******************************************************/

/*
  The variable reply_data is used to pass information from these C
    routines back to the TclTk application.  Thus, these variables
    MUST NOT be set in any of the TclTk procedures.  Instead, memory
    is allocated here so C can modify it's contents and the location
    of this memory (the variable) is availble for use in TclTk.
*/

reply_data = Tcl_Alloc( IO_LENGTH );
// Must access the string created here before it can be linked back to a 
//   Tcl variable
strcpy(reply_data, "junk");
Tcl_LinkVar(interp, "reply_data", (char*) &reply_data, TCL_LINK_STRING);

/***********
 * DClient *
 **********/
Tcl_CreateCommand(interp,"DClient_Cmd", (Tcl_CmdProc*) DClient_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "debug_DClient", (char *)&debug_DClient, TCL_LINK_INT);
Tcl_LinkVar(interp, "debug_DServ", (char *)&debug_DServ, TCL_LINK_INT);

/**************
 * Info Server *
 **************/
Tcl_CreateCommand(interp,"Info_Cmd", (Tcl_CmdProc*) Info_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "debug_Info", (char *)&debug_Info, TCL_LINK_INT);

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
Tcl_LinkVar(interp, "Info_Connected", (char *)&Info_Connected, TCL_LINK_INT);
Tcl_LinkVar(interp, "Info_Error", (char *)&Info_Error, TCL_LINK_INT);

Info_Reply = Tcl_Alloc( CLIENT_REPLY_LENGTH );
// Must access the string created here before it can be linked back to a 
//   Tcl variable
strcpy(Info_Reply, "EOF");
Tcl_LinkVar(interp, "Info_Reply", (char*) &Info_Reply, TCL_LINK_STRING);

/*
  Send Tcl the current average seeing value
*/
Tcl_LinkVar(interp, "Seeing_Average", (char *)&Seeing_Average, TCL_LINK_DOUBLE);

/*******************************
 * Topbox Server Stuff *
 *******************************/
Tcl_CreateCommand(interp,"Topbox_Cmd", (Tcl_CmdProc*) Topbox_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "debug_Topbox", (char *)&debug_Topbox, TCL_LINK_INT);

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
Tcl_LinkVar(interp, "Topbox_Connected", (char *)&Topbox_Connected, TCL_LINK_INT);
Tcl_LinkVar(interp, "Topbox_Error", (char *)&Topbox_Error, TCL_LINK_INT);

/*
  This structure that holds information about the connection to the Topbox_Server
    is created here so it can be used as other routines 
    (eg external Server_Info *Topbox_Info)
*/
Topbox_Info = (Client_Info *)malloc(sizeof(Server_Info));

/**************
 * Status Server *
 **************/
Tcl_CreateCommand(interp,"Status_Cmd", (Tcl_CmdProc*) Status_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "debug_Status", (char *)&debug_Status, TCL_LINK_INT);

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
Tcl_LinkVar(interp, "Status_Connected", (char *)&Status_Connected, TCL_LINK_INT);
Tcl_LinkVar(interp, "Status_Error", (char *)&Status_Error, TCL_LINK_INT);

Status_Reply = Tcl_Alloc( CLIENT_REPLY_LENGTH );
// Must access the string created here before it can be linked back to a 
//   Tcl variable
strcpy(Status_Reply, "EOF");
Tcl_LinkVar(interp, "Status_Reply", (char*) &Status_Reply, TCL_LINK_STRING);

/**************
 * Science Server *
 **************/
Tcl_CreateCommand(interp,"Science_Cmd", (Tcl_CmdProc*) Science_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "debug_Science", (char *)&debug_Science, TCL_LINK_INT);

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
Tcl_LinkVar(interp, "Science_Connected", (char *)&Science_Connected, TCL_LINK_INT);
Tcl_LinkVar(interp, "Science_Error", (char *)&Science_Error, TCL_LINK_INT);

Science_Reply = Tcl_Alloc( CLIENT_REPLY_LENGTH );
// Must access the string created here before it can be linked back to a 
//   Tcl variable
strcpy(Science_Reply, "EOF");
Tcl_LinkVar(interp, "Science_Reply", (char*) &Science_Reply, TCL_LINK_STRING);

/**********************/
/* TCS Stage Stuff */
/**********************/
Tcl_CreateCommand(interp,"TCS_Cmd", (Tcl_CmdProc*) TCS_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "TCS_Address", (char*) &TCS_Address, TCL_LINK_STRING);
Tcl_LinkVar(interp, "TCS_Port", (char *)&TCS_Port, TCL_LINK_INT);

Tcl_LinkVar(interp, "debug_TCS", (char *)&debug_TCS, TCL_LINK_INT);

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
Tcl_LinkVar(interp, "TCS_Connected", (char *)&TCS_Connected, TCL_LINK_INT);
Tcl_LinkVar(interp, "TCS_Error", (char *)&TCS_Error, TCL_LINK_INT);

TCS_Reply = Tcl_Alloc( CLIENT_REPLY_LENGTH );
// Must access the string created here before it can be linked back to a 
//   Tcl variable
strcpy(TCS_Reply, ".EOF");
Tcl_LinkVar(interp, "TCS_Reply", (char*) &TCS_Reply, TCL_LINK_STRING);

/************************/
/* Hexapod Server Stuff */
/************************/
Tcl_CreateCommand(interp,"Hex_Cmd", (Tcl_CmdProc*) Hex_Cmd,
                 (ClientData) NULL, (Tcl_CmdDeleteProc*) NULL);

Tcl_LinkVar(interp, "Hex_Address", (char*) &Hex_Address, TCL_LINK_STRING);
Tcl_LinkVar(interp, "Hex_Port", (char *)&Hex_Port, TCL_LINK_INT);

Tcl_LinkVar(interp, "debug_Hex", (char *)&debug_Hex, TCL_LINK_INT);

/*
  State variables with a link between C and TclTk variables so
    information changed in C will be reflected on the GUI
*/
Tcl_LinkVar(interp, "Hex_Connected", (char *)&Hex_Connected, TCL_LINK_INT);
Tcl_LinkVar(interp, "Hex_Error", (char *)&Hex_Error, TCL_LINK_INT);

Hex_Reply = Tcl_Alloc( CLIENT_REPLY_LENGTH );
// Must access the string created here before it can be linked back to a 
//   Tcl variable
strcpy(Hex_Reply, ".EOF");
Tcl_LinkVar(interp, "Hex_Reply", (char*) &Hex_Reply, TCL_LINK_STRING);
