###################################
#
# topbox_close.tcl
#
###################################
#
# Close the connection to the Topbox
#
# Written  14sep07  Started with pcr_connect.tcl
#

proc topbox_close { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl

    global reply_data

    if { $Topbox_Connected } {
	set status [catch { Topbox_Cmd Close } msg ]
	if { $status } {
	    tk_messageBox -message \
		"Error closing Topcox_Server Connection\n$msg" \
		-parent $parent_win \
		-icon error -type ok
	    return -1
	} else {
	    set Topbox_Connected 0
	    set Topbox_Error 0
	}
    }

    return 0
}
