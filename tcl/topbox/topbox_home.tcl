###################################
#
# topbox_home.tcl
#
###################################
#
# Send a home request to the Topbox_Server for given device 
#
# Written  17sep07  DLM  Started with fsm_home.tcl
#

proc topbox_home { device parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl

    upvar #0 Status Status

    global reply_data

    if { $Status(${device}_Home) } {
	set status \
	    [tk_messageBox -parent $parent_win -icon question -type yesno \
		 -message "${device} is currently homed\n Are you sure want to re-home?" ]
	if { $status == "no" } {
	    return 0
	}
    }

    set status [catch { Topbox_Cmd Send "${device} Home" } msg ]

    return $status
}
