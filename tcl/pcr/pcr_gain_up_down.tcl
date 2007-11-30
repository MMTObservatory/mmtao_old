################################################## 
#
# pcr_gain_up_down.tcl
#
################################################## 
#
# Change the loop gain up/down by a preset increment
#
# Written  23aug06  DLM  Started with loop_gain_up_down.tcl for
#                          the VME
#
##################################################

proc pcr_gain_up_down { direction parent_win } {

    global AO_GUI_HOME
    source $AO_GUI_HOME/tcl/loop/loop_globals.tcl
    source $AO_GUI_HOME/../pcr/tcl/pcr/pcr_globals.tcl

    upvar #0 Info Info

    if { !$Info(Loop_Running) } {
	tk_messageBox -message "Cannot change the Loop Gain. AO Loop is not running" \
	    -parent $parent_win -icon error -type ok
	return
    }

    if { $direction == "down" && $Info(User_Gain) <= $Loop_Gain_Minimum } {
	return
    }

    if { $direction == "up" && $Info(User_Gain) >= $Loop_Gain_Maximum } {
	return
    }

    if { $direction == "up" } {
	set loop_gain [ expr $Info(User_Gain) + $Loop_Gain_Increment ]
    } elseif { $direction == "down" } {
	set loop_gain [ expr $Info(User_Gain) - $Loop_Gain_Increment ]
    } else {
	puts "Unknown direction in pcr_gain_up_down.tcl: $direction"
    }

    set status [catch {PCR_Cmd User_Gain $loop_gain } msg]
    if { $status }  {

	tk_messageBox -message "Error setting AO Loop Gain" \
	    -parent $parent_win -icon error -type ok

    } else {

	set Info(User_Gain) $loop_gain
#	set status [catch { Info_Cmd Set User_Gain $loop_gain } msg ]
#	set status [catch { Info_Cmd Set Loop_Gain $loop_gain } msg ]

    }

    return 0
}
