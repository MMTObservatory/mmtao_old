
proc sc_looper { parent_win } {
      
    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    info_get all $parent_win
    tcs_get all $parent_win
    Motor_Client all

    if { $SC_Update } {
	after 1000 sc_looper $parent_win
    }

}
