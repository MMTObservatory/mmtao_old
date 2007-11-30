###################################
#
# info_reset_seeing.tcl
#
###################################
#
# Clear the seeing arrays on the Info server on the VME
#
# Written  10feb06  DLM
#

proc info_reset_seeing { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/info/info_globals.tcl

    global reply_data

    if { $Info_Connected } {

	set status [catch { Info_Cmd Reset_Seeing } msg ]
	if { $status } {
	    tk_messageBox -message \
		"Error resetting seeing array Info server\n $reply_data" \
		-parent $parent_win \
		-icon error -type ok
	    return -1
	}
    }
    return 0
}
