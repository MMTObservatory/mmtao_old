proc sc_log_open { filename } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    set SC_Log_Sock [ open $filename a+ ]

    set time_date [ exec date -u ]
    puts $SC_Log_Sock "$time_date Started"
    flush $SC_Log_Sock

    if { $debug_SC } {
	puts "$time_date Starting"
    }

}

proc sc_log_close { } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    set time_date [ exec date -u +%T ]
    puts $SC_Log_Sock "$time_date Exiting"

    if { $debug_SC } {
	puts "$time_date Exiting"
    }

    flush $SC_Log_Sock
    close $SC_Log_Sock

}
