###################################
#
# bg_dynamic_on_off.tcl
#
###################################
#
# Turn on or off the dynamic background subtraction.
#
# Written 26sep07  Started with bg_on_off.tcl
#

proc bg_dynamic_on_off { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/bg/bg_globals.tcl

    upvar #0 Info Info

    global reply_data

    if { $Info(Background_Dynamic) } {

	set status [catch { PCR_Cmd Cmd Cam_Dynamic_Off } msg ]
	if { $status } {
	    tk_messageBox -message "Dynamic Background not turned off" \
		-parent $parent_win -icon error -type ok
	    return -1
	}

    } else {
	
	set status [catch { PCR_Cmd Cmd Cam_Dynamic_On } msg ]
	if { $status } {
	    tk_messageBox -message "Dynamic Background not turned on" \
		-parent $parent_win -icon error -type ok
	    return -1
	}

    }

    return 0
	
}
