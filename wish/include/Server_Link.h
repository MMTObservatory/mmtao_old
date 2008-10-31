/*
  Server_Link.h

  Link the Server Names and Ports to Tcl variables.  These valuess
    are #defined in the include file ../../include/Server_Ports.h.
    This way the addresses for the need servers (PCR, DDS, PCR_Server
    and AO_Server) that are define in the C routines are available
    for the process_*.tcl routines

  Written   1aug07  DLM

+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  See description in ./Client_Globals.c and ../PCR_Wish.c

=========================================================*/

/*
  PCR_Server Address
*/

PCR_Server_Address = Tcl_Alloc( STRING_LENGTH );
strcpy(PCR_Server_Address, PCR_SERVER_ADDRESS);
Tcl_LinkVar(interp, "PCR_Server_Address", (char*) &PCR_Server_Address, TCL_LINK_STRING);

/*
  PCR Address
*/

PCR_Address = Tcl_Alloc( STRING_LENGTH );
strcpy(PCR_Address, PCR_ADDRESS);
Tcl_LinkVar(interp, "PCR_Address", (char*) &PCR_Address, TCL_LINK_STRING);

/*
  Topbox Server Address
*/

Topbox_Server_Address = Tcl_Alloc( STRING_LENGTH );
strcpy(Topbox_Server_Address, TOPBOX_SERVER_ADDRESS);
Tcl_LinkVar(interp, "Topbox_Server_Address", (char*) &Topbox_Server_Address, TCL_LINK_STRING);

/*
  Science Server Address
*/

Science_Server_Address = Tcl_Alloc( STRING_LENGTH );
strcpy(Science_Server_Address, SCIENCE_SERVER_ADDRESS);
Tcl_LinkVar(interp, "Science_Server_Address", (char*) &Science_Server_Address, TCL_LINK_STRING);
