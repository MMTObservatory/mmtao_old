###################################
#
# voltmeter_query.tcl
#
###################################
#
# Initialize the VoltMeter communication.
#
# Written  25mar04  Started with oap_init.tcl
#

proc voltmeter_query { parent_win } {

    global AO_GUI_HOME
    source $AO_GUI_HOME/tcl/voltmeter/voltmeter_globals.tcl

    global reply_data

    set status [catch { VoltMeter_Cmd Query } msg ]
    if { $status } {
#    set VoltMeter_Connected $VoltMeter_Connected
	tk_messageBox -message \
	    "Error retrieving postions VoltMeter motors\n $reply_data" \
	    -parent $parent_win \
	    -icon error -type ok
	return -1
    }
#    set VoltMeter_Connected $VoltMeter_Connected

    return 0
}
