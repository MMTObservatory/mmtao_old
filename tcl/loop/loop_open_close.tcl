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
	set status [catch { PCR_Cmd Loop_PGain 0.0 } msg ]
	if { $status }  {
	    tk_messageBox -message "Error changing loop pgain" \
		-parent $parent_win -icon error -type ok
	} else {
	    set Info(Loop_PGain) 0.0
	}
	set status [catch { PCR_Cmd Loop_IGain 0.0 } msg ]
	if { $status }  {
	    tk_messageBox -message "Error changing loop igain" \
		-parent $parent_win -icon error -type ok
	} else {
	    set Info(Loop_IGain) 0.0
	}
	set status [catch { PCR_Cmd Loop_DGain 0.0 } msg ]
	if { $status }  {
	    tk_messageBox -message "Error changing loop dgain" \
		-parent $parent_win -icon error -type ok
	} else {
	    set Info(Loop_DGain) 0.0
	}

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
