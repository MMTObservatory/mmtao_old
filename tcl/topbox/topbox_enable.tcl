###################################
#
# topbox_enable.tcl
#
###################################
#
# Send a relative enable a Topbox_Server device 
#
# Written  19sep07  DLM  Started with topbox_move_rel.tcl
#

proc topbox_enable { device parent_win } {

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

    if { $Status(${device}_Enabled) } {

	set status [catch { Topbox_Cmd Send "${device} Disable" } msg ]

    } else {

	set status [catch { Topbox_Cmd Send "${device} Enable" } msg ]

    }

    return $status
}
