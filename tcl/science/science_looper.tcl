###################################
#
# science_looper.tcl
#
###################################
#
# 2008-10-28 skip
#

proc science_looper { parent_win } {
      
    global PCR_HOME
    source $PCR_HOME/tcl/science/science_globals.tcl
    source $PCR_HOME/tcl/client/client_globals.tcl
    source $PCR_HOME/tcl/process/process_globals.tcl

    if { $Science_Connected } {
	set status [ science_get all $parent_win ]
	if { $status } {
	    set status [ catch { exec query_process science_server } msg ]
	    if { $msg == "0" } {
		set Science_Server_Running 0
	    } elseif { $msg == "1" } {
		set Science_Server_Running 1
	    } else {
		puts "  science_looper: $msg"
		set Science_Server_Running 0
	    }
	}
    } else {
	science_connect $parent_win	
	if { $Science_Connected } {
	    set Science_Server_Running 1
	    set status [ science_get all $parent_win ]
	} else {
	    set Science_Server_Running 0
	    source $PCR_HOME/tcl/science/science_globals_set.tcl
	}
    }

    after $Client_Update_Rate science_looper $parent_win

}

