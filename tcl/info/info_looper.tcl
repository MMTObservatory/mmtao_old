###################################
#
# info_looper.tcl
#
###################################
#
# Self calling prodecure to update WFC information on the GUI
#
# Written   26may04    DLM
#
# Altered    2feb06  DLM  Change so the loop is infinite, but the call to retieve
#                           xinformation only occurs if we are connected to the
#                           Info server.
#

proc info_looper { parent_win } {
      
    global PCR_HOME
    source $PCR_HOME/tcl/info/info_globals.tcl
    source $PCR_HOME/tcl/client/client_globals.tcl
    source $PCR_HOME/tcl/process/process_globals.tcl

    if { $Info_Connected } {
	set status [ info_get all $parent_win ]
	if { $status } {
	    set status [ catch { exec query_process pcr_server } msg ]
	    if { $msg == "0" } {
		set PCR_Server_Running 0
	    } elseif { $msg == "1" } {
		set PCR_Server_Running 1
	    } else {
		puts "  info_looper: $msg"
		set PCR_Server_Running 0
	    }
	}
    } else {
	info_connect $parent_win	
	if { $Info_Connected } {
	    set PCR_Server_Running 1
	    set status [ info_get all $parent_win ]
	    pcr_server_connect PCR $parent_win	
	} else {
	    set PCR_Server_Running 0
	    source $PCR_HOME/tcl/info/info_globals_set.tcl
	}
    }

    after $Client_Update_Rate info_looper $parent_win

}

