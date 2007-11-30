###################################
#
# pcr_request.tcl
#
###################################
#
# Send a request to the PCR (DServ)
#
# Written  17jun06  Started with tcs_connect.tcl
#

proc pcr_request { request parent_win } {

    global PCR_HOME
    source ${PCR_HOME}/tcl/pcr/pcr_globals.tcl

    global reply_data

    set status [catch { PCR_Cmd Cmd $request } msg ]
    if { $status } {
	tk_messageBox -message \
	    "Error: $request command not complete\n$reply_data" \
	    -parent $parent_win \
	    -icon error -type ok
	return $status
    }

    return 0
}
