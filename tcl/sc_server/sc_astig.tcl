proc sc_astig { sock astig_0 astig_45 parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl
    source $PCR_HOME/tcl/static/static_globals.tcl

    set msg "Appy this request?"
    set msg "$msg\n   => Static"
    set msg "$msg\n   => 0 Astig  = $astig_0"
    set msg "$msg\n   => 45 Astig = $astig_45"

    SC_Log "sc_astig : $msg"

    if { $SC_Prompt } {
	set status \
	    [tk_messageBox -parent $parent_win -icon question -type yesno \
		 -message $msg ]
    } else {
	set status "yes"
    }

    if { $status == "yes" } {

	set Static_Value(Static_Astig_0) $astig_0
	set Static_Value(Static_Astig_45) $astig_45

	set status [ static_apply $Base $File $parent_win ]

	if { $status } {
	    SC_Log "sc_astig: Error: Could not send Astigmatism to the DM, not implemented yet"
	    SC_Write $sock "Error: Could not send Astigmatism to the DM, not implemented yet"
	    return -1
	} else {
	    SC_Log "sc_astig: OK: Astigmatism application complete"
	    SC_Write $sock "Done: Astigmatism application complete"
	}

    } else {
	SC_Log "sc_astig: Error: Astigmatism refused"
	SC_Write $sock "Error: Astigmatism refused"
    }

    return 0
}
