proc SC_Log { msg } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    set time_date [ exec date -u +%T ]
    set debug_SC 1
    if { $debug_SC } {
	puts "$time_date $msg"
    }

    puts $SC_Log_Sock "$time_date $msg"
    flush $SC_Log_Sock

}
