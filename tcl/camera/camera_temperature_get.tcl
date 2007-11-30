###################################
#
# camera_temperature_get.tcl
#
###################################
#
# Get the camera temperature
#
# Written  27nov06 DLM
#

proc camera_temperature_get { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/camera/camera_globals.tcl

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
    set status [catch { PCR_Cmd cmd cam_temp_get } msg ]

    return 0
}
