###################################
#
# info_close.tcl
#
###################################
#
# Close the Info Server connection.
#
# Written  17jun05  DLM  Started with fsm_close.tcl
#

proc info_close { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/info/info_globals.tcl

    global reply_data

    if { $Info_Connected } {
	set status [catch { Info_Cmd Close } msg ]
	if { $status } {
	    tk_messageBox -message \
		"Error closing Info Server\n$msg" \
		-parent $parent_win \
		-icon error -type ok
	    return -1
	} else {
	    set Info_Connected 0
	    set Info_Error 0
	}
    }

    return 0
}
