###################################
#
# save_globals_set.tcl
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
# Set the local display and utility variables
#
set status [catch { Utility_Cmd UT_Time } msg ]
if { $status } {
    tk_messageBox -message $msg -parent $save_page -icon error -type ok
}
set current_time [lindex $reply_data 1]
#
set WFSC_Time ${current_time}
set WFSC_File "wfsc_${current_time}.fits"
#
set DDS_Time ${current_time}
set DDS_File "dds_${current_time}fits"
#
set Save_PCR_Data 0
