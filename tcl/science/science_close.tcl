###################################
#
# science_close.tcl
#
###################################
#
# Close the Science Server connection.
#
# 2008-10-28 skip
#

proc science_close { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/science/science_globals.tcl

    global reply_data

    if { $Science_Connected } {
	set status [catch { Science_Cmd Close } msg ]
	if { $status } {
	    tk_messageBox -message \
		"Error closing Science Server\n$msg" \
		-parent $parent_win \
		-icon error -type ok
	    return -1
	} else {
	    set Science_Connected 0
	    set Science_Error 0
	}
    }

    return 0
}
