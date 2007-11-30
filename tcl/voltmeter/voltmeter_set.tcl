###################################
#
# voltmeter_set.tcl
#
###################################
#
# Send a configuration request to the VoltMeter
#
# Written  12apr04
#

proc voltmeter_set { request parent_win } {

    global AO_GUI_HOME
    source $AO_GUI_HOME/tcl/voltmeter/voltmeter_globals.tcl

    global reply_data

    set status [catch { VoltMeter_Cmd Set $request } msg ]
    if { $status } {
	tk_messageBox -message \
	    "Error retrieving postions VoltMeter motors\n $reply_data" \
	    -parent $parent_win \
	    -icon error -type ok
	return -1
    }

    return 0
}
