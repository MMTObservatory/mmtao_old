###################################
#
# pcr_dm_stop.tcl
#
###################################
#
# Request DServ to stop the PCR DM Client
#
# Written  14aug06  Started with pcr_dm_connect.tcl
#

proc pcr_dm_stop { parent_win } {

    global AO_GUI_HOME
    source $AO_GUI_HOME/../pcr/tcl/pcr/pcr_globals.tcl

    global reply_data

#    if { $TCS_Connected } {
#	set status \
#	    [tk_messageBox -parent $parent_win -icon question -type yesno \
#		 -message "Tcs stage is currently connected\n Are you sure want to reconnect?" ]
#	if { $status == "yes" } {
#	    tcs_close $parent_win
#	} else {
#	    return 0
#	}
#    }
    set status [catch { PCR_Cmd DM_Stop } msg ]

    return 0
}
