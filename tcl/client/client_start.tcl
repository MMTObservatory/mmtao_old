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
proc client_start { tcs_start pcr_server_start pcr_start topbox_start parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/client/client_globals.tcl
    source $PCR_HOME/tcl/tcs/tcs_globals.tcl
    source $PCR_HOME/tcl/pcr/pcr_globals.tcl
    source $PCR_HOME/tcl/process/process_globals.tcl
    source $PCR_HOME/tcl/info/info_globals.tcl
    source $PCR_HOME/tcl/status/status_globals.tcl
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl
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

}
