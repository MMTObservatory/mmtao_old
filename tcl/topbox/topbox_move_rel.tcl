###################################
#
# topbox_move_rel.tcl
#
###################################
#
# Send a relative move request to the Topbox_Server for given device 
#
# Written  17sep07  DLM  Started with fsm_home.tcl
#

proc topbox_move_rel { device incr parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl

    upvar #0 Status Status

    global reply_data

    if { !$Status(${device}_Connected) } {
	set status \
	    [tk_messageBox -parent $parent_win -icon question -type ok \
		 -message "${device} is currently not conneceted" ]
	return 0
    }

    if { !$Status(${device}_Enabled) } {
	set status \
	    [tk_messageBox -parent $parent_win -icon question -type ok \
		 -message "${device} is currently not enabled" ]
	return 0
    }
    set status [catch { Topbox_Cmd Send "${device} Rel [format %.2f ${incr}]" } msg ]

    return $status
}
