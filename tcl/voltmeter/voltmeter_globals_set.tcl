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
# Flag to continously update FSM information
#
set VoltMeter_Update 0
#
# VoltMeter status variables
#
set VoltMeter_Connected 0
set VoltMeter_Error 0
#
# Set the list of channel names
#
set Resistance_Channels [ list 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 ]
#
# Set all default values
#
foreach el $Resistance_Channels {
    set Resistance($el) -1
    set Resistance_Format($el) "%10.2f"
    set Resistance_Units($el) "KOhm"
}

set Resistance_Units(02) "Ohm"
