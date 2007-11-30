###################################
#
# dclient_wfsc_save.tcl
#
###################################
#
# Save the requested number of WFSC frame in the given file
#
# Written  4sep06  Started with pcr_dm_stop.tcl
#

proc dclient_wfsc_save { frames dirname filename parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/dclient/dclient_globals.tcl

    global reply_data

    set status [catch { DClient_Cmd DM_Save WFSC $frames $dirname $filename } msg ]
    if { $status } {
	tk_messageBox -message $reply_data \
	    -parent $parent_win \
	    -icon error -type ok
    }

    return 0
}
