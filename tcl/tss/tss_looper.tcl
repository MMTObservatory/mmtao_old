###################################
#
# tss_looper.tcl
#
###################################
#
# 2008-11-03 skip
#

proc tss_looper { parent_win } {
      
    global PCR_HOME
    source $PCR_HOME/tcl/tss/tss_globals.tcl
    source $PCR_HOME/tcl/client/client_globals.tcl
    source $PCR_HOME/tcl/process/process_globals.tcl
    source $PCR_HOME/tcl/loop/loop_globals.tcl

    upvar #0 Info Info

    if { $TSS_Connected } {
	set status [ tss_get all $parent_win ]
	if { $status } {
	    set status [ catch { exec query_process tss_server } msg ]
	    if { $msg == "0" } {
		set TSS_Server_Running 0
	    } elseif { $msg == "1" } {
		set TSS_Server_Running 1
	    } else {
		puts "  tss_looper: $msg"
		set TSS_Server_Running 0
	    }
	}
    } else {
	tss_connect $parent_win	
	if { $TSS_Connected } {
	    set TSS_Server_Running 1
	    set status [ tss_get all $parent_win ]
	} else {
	    set TSS_Server_Running 0
	    source $PCR_HOME/tcl/tss/tss_globals_set.tcl
	}
    }

#   if TSS goes into safe mode while the loop is closed, open it
    if { $TSS(tss_status) == "ok" } {
        if { $Info(Loop_Running) } {
            loop_open_close $parent_win
	    tss_bell;after 500;tss_bell;after 500;tss_bell
            tk_messageBox -message "TSS in SAFE mode\nLoop commanded open" \
                -parent $parent_win -icon error -type ok
        }
    }

    after $Client_Update_Rate tss_looper $parent_win

}

proc tss_bell {} {
    catch { exec ccdacq_eoxcmd }
}
