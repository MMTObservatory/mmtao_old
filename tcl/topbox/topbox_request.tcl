###################################
#
# topbox_request.tcl
#
###################################
#
# Send a request to the Topbox Server
#
# Written  14sep07  Started with pcr_request.tcl
#

proc topbox_request { request parent_win } {

    global PCR_HOME
    source ${PCR_HOME}/tcl/topbox/topbox_globals.tcl

    global reply_data

    set status [catch { Topbox_Cmd Cmd $request } msg ]
    if { $status } {
	tk_messageBox -message \
	    "Error: $request command not complete\n$reply_data" \
	    -parent $parent_win \
	    -icon error -type ok
	return $status
    }

    return 0
}
