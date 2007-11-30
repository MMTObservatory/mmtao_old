###################################
#
# global_trace.tcl
#
###################################
#
# The upper most level TCL code must also source this file
#   The trace command is used to force the update of gui
#   variables on all gui panels.
#
# All TCL source codes should source global_variables.tcl
#   in order for the automatic update to take place.
#
#####################################################
#
# Field Steering Mirror (FSM)
#
#####################################################
#
# Default camera for FSM motion
#
set FSM_Camera WFSC
#
# Default increment for FSM motion in arcsec
#
set FSM_Incr 0.3
