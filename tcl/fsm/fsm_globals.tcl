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
#####################################################
#
# Field Steering Mirror (FSM)
#
#####################################################
#
# Default camera for FSM motion
#
global FSM_Camera
#
# Default increments for arrows for OAP motion
#
global FSM_Incr
