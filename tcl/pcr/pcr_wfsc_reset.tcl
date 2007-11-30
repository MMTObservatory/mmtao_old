###################################
#
# pcr_wfsc_reset.tcl
#
###################################
#
# Request DServ to reset the PCR WFSC Client
#
# Written  24aug06  Started with pcr_wfsc_stop.tcl
#

proc pcr_wfsc_reset { parent_win } {

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
    set status [catch { PCR_Cmd WFSC_Reset } msg ]

    return 0
}
