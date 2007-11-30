###################################
#
# status_looper.tcl
#
###################################
#
# Self calling prodecure to update WFC statusrmation on the GUI
#
# Written   26may04    DLM
#
# Altered    2feb06  DLM  Change so the loop is infinite, but the call to retieve
#                           information only occurs if we are connected to the
#                           Status server.
#

proc status_looper { parent_win } {
      
    global PCR_HOME
    source $PCR_HOME/tcl/status/status_globals.tcl
    source $PCR_HOME/tcl/client/client_globals.tcl
    source $PCR_HOME/tcl/process/process_globals.tcl

    if { $Status_Connected } {
	set status [ status_get all $parent_win ]
	if { $status } {
	    set status [ catch { exec query_process pcr_server } msg ]
	    if { $msg == "0" } {
		set Topbox_Server_Running 0
	    } elseif { $msg == "1" } {
		set Topbox_Server_Running 1
	    } else {
		puts "  status_looper: $msg"
		set Topbox_Server_Running 0
	    }
	}
    } else {
	status_connect $parent_win	
	if { $Status_Connected } {
	    set Topbox_Server_Running 1
	    set status [ status_get all $parent_win ]
	    topbox_connect $parent_win	
	} else {
	    set Topbox_Server_Running 0
	    source $PCR_HOME/tcl/status/status_globals_set.tcl
	}
    }

    after $Client_Update_Rate status_looper $parent_win

}

