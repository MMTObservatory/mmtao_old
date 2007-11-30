###################################
#
# voltmeter_connect.tcl
#
###################################
#
# Connect to the HP VoltMeter via the Cyclades termial
#  server (RS232)
#
# Written  25mar04  Started with oap_init.tcl
#

proc voltmeter_connect { parent_win } {

    global AO_GUI_HOME
    source $AO_GUI_HOME/tcl/voltmeter/voltmeter_globals.tcl

    global reply_data

    set status [catch { VoltMeter_Cmd Connect } msg ]
    if { $status } {
	tk_messageBox -message \
	    "Error connecting to the HP VoltMeter\n $reply_data" \
	    -parent $parent_win \
	    -icon error -type ok
	return -1
    }

    return 0
}
