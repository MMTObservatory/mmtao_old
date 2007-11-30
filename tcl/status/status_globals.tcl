###################################
#
# status_globals.tcl
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
global Status_Connected
global Status_Error
global Status_Reply

global Status_Int
global Status_Float
global Status_Char
global Status_List
#
# Array variables.
#   **Note: Only the array should be made global, not individual elementes
#           The element are defined in status_globals_set.tcl
#
global Status
global Status_Format
#
# This variable is set after all Status variables have been set so the gui
#  can be updated according to the new values of the Status variables.  The
#  update routines are in tcl/gui/gui_update.tcl
#
global Status_Update
