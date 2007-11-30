###################################
#
# voltmeter_get.tcl
#
###################################
#
# Request data from the HP VoltMeter
#
# Written  12apr04
#

proc voltmeter_get { request parent_win } {

    global AO_GUI_HOME
    source $AO_GUI_HOME/tcl/voltmeter/voltmeter_globals.tcl

    global reply_data

    set status [catch { VoltMeter_Cmd Get $request } msg ]
    if { $status } {
	tk_messageBox -message \
	    "Error retrieving information from the HP VoltMeter\n $reply_data" \
	    -parent $parent_win \
	    -icon error -type ok
	return -1
    }

    return $reply_data
}
