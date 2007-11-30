###################################
#
# dclient_globals.tcl
#
###################################
#
# The *_globals.tcl file is sourced by the upper most level of TCL
#   source code (ie. AO_GUI_startup.tcl) as well as the all lower
#   level of tcl code that need these variables.
#
# The *_globals_set.tcl file is sourced by only the upper most level
#   TCL code (usually *_startup.tcl) so values are set to the global
#   variables only once.  In *_globals_set.tcl file, the trace command
#   can be used to force the update of gui variables, colors, buttons,
#   etc on all gui panels.
#
###################################
#
global DClient_DM_Save
global DClient_DM_Save_List

global DClient_WFSC_Frames
global DClient_DDS_Frames

global DClient_All

global DClient_List
#global Display_RT
#global Display_RT_Rate
