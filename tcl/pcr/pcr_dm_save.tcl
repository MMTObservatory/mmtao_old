###################################
#
# pcr_dm_save.tcl
#
###################################
#
# Request the PCR DM Server to save data
#
# Written  4sep06  Started with pcr_dm_stop.tcl
#

proc pcr_dm_save { value parent_win } {

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
    if { $value } {

	set status [catch { PCR_Cmd DM_Cmd Save_DM_On } msg ]

    } else {

	set status [catch { PCR_Cmd DM_Cmd Save_DM_Off } msg ]

    }	

    return 0
}
