###################################
#
# process_looper.tcl
#
###################################
#
# Self calling prodecure to update check that all AO Servers are running
#
# Written   26jul07  DLM
#

proc process_looper { parent_win } {
      
    global PCR_HOME
    source $PCR_HOME/tcl/process/process_globals.tcl
#
    upvar #0 Info Info
#
# Set the appropriate flags for the requested process
#
    if { $PCR_Server_Start } {
	if { !$PCR_Server_Running } {
	    set status [ catch { exec query_process pcr_server $PCR_Server_Address} msg ]
	    if { $msg == "0" } {
		set PCR_Server_Running 0
	    } elseif { $msg == "1" } {
		set PCR_Server_Running 1
	    } else {
		puts "  process_looper: $msg"
		set PCR_Server_Running 0
	    }
	}
    }
#
# Check to see if the PCR (pcr and dds) processes are running
#
    if { $PCR_Start } {
	if { !$PCR_Running || !$Info(PCR_CMD_Connected) || !$Info(PCR_WFSC_Connected) || !$Info(PCR_ERROR_Connected) } {
	    set status [ catch { exec query_process pcr $PCR_Address} msg ]
	    if { $msg == "0" } {
		set PCR_Running 0
	    } elseif { $msg == "1" } {
		set PCR_Running 1
	    } else {
		puts "  process_looper: $msg"
		set PCR_Running 0
	    }
	}

	if { !$DDS_Running  || !$Info(PCR_DDS_Connected) } {
	    set status [ catch { exec query_process dds $PCR_Address} msg ]
	    if { $msg == "0" } {
		set DDS_Running 0
	    } elseif { $msg == "1" } {
		set DDS_Running 1
	    } else {
		puts "  process_looper: $msg"
		set DDS_Running 0
	    }
	}
    }

    if { $Topbox_Server_Start } {
	if { !$Topbox_Server_Running } {
	    set status [ catch { exec query_process topbox_server $Topbox_Server_Address} msg ]
	    if { $msg == "0" } {
		set Topbox_Server_Running 0
	    } elseif { $msg == "1" } {
		set Topbox_Server_Running 1
	    } else {
		puts "  process_looper: $msg"
		set Topbox_Server_Running 0
	    }
	}
    }

    after $Process_Update_Rate process_looper $parent_win

}

