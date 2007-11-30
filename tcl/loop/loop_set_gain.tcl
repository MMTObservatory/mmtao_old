
proc loop_set_gain { gain parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/loop/loop_globals.tcl

    upvar #0 Info Info

    if { !$Info(Loop_Running) } {
	tk_messageBox -message "Cannot change the Loop Gain. AO Loop is not running" \
	    -parent $parent_win -icon error -type ok
	return -1
    }

    if { $gain < $Loop_Gain_Minimum } {
	return 0
    }

    if { $gain >= $Loop_Gain_Maximum } {
	return 0
    }

    set loop_gain $gain

    set status [catch { PCR_Cmd Loop_Gain $loop_gain } msg ]
    if { $status }  {
	tk_messageBox -message "Error changing loop gain" \
	    -parent $parent_win -icon error -type ok
    }

    return 0

}
