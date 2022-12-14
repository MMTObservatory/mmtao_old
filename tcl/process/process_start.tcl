################################################## 
#
# process_start.tcl
#
################################################## 
#
# Start a process on either the localhost or on a remote machine
#
# Written  26jul07  DLM
#
# Altered  13sept07 DLM  Added startup stuff for the Topbox Server
#
##################################################

proc process_start { process parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/process/process_globals.tcl

    if { $process == "pcr_server" } {
	set status [catch { exec start_data_server_log & } msg ]
	if { $PCR_Server_Running } {
	    set status \
		[tk_messageBox -parent $parent_win -icon question -type yesno \
		     -message "Data_Server is running\nAre you sure you want to kill it?" ]
	    if { $status == "yes" } {
		set status [catch { exec stop_pcr_server $PCR_Server_Address & } msg ]
		set status [catch { exec stop_data_server_log & } msg ]
	    }
	} else {
	    after 1000
	    set status [ catch { exec start_pcr_server $PCR_Server_Address & } msg ]
	}
    }

    if { $process == "pcr" } {
	if { $PCR_Running } {
	    set status \
		[tk_messageBox -parent $parent_win -icon question -type yesno \
		     -message "PCR is running\nAre you sure you want to kill it?" ]
	    if { $status == "yes" } {
		set status [catch { exec stop_pcr $PCR_Address & } msg ]
	    }
	} else {
	    set status [catch { exec start_pcr $PCR_Address & } msg ]
	}
    }

    if { $process == "dds" } {
	if { $DDS_Running } {
	    set status \
		[tk_messageBox -parent $parent_win -icon question -type yesno \
		     -message "dds is running\nAre you sure you want to kill it?" ]
	    if { $status == "yes" } {
		set status [catch { exec stop_dds $PCR_Address & } msg ]
	    }
	} else {
	    set status [catch { exec start_dds $PCR_Address & } msg ]
	}
    }

    if { $process == "topbox_server" } {
	set status [catch { exec start_topbox_server_log & } msg ]
	if { $Topbox_Server_Running } {
	    set status \
		[tk_messageBox -parent $parent_win -icon question -type yesno \
		     -message "Topbox_Server is running\nAre you sure you want to kill it?" ]
	    if { $status == "yes" } {
		set status [catch { exec stop_topbox_server $Topbox_Server_Address & } msg ]
		set status [catch { exec stop_topbox_server_log & } msg ]
	    }
	} else {
	    after 1000
	    set status [catch { exec start_topbox_server $Topbox_Server_Address & } msg ]
	}
    }

    if { $process == "science_server" } {
	set status [catch { exec start_science_server_log & } msg ]
	if { $Science_Server_Running } {
	    set status \
		[tk_messageBox -parent $parent_win -icon question -type yesno \
		     -message "Science Server is running\nAre you sure you want to kill it?" ]
	    if { $status == "yes" } {
		set status [catch { exec stop_science_server $Science_Server_Address & } msg ]
		set status [catch { exec stop_science_server_log & } msg ]
	    }
	} else {
	    after 1000
	    set status [catch { exec start_science_server $Science_Server_Address & } msg ]
	}
    }

    if { $process == "tss_server" } {
	if { $TSS_Server_Running } {
	    set status \
		[tk_messageBox -parent $parent_win -icon question -type yesno \
		     -message "TSS Server is running\nAre you sure you want to kill it?" ]
	    if { $status == "yes" } {
		set status [catch { exec stop_tss_server $TSS_Server_Address & } msg ]
	    }
	} else {
	    set status [catch { exec start_tss_server $TSS_Server_Address & } msg ]
	}
    }

    return 0
}
