###################################
#
# dclient_dm_save.tcl
#
###################################
#
# Save the requested number of DM frame in the given file
#
# Written  4sep06  Started with pcr_dm_stop.tcl
#

proc dclient_dm_save { frames dirname filename parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/dclient/dclient_globals.tcl

    global reply_data

    set request ""
    foreach el $DClient_DM_Save_List {
	if { $DClient_DM_Save($el) } {
	    if { $request == "" } {
		set request $el
	    } else {
		set request ${request}_$el
	    }
	}
    }
    if { $request == "" } {
	tk_messageBox -message \
	    "At least one type of data must be checked" \
	    -parent $parent_win \
	    -icon error -type ok
    } else {
	set status [catch { DClient_Cmd DM_Save $request $frames $dirname $filename } msg ]
	if { $status } {
	    tk_messageBox -message $reply_data \
		-parent $parent_win \
		-icon error -type ok
	}
    }

    return 0
}
