###################################
#
# voltmeter_measure.tcl
#
###################################
#
# Request the HP VoltMeter to update all measured values
#
# Written  12apr04
#

proc voltmeter_measure { parent_win } {

    global AO_GUI_HOME
    source $AO_GUI_HOME/tcl/voltmeter/voltmeter_globals.tcl

    global reply_data

    set status [catch { VoltMeter_Cmd Measure } msg ]
    if { $status } {
	tk_messageBox -message \
	    "Error retrieving measurements from the HP VoltMeter\n $reply_data" \
	    -parent $parent_win \
	    -icon error -type ok
	return -1
    }
#
# Update displayed variables after all have been updated by the C routines.
#
    foreach el $Resistance_Channels {
	set value "Resistance_${el}"
	set Resistance($el) [format "%.2f" [expr $[lindex $value 0]] ]
    }

    return 0
}
