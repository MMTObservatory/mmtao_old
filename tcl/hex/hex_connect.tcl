###################################
#
# hex_connect.tcl
#
###################################
#
# Connect to the Hex stage
#
# Written  3feb07  Started with tcs_connect.tcl
#

proc hex_connect { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/hex/hex_globals.tcl

    global reply_data

    set status [catch { Hex_Cmd Connect } msg ]
    if { $status } {
	tk_messageBox -message \
	    "Error connecting to Hexapod server\n$reply_data" \
	    -parent $parent_win \
	    -icon error -type ok
	return $status
    }


    return 0
}
