###################################
#
# status_close.tcl
#
###################################
#
# Close the Status Server connection.
#
# Written  17jun05  DLM  Started with fsm_close.tcl
#

proc status_close { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/status/status_globals.tcl

    global reply_data

    if { $Status_Connected } {
	set status [catch { Status_Cmd Close } msg ]
	if { $status } {
	    tk_messageBox -message \
		"Error closing Status Server\n$msg" \
		-parent $parent_win \
		-icon error -type ok
	    return -1
	} else {
	    set Status_Connected 0
	    set Status_Error 0
	}
    }

    return 0
}
