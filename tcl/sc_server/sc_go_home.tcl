proc sc_go_home { sock parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    upvar #0 TCS TCS
    upvar #0 Info Info

    set msg "Go to Home position?"

    if { $SC_Prompt } {
	set status \
	    [tk_messageBox -parent $parent_win -icon question -type yesno \
		 -message $msg ]
    } else {
	set status "yes"
    }

    if { $status == "yes" } {

	set ra [expr $SC_RA_Home - $TCS(ao_off_ra) ]
	set dec [expr $SC_DEC_Home - $TCS(ao_off_dec) ]

	if { $Info(Loop_Running) } {
	    sc_offset_closed $sock $ra $dec $parent_win
	} else {
	    sc_offset_open $sock $ra $dec $parent_win
	}

    }

    return 0
}
