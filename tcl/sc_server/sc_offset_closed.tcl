proc sc_offset_closed { sock ra dec parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl
    source $PCR_HOME/tcl/tcs/tcs_globals.tcl
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl
#
    upvar #0 Info Info
    upvar #0 Status Status

    set msg "Appy this request?"
    set msg "$msg\n   => Offset_Closed"
    set msg "$msg\n   => $ra"
    set msg "$msg\n   => $dec"

    SC_Log "sc_offset_closed : $msg"

    if { $SC_Prompt } {
	set status \
	    [tk_messageBox -parent $parent_win -icon question -type yesno \
		 -message $msg ]
    } else {
	set status yes
    }

    if { !$Topbox_Connected } {
	SC_Log "sc_offset_closed: Error: Not connected to the Topbox Server"
	SC_Write $sock "Error: Not connected to the Topbox Server"
	return -1
    }

    if { !$TCS_Connected } {
	SC_Log "sc_offset_closed: Error: Not connected to the TCS"
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

	if { [expr sqrt( $ra*$ra + $dec*$dec ) ] > $SC_Max_Closed } {
	    #
	    # Store current value of user gain 
	    #
	    set SC_User_Gain $Info(Loop_Gain)
	    #
	    # Set gain to 0
	    #
	    set status [ loop_set_gain 0.0 $parent_win ]
	    if { $status } {
		SC_Log "sc_offset_closed: Error: Could not set loop gain to 0.0"
		SC_Write $sock "Error: Could not set loop gain to 0.0"
		return -1
	    }
	    SC_Log "sc_offset_closed: Loop gain set to 0.0, waiting $SC_Delay_1 milliseconds"
	    after $SC_Delay_1
	    #
	    # Move the telescope and FSM
	    #
	    set status [ catch { fsm_move_ra_dec $ra $dec $parent_win } msg ]
	    if { $status } {
		SC_Log "sc_offset_closed: Error: Could not Offset_Closed the FSM"
		SC_Log $msg
		SC_Write $sock "Error: Could not move the FSM"
		return -1
	    }

	    SC_Log "sc_offset_closed: FSM moved, waiting $SC_Delay_2 milliseconds"
	    after $SC_Delay_2

	    set status [ tcs_set setrdao $ra $dec $parent_win ]
	    if { $status } {
		SC_Log "sc_offset_closed: Error: Could not Offset_Closed the telescsope"
		SC_Log $msg
		SC_Write $sock "Error: Could not move the telescsope"
		return -1
	    }

	    SC_Log "sc_offset_closed: TCS moved, waiting $SC_Delay_3 milliseconds"
	    after $SC_Delay_3

	    # Set gain to the original value
	    #
	    set status [ loop_set_gain 0.05 $parent_win ]
	    if { $status } {
		SC_Log "sc_offset_closed: Error: Could not set loop gain to 0.05"
		SC_Log $msg
		SC_Write $sock "Error: Could not set loop gain to 0.05"
		return -1
	    }

	    SC_Log "sc_offset_closed: Gain set to 0.05, waiting $SC_Delay_4 milliseconds"
	    after $SC_Delay_4

	    # Set gain to the original value
	    #
	    set status [ loop_set_gain $SC_User_Gain $parent_win ]
	    if { $status } {
		SC_Log "sc_offset_closed: Error: Could not set loop gain to $SC_User_Gain"
		SC_Log $msg
		SC_Write $sock "Error: Could not set loop gain to $SC_User_Gain"
		return -1
	    }

	    SC_Log "sc_offset_closed: Loop gain set to $SC_User_Gain, waiting $SC_Delay_5 milliseconds"
	    after $SC_Delay_5

	} else {

	    while { [expr abs($ra)] > $SC_Min_Incr || [expr abs($dec)] > $SC_Min_Incr } {

		if { $ra > $SC_Max_Incr } {
		    set delta_ra $SC_Max_Incr
		} elseif { $ra < [expr $SC_Max_Incr * -1 ] } {
		    set delta_ra [expr $SC_Max_Incr * -1 ]
		} else {
		    set delta_ra $ra
		}
		set ra [expr $ra - $delta_ra]

		if { $dec > $SC_Max_Incr } {
		    set delta_dec $SC_Max_Incr
		} elseif { $dec < [expr $SC_Max_Incr * -1 ] } {
		    set delta_dec [expr $SC_Max_Incr * -1 ]
		} else {
		    set delta_dec $dec
		}
		set dec [expr $dec - $delta_dec]

		set status [ fsm_move_ra_dec $delta_ra $delta_dec $parent_win ]
		if { $status } {
		    SC_Log "sc_offset_closed: Error: Could not Offset_Closed the FSM"
		    SC_Log $msg
		    SC_Write $sock "Error: Could not move the FSM"
		    return -1
		}

		set status [ tcs_set setrdao $delta_ra $delta_dec $parent_win ]
		if { $status } {
		    SC_Log "sc_offset_closed: Error: Could not Offset_Closed the telescsope"
		    SC_Log $msg
		    SC_Write $sock "Error: Could not move the telescsope"
		    return -1
		}

		after $SC_Wait

	    }
	}

	SC_Log "sc_offset_closed: OK: Offset_Closed complete"
	SC_Write $sock "Done: Offset_Closed complete"

    } else {
	SC_Log "sc_offset_closed: Error: Offset_Closed refused"
	SC_Write $sock "Error: Offset_Closed refused"
    }

    return 0
}
