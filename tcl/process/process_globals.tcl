###################################
#
# global_variables
#
###################################
#
# This file is sourced by the upper most level of TCL source code
#   (ie. AO_GUI_startup.tcl) as well as the all lower level of
#   tcl code that need these variables.
#
# The upper most level TCL code must also source the file
#   global_trace.tcl.  In this file, the trace command is used
#   to force the update of gui variables on all gui panels.
#
global Process_Update_Rate

global PCR_Server_Start
global PCR_Server_Address
global PCR_Server_Running

global PCR_Start
global PCR_Address
global PCR_Running
global DDS_Running

global AO_Server_Address
global AO_Server_Running

global Topbox_Server_Start
global Topbox_Server_Running
global Topbox_Server_Address
