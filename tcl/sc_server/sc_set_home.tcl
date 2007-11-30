proc sc_set_home { sock parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    upvar #0 TCS TCS
    upvar #0 Status Status

    set msg "Set current position at Home?"

    if { $SC_Prompt } {
	set status \
	    [tk_messageBox -parent $parent_win -icon question -type yesno \
		 -message $msg ]
    } else {
	set status "yes"
    }

    if { $status == "yes" } {

	set status [ status_get all $parent_win ]

	set SC_Tip_Home $Status(FSM_Tip_Position)
	set SC_Tip_Off [expr $Status(FSM_Tip_Position) - $SC_Tip_Home]

	set SC_Tilt_Home $Status(FSM_Tilt_Position)
	set SC_Tilt_Off [expr $Status(FSM_Tilt_Position) - $SC_Tip_Off]

	tcs_get all $parent_win
	set SC_RA_Home $TCS(ao_off_ra)
	set SC_DEC_Home $TCS(ao_off_dec)

	set SC_Home_Set 1

	SC_Log "sc_set_home: Done: Home Set"
	SC_Log "     => SC_Tip_Home  = $SC_Tip_Home"
	SC_Log "     => SC_Tilt_Home = $SC_Tilt_Home"
	SC_Log "     => SC_RA_Home   = $SC_RA_Home"
	SC_Log "     => SC_DEC_Home  = $SC_DEC_Home"

	SC_Write $sock "Done: Home Set"

    } else {

	SC_Log "sc_set_home: Error: Set_Home request refused"
	SC_Write $sock "Error: Set_Home request refused"

    }

    return 0
}
