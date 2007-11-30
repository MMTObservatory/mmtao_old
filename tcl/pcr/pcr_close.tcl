###################################
#
# pcr_close.tcl
#
###################################
#
# Close the connection to the PCR (DServ)
#
# Written  17jun06  Started with tcs_connect.tcl
#

proc pcr_close { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/pcr/pcr_globals.tcl

    global reply_data

    if { $PCR_Connected } {
	set status [catch { PCR_Cmd Close } msg ]
	if { $status } {
	    tk_messageBox -message \
		"Error closing PCR_Server Connection\n$msg" \
		-parent $parent_win \
		-icon error -type ok
	    return -1
	} else {
	    set PCR_Connected 0
	    set PCR_Error 0
	}
    }

    return 0
}
