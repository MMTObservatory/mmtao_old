proc sc_processing { sock } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl
    source $PCR_HOME/tcl/tcs/tcs_globals.tcl

    upvar #0 TCS TCS
    upvar #0 Info Info
#
# Give a response anytime (SC_Accept = 0,1) to Informative requests
#  and reply with Error: for action request if accepting of requests
#  is off
#
#
# Check the request as a lower case string
#
    set request [ string tolower $SC_Request(0) ]

    if {  $request == "all" } {

#	SC_Log "Requesting Status"
	foreach el $TCS_List {
	    SC_Write $sock "$el $TCS($el)"
	}
	foreach el $SC_Info_List {
	    SC_Write $sock "$el $Info($el)"
	}
	SC_Write $sock "Hexapod_Focus 0"
	SC_Write $sock "RTR_Reconstructor rtr_56m"
	SC_Write $sock "SC_Accepting 1"
	SC_Write $sock "EOF"

    } elseif {  $request == "set_home" } {

	if { $SC_Parameters != 1 } {

	    SC_Log "sc_processing: Error: Set_Home requires no parameters"
	    SC_Write $sock "Error: Set_Home requires no parameters"

	} elseif { $SC_Accept } {

	    SC_Write $sock "OK: Processing Set_Home..."
	    
	    set status [catch { sc_set_home $sock .sc_win } msg ]
	    if { $status } {
		SC_Log $msg
	    }

	} else {

	    SC_Log "sc_processing: Error: Not accepting Science Camera requests"
	    SC_Write $sock "Error: Not accepting Science Camera requests"

	}

    } elseif {  $request == "go_home" } {

	if { $SC_Parameters != 1 } {

	    SC_Log "sc_processing: Error: Go_Home requires no parameters"
	    SC_Write $sock "Error: Go_Home requires no parameters"

	} elseif { $SC_Accept } {

	    SC_Write $sock "OK: Processing Go_Home..."
	    
	    set status [catch { sc_go_home $sock .sc_win } msg ]
	    if { $status } {
		SC_Log $msg
	    }

	} else {

	    SC_Log "sc_processing: Error: Not accepting Science Camera requests"
	    SC_Write $sock "Error: Not accepting Science Camera requests"

	}

    } elseif {  $request == "offset" } {

	if { $SC_Parameters != 3 } {

	    SC_Log "sc_processing: Error: Offset request requires 2 parameters"
	    SC_Write $sock "Error: Offset request requires 2 parameters"

	} elseif { $SC_Accept } {

	    SC_Write $sock "OK: Processing Offset..."

	    if { $Info(Loop_Running) } {
		set status [catch { sc_offset_closed $sock $SC_Request(1) $SC_Request(2) .sc_win } msg ]
	    } else {
		set status [catch { sc_offset_open $sock $SC_Request(1) $SC_Request(2) .sc_win } msg ]
	    }
	    if { $status } {
		SC_Log "sc_processing: $msg"
	    }

	} else {

	    SC_Log "sc_processing: Error: Request for Offset_Closed but not accepting Science Camera requests"
	    SC_Write $sock "Error: Request for Offset_Closed but not accepting Science Camera requests"

	}

    } elseif {  $request == "focus" } {

	if { $SC_Parameters != 2 } {

	    SC_Log "sc_processing: Error: Focus request requires 1 parameters"
	    SC_Write $sock "Error: Focus request requires 1 parameters"

	} elseif { $SC_Accept } {

	    SC_Write $sock "OK: Processing Focus..."

	    set status [catch { sc_focus $sock $SC_Request(1) .sc_win } msg ]
	    if { $status } {
		SC_Log "sc_processing: $msg"
	    }

	} else {

	    SC_Log "sc_processing: Error: Request for Focus but not accepting Science Camera requests"
	    SC_Write $sock "Error: Request for Focus but not accepting Science Camera requests"

	}

    } elseif {  $request == "static" } {

	if { $SC_Parameters != 3 } {

	    SC_Log "sc_processing: Error: Astigmatism request requires 2 parameters"
	    SC_Write $sock "Error: Astigmatism request requires 2 parameters"

	} elseif { $SC_Accept } {

	    set status [catch { sc_astig $sock $SC_Request(1) $SC_Request(2) .sc_win } msg ]
	    if { $status } {
		SC_Log "sc_processing: $msg"
	    }

	} else {

	    SC_Log "sc_processing: Error: Request for Astigmatism but not accepting Science Camera requests"
	    SC_Write $sock "Error: Request for Astigmatism but not accepting Science Camera requests"

	}

    } else {

	SC_Log "sc_processing: Error: Unknown request: $SC_Request(0)"
	SC_Write $sock "Error: Unknown request: $SC_Request(0)"

    }

    for {set i 0} {$i < $SC_Parameters} { incr i } {
	set SC_Request($i) --
    }
    set SC_Parameters 0

}
