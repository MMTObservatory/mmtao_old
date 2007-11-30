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
# Flag to continously update VoltMeter information
#
global VoltMeter_Update
#
# Variables set by AO_Wish
#
global VoltMeter_Connected
global VoltMeter_Error
#
# Array variables.
#   **Note: Only the array should be made global, not individual elementes
#           The element are defined in voltmeter_globals_set.tcl
#
global Resistance_Channels
global Resistance
global Resistance_Format
global Resistance_Units
#
# Set the list of channel names
#
set Resistance_Channels [ list 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 ]
#
foreach el $Resistance_Channels {
    global Resistance_$el
}
