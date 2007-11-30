###################################
#
# topbox_power.tcl
#
###################################
#
# Send a request to turn the power on or off to a Topbox_Server device
#
# Written  21sep07  DLM  Started with topbox_enable.tcl
#

proc topbox_power { device parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl

    upvar #0 Status Status

    global reply_data

    if { !$Status(Power_Connected) } {
	set status \
	    [tk_messageBox -parent $parent_win -icon question -type ok \
		 -message "${device} is currently not conneceted" ]
	return 0
    }

    if { $Status(Power_${device}) } {

	set status [catch { Topbox_Cmd Send "Power ${device} Off" } msg ]

    } else {

	set status [catch { Topbox_Cmd Send "Power ${device} On" } msg ]

    }

    return $status
}
