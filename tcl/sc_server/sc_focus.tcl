proc sc_focus { sock hex parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    set msg "Appy this request?"
    set msg "$msg\n   => Focus"
    set msg "$msg\n   => $hex"

    SC_Log "sc_focus : $msg"

    if { $SC_Prompt } {
	set status \
	    [tk_messageBox -parent $parent_win -icon question -type yesno \
		 -message $msg ]
    } else {
	set status "yes"
    }

    if { $status == "yes" } {

	if { [expr abs($hex)] <= 10 } {

	    Motor_Client move Focus $hex
	    SC_Log "sc_focus: OK: Focus of $hex microns complete"
	    SC_Write $sock "Done: Focus complete"
	    set SC_Focus [expr $SC_Focus + $hex]
	    puts $SC_Focus


	} else {

	    SC_Log "sc_focus: Error: Focus request of $hex too large (<=10)"
	    SC_Write $sock "Error: Focus request of $hex too large (<=10)"
	    return -1

	}

    } else {
	SC_Log "sc_focus: Error: Focus refused"
	SC_Write $sock "Error: Focus refused"
    }

    return 0
}
