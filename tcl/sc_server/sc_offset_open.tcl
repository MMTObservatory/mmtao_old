proc sc_offset_open { sock ra dec parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl
    source $PCR_HOME/tcl/tcs/tcs_globals.tcl
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl
#
    global error_msg
#
    upvar #0 Info Info
    upvar #0 Status Status
#
# Make sure the loop is open or loop gain is set to zero before making the offset
#
    if { $Info(Loop_Running) && $Info(Loop_Gain) != "0.0" } {
	SC_Log "sc_offset_open: Error: Cannot perform a Offset_Open with the loop closed"
	SC_Write $sock "Error: Cannot perform a Offset_Open with the loop closed"
	return -1
    }
	
    set msg "Appy this request?"
    set msg "$msg\n   => Offset_Open"
    set msg "$msg\n   => $ra"
    set msg "$msg\n   => $dec"

    SC_Log "sc_offset_open : $msg"

    if { $SC_Prompt } {
	set status \
	    [tk_messageBox -parent $parent_win -icon question -type yesno \
		 -message $msg ]
    } else {
	set status "yes"
    }

    if { !$Topbox_Connected } {
	SC_Log "sc_offset_open: Error: Not connected to the Topbox Server"
	SC_Write $sock "Error: Not connected to the Topbox Server"
	return -1
    }

    if { !$TCS_Connected } {
	SC_Log "sc_offset_open: Error: Not connected to the TCS"
	SC_Write $sock "Error: Not connected to the TCS"
	return -1
    }

    if { $status == "yes" } {

	if { [expr abs($ra) ] > $SC_Max_Move || [expr abs($dec) ] > $SC_Max_Move } {
	    SC_Log "sc_offset_closed: Error: Requested move too large (>$SC_Max_Move)"
	    SC_Log "                          RA = $ra"
	    SC_Log "                         DEC = $dec"
	    SC_Write $sock "Error: ra or dec too large"
	    return 1
	}

	if { [expr abs($ra)] > $SC_Min_Incr || [expr abs($dec)] > $SC_Min_Incr } {

	    set status [ fsm_move_ra_dec $ra $dec $parent_win ]

	    if { $status } {
		SC_Log "sc_offset_open: Error: Could not Offset_Open the FSM $msg"
		SC_Write $sock "Error: Could not move the FSM"
		return -1
	    }

	    set status [ tcs_set setrdao $ra $dec $parent_win ]

	    if { $status } {
		SC_Log "sc_offset_open: Error: Could not Offset_Open the telescsope $msg"
		SC_Write $sock "Error: Could not move the telescsope"
		return -1
	    }

	}

	SC_Log "sc_offset_open: OK: Offset_Open complete"
	SC_Write $sock "Done: Offset_Open complete"

    } else {
	SC_Log "sc_offset_open: Error: Offset_Open refused"
	SC_Write $sock "Error: Offset_Open refused"
    }

    return 0
}
