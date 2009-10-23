################################################## 
#
# loop_open_close.tcl
#
################################################## 
#
# Routine to open or close the AO loop
#
# Written  8apr06  DLM  Started with old loop_open_close.tcl
#                       Removed all SInfo calls and added Info
#                       Now pass logical open_loop.  If open_loop is non-zero
#                         then the loop will be opened.  This is so the current
#                         value of Info(Loop_Running) can be passed to have this
#                         routine do the opposite.
#
# Altered 30jan07  DLM  Change to work with PCR
#
##################################################

proc loop_open_close { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/loop/loop_globals.tcl

    upvar #0 Info Info

    if { $Info(Loop_Running) } {

	set status [catch { PCR_Cmd Loop_Open } msg ]
	if { $status }  {
	    tk_messageBox -message "Error Stopping the loop" \
		-parent $parent_win -icon error -type ok
	}

	# per vidhya's email to skip 2009-9-30
	# we set the the p,i,d, gains to 0
	# when we push open loop
	loop_pgain_set 0.0 $parent_win
	loop_igain_set 0.0 $parent_win
	loop_dgain_set 0.0 $parent_win

    } else {

	if { !$Info(WFSC_Running) } {
	    tk_messageBox -message "WFSC is not running\nCannot Close the loop" \
		-parent $parent_win -icon error -type ok
	    return -1
	}

	set status [catch { PCR_Cmd Loop_Close } msg ]
	if { $status }  {
	    tk_messageBox -message "Error Starting the loop" \
		-parent $parent_win -icon error -type ok
	}

    }

    return 0
}
