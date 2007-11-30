###################################
#
# camera_start_stop.tcl
#
###################################
#
# Start or Stop the camera
#
# Written  229jan07 DLM
#

proc camera_start_stop { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/camera/camera_globals.tcl

    upvar #0 Info Info

    global reply_data

    if { $Info(WFSC_Running) } {
	if { $Info(Loop_Running) } {
	    set status \
		[tk_messageBox -parent $parent_win -icon question -type yesno \
		     -message "Loop is running\nAre you sure want stop\nthe WFS Camera?" ]
	    if { $status == "yes" } {
		loop_open_close $parent_win
	    } else {
		return 0
	    } 
	}
	set status [catch { PCR_Cmd Camera_Stop } msg ]
    } else {
	set status [catch { PCR_Cmd Camera_Start } msg ]
    }

    return 0
}
