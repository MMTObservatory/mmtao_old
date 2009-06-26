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
set Loop_Gain_Increment 0.05
set Loop_Gain_Minimum 0.0
set Loop_Gain_Maximum 1.0
set Loop_PGain_Increment 0.05
set Loop_PGain_Minimum 0.0
set Loop_PGain_Maximum 10.0
set Loop_IGain_Increment 0.05
set Loop_IGain_Minimum 0.0
set Loop_IGain_Maximum 10.0
set Loop_DGain_Increment 0.05
set Loop_DGain_Minimum 0.0
set Loop_DGain_Maximum 10.0
