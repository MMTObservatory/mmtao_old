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
set Process_Update_Rate 1000

set PCR_Server_Running 0

set PCR_Running 0
set DDS_Running 0

set Topbox_Server_Running 0

set Science_Server_Running 0

set TSS_Server_Running 0
