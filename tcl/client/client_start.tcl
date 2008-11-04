###################################
#
# client_start.tcl
#
###################################
#
# Procedure to open all connections when GUI is started
#
# Written 24may04  DLM
#
# Altered  2feb06  DLM  Renamed from main_connect.tcl
#
# Altered 11sep06  DLM  Added arguments (flags) to request only specifice clients
#                         to be started.
#
# Altered 10jul07  DLM  Removed connection to Hexapod.  All hexapod connections are
#                         single request communications.  Thus, a connection to the hexapod
#                         is established, a request sent and then the connection closed.
#                       Rolled the connection to the PCR_Server for commands and for information
#                         into one flag (pcr_server_start)
#
# Altered 12sep07  DLM  Added the new connection to the topbox and status server
#
# Altered 2008-10-28 skip Added science
# Altered 2008-11-03 skip Added tss
#
proc client_start { tcs_start pcr_server_start pcr_start topbox_start science_start tss_start parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/client/client_globals.tcl
    source $PCR_HOME/tcl/tcs/tcs_globals.tcl
    source $PCR_HOME/tcl/pcr/pcr_globals.tcl
    source $PCR_HOME/tcl/process/process_globals.tcl
    source $PCR_HOME/tcl/info/info_globals.tcl
    source $PCR_HOME/tcl/status/status_globals.tcl
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl
    source $PCR_HOME/tcl/science/science_globals.tcl
    source $PCR_HOME/tcl/tss/tss_globals.tcl
#
# Open connection to the TCS
#
    if { $tcs_start } {
	tcs_connect $parent_win
	if { $TCS_Connected } {
	    tcs_get all $parent_win
	}
    }
    after $Client_Update_Rate tcs_looper $parent_win
#
# Connect to the PCR_Server (commands and information)
#
    if { $pcr_server_start } {
	info_connect $parent_win
	if { $Info_Connected } {
	    set PCR_Server_Running 1
	    set status [ info_get all $parent_win ]
	    pcr_server_connect PCR $parent_win
	}
    }
    after $Client_Update_Rate info_looper $parent_win
#
# Connect to the Topbox_Server (commands and status)
#
    if { $topbox_start } {
	status_connect $parent_win
	if { $Status_Connected } {
	    set Topbox_Server_Running 1
	    set status [ status_get all $parent_win ]
	    topbox_connect $parent_win
	}
    }
    after $Client_Update_Rate status_looper $parent_win
#
# Connect to the Science_Server (commands and status)
#
    if { $science_start } {
	science_connect $parent_win
	if { $Science_Connected } {
	    set Science_Server_Running 1
	    set status [ science_get all $parent_win ]
	}
    }
    after $Client_Update_Rate science_looper $parent_win
#
# Connect to the TSS_Server (commands and status)
#
    if { $tss_start } {
	tss_connect $parent_win
	if { $TSS_Connected } {
	    set TSS_Server_Running 1
	    set status [ tss_get all $parent_win ]
	}
    }
    after $Client_Update_Rate tss_looper $parent_win

}
