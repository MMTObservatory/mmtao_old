################################################## 
#
# loop_gain_up_down.tcl
#
################################################## 
#
# Change the loop gain up/down by a preset increment
#
# Written  11apr06  DLM
#
##################################################

proc loop_gain_up_down { direction parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/loop/loop_globals.tcl

    upvar #0 Info Info

    if { !$Info(Loop_Running) } {
	tk_messageBox -message "Cannot change the Loop Gain. AO Loop is not running" \
	    -parent $parent_win -icon error -type ok
	return
    }

    if { $direction == "down" && $Info(Loop_Gain) <= $Loop_Gain_Minimum } {
	return
    }

    if { $direction == "up" && $Info(Loop_Gain) >= $Loop_Gain_Maximum } {
	return
    }

    if { $direction == "up" } {
	set loop_gain [ expr $Info(Loop_Gain) + $Loop_Gain_Increment ]
    } elseif { $direction == "down" } {
	set loop_gain [ expr $Info(Loop_Gain) - $Loop_Gain_Increment ]
    } else {
	puts "Unknown direction in loop_gain_up_down.tcl: $direction"
    }

    set status [catch { PCR_Cmd Loop_Gain $loop_gain } msg ]
    if { $status }  {
	tk_messageBox -message "Error changing loop gain" \
	    -parent $parent_win -icon error -type ok
    }

    return 0
}
