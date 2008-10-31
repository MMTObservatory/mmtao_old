###################################
#
# science_globals.tcl
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
global Science_Connected
global Science_Error
global Science_Reply

global Science_Int
global Science_Float
global Science_Char
global Science_List
#
# Array variables.
#   **Note: Only the array should be made global, not individual elementes
#           The element are defined in science_globals_set.tcl
#
global Science
global Science_Format
#
# This variable is set after all Science variables have been set so the gui
#  can be updated according to the new values of the Science variables.  The
#  update routines are in tcl/gui/gui_update.tcl
#
global Science_Update
