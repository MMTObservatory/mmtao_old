###################################
#
# tss_close.tcl
#
###################################
#
# Close the TSS Server connection.
#
# 2008-11-03 skip
#

proc tss_close { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/tss/tss_globals.tcl

    global reply_data

    if { $TSS_Connected } {
	set status [catch { TSS_Cmd Close } msg ]
	if { $status } {
	    tk_messageBox -message \
		"Error closing TSS Server\n$msg" \
		-parent $parent_win \
		-icon error -type ok
	    return -1
	} else {
	    set TSS_Connected 0
	    set TSS_Error 0
	}
    }

    return 0
}
