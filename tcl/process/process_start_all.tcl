###################################
#
# process_start_all.tcl
#
###################################
#
# Procedure to check if the needed processes are running.  If not, then
#   ask if they should be started.
#
# If one or more process is not running then the process_looper is started
#   to watch then all processes are running.  When all processes have been
#   started process_looper is exit because it makes the gui slow (running of
#   exec).  The process_looper is restarted by info_looper if it detects that
#   one of the processes is not running (can not connect).
#
# Written 30jul07  DLM  Started with client_start.tcl
#
# Altered 16sep07  DLM  Removed passes arguements and now use global varialbes
#                         set in client_start.tcl
# Altered 2008-10-28 skip Added science
# Altered 2008-11-03 skip Added tss
#
proc process_start_all { pcr_server_start pcr_start topbox_start science_start tss_start parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/process/process_globals.tcl
    source $PCR_HOME/tcl/info/info_globals.tcl
    source $PCR_HOME/tcl/status/status_globals.tcl
    source $PCR_HOME/tcl/science/science_globals.tcl
    source $PCR_HOME/tcl/tss/tss_globals.tcl
#
# Start the PCR_Server (commands and information)
#
    if { $Info_Connected } {
	set PCR_Server_Running 1
    } else {
	if { $pcr_server_start } {
	    set status [ catch { exec query_process pcr_server $PCR_Server_Address} msg ]
	    if { $msg == "0" } {
		set status \
		    [tk_messageBox -parent $parent_win -icon question -type yesno \
			 -message "Data_Server is not running\nDo you want to start it?" ]
		if { $status == "yes" } {
                    set status [catch { exec start_data_server_log } msg ]
		    set status [catch { exec start_pcr_server $PCR_Server_Address & } msg ]
		}
	    } elseif { $msg == "1" } {
	        set status [catch { exec start_data_server_log } msg ]
		set PCR_Server_Running 1
	    } else {
		puts "  process_start_all: $msg"
		set PCR_Server_Running 0
	    }
	}
    }

#
# Start the Topbox_Server (commands and status)
#
    if { $Status_Connected } {
	set Topbox_Server_Running 1
    } else {
	if { $topbox_start } {
	    set status [ catch { exec query_process topbox_server $Topbox_Server_Address} msg ]
	    if { $msg == "0" } {
		set status \
		    [tk_messageBox -parent $parent_win -icon question -type yesno \
			 -message "Topbox_Server is not running\nDo you want to start it?" ]
		if { $status == "yes" } {
                    set status [catch { exec start_topbox_server_log } msg ]
		    set status [catch { exec start_topbox_server $Topbox_Server_Address & } msg ]
		}
	    } elseif { $msg == "1" } {
	        set status [catch { exec start_topbox_server_log } msg ]
		set Topbox_Server_Running 1
	    } else {
		puts "  process_start_all: $msg"
		set Topbox_Server_Running 0
	    }
	}
    }

#
# Start the Science server
#
    if { $Science_Connected } {
	set Science_Server_Running 1
    } else {
	if { $science_start } {
	    set status [ catch { exec query_process science_server $Science_Server_Address} msg ]
	    if { $msg == "0" } {
		set status \
		    [tk_messageBox -parent $parent_win -icon question -type yesno \
			 -message "Science_Server is not running\nDo you want to start it?" ]
		if { $status == "yes" } {
                    set status [catch { exec start_science_server_log } msg ]
		    set status [catch { exec start_science_server $Science_Server_Address & } msg ]
		}
	    } elseif { $msg == "1" } {
	        set status [catch { exec start_science_server_log } msg ]
		set Science_Server_Running 1
	    } else {
		puts "  process_start_all: $msg"
		set Science_Server_Running 0
	    }
	}
    }

#
# Start the TSS server
#
    if { $TSS_Connected } {
	set TSS_Server_Running 1
    } else {
	if { $tss_start } {
	    set status [ catch { exec query_process tss_server $TSS_Server_Address} msg ]
	    if { $msg == "0" } {
		set status \
		    [tk_messageBox -parent $parent_win -icon question -type yesno \
			 -message "TSS_Server is not running\nDo you want to start it?" ]
		if { $status == "yes" } {
		    set status [catch { exec start_tss_server $TSS_Server_Address & } msg ]
		}
	    } elseif { $msg == "1" } {
		set TSS_Server_Running 1
	    } else {
		puts "  process_start_all: $msg"
		set TSS_Server_Running 0
	    }
	}
    }

#
# Start the PCR
#
    if { $Info(PCR_CMD_Connected) } {
	set PCR_Running 1
    } else {
	if { $pcr_start } {
	    set status [ catch { exec query_process pcr $PCR_Address} msg ]
	    if { $msg == "0" } {
		set status \
		    [tk_messageBox -parent $parent_win -icon question -type yesno \
			 -message "PCR is not running\nDo you want to start it?" ]
		if { $status == "yes" } {
		    set status [catch { exec start_pcr $PCR_Address & } msg ]
		}
	    } elseif { $msg == "1" } {
		set PCR_Running 1
	    } else {
		puts "  process_start_all: $msg"
		set PCR_Running 0
	    }
	}
    }

#
# Start the DDS
#
    if { $Info(PCR_DDS_Connected) } {
	set DDS_Running 1
    } else {
	if { $pcr_start } {
	    set status [ catch { exec query_process dds $PCR_Address} msg ]
	    if { $msg == "0" } {
		set status \
		    [tk_messageBox -parent $parent_win -icon question -type yesno \
			 -message "DDS is not running\nDo you want to start it?" ]
		if { $status == "yes" } {
		    set status [catch { exec start_dds $PCR_Address & } msg ]
		}
	    } elseif { $msg == "1" } {
		set DDS_Running 1
	    } else {
		puts "  process_start_all: $msg"
		set DDS_Running 0
	    }
	}
    }

#    process_looper $parent_win

}
