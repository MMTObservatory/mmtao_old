###################################
#
# pcr_slope_offset_send.tcl
#
###################################
#
# Send a static background WFSC image to the PCR
#
# Written  22nov06  Started with pcr_dm_stop.tcl
#

proc pcr_slope_offset_send { parent_win } {

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
    set status [catch { PCR_Cmd Slope_Mod_Send } msg ]

    return 0
}
