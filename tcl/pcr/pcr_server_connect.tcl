###################################
#
# pcr_server_connect.tcl
#
###################################
#
# Connect to one of the PCR Server
#
# Written  28jan07  Started with pcr_connect.tcl
#

proc pcr_server_connect { server parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/pcr/pcr_globals.tcl

    global reply_data

    set status [catch { PCR_Cmd ${server}_Connect } msg ]
    if { $status } {
	tk_messageBox -message \
	    "Error connecting to ${server} server\n$reply_data" \
	    -parent $parent_win \
	    -icon error -type ok
	return $status
    }

    return 0
}
