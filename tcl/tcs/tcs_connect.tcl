###################################
#
# tcs_connect.tcl
#
###################################
#
# Connect to the Tcs stage
#
# Written  25mar04  Started with oap_init.tcl
#

proc tcs_connect { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/tcs/tcs_globals.tcl

    global reply_data

    set status [catch { TCS_Cmd Connect } msg ]
    if { $status } {
	tk_messageBox -message \
	    "Error connecting to TCS server\n$reply_data" \
	    -parent $parent_win \
	    -icon error -type ok
	return $status
    }


    return 0
}
