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

proc loop_pgain_up_down { direction parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/loop/loop_globals.tcl

    upvar #0 Info Info

    if { !$Info(Loop_Running) } {
	tk_messageBox -message "Cannot change the Loop PGain. AO Loop is not running" \
	    -parent $parent_win -icon error -type ok
	return
    }

    if { $direction == "down" && $Info(Loop_PGain) <= $Loop_PGain_Minimum } {
	return
    }

    if { $direction == "up" && $Info(Loop_PGain) >= $Loop_PGain_Maximum } {
	return
    }

    if { $direction == "up" } {
	set loop_pgain [ expr $Info(Loop_PGain) + $Loop_PGain_Increment ]
    } elseif { $direction == "down" } {
	set loop_pgain [ expr $Info(Loop_PGain) - $Loop_PGain_Increment ]
    } else {
	puts "Unknown direction in loop_pgain_up_down.tcl: $direction"
    }

    set status [catch { PCR_Cmd Loop_PGain $loop_pgain } msg ]
    if { $status }  {
	tk_messageBox -message "Error changing loop pgain" \
	    -parent $parent_win -icon error -type ok
    } else {
        set Info(Loop_PGain) $loop_pgain
    }

    return 0
}

proc loop_igain_up_down { direction parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/loop/loop_globals.tcl

    upvar #0 Info Info

    if { !$Info(Loop_Running) } {
	tk_messageBox -message "Cannot change the Loop IGain. AO Loop is not running" \
	    -parent $parent_win -icon error -type ok
	return
    }

    if { $direction == "down" && $Info(Loop_IGain) <= $Loop_IGain_Minimum } {
	return
    }

    if { $direction == "up" && $Info(Loop_IGain) >= $Loop_IGain_Maximum } {
	return
    }

    if { $direction == "up" } {
	set loop_igain [ expr $Info(Loop_IGain) + $Loop_IGain_Increment ]
    } elseif { $direction == "down" } {
	set loop_igain [ expr $Info(Loop_IGain) - $Loop_IGain_Increment ]
    } else {
	puts "Unknown direction in loop_igain_up_down.tcl: $direction"
    }

    set status [catch { PCR_Cmd Loop_IGain $loop_igain } msg ]
    if { $status }  {
	tk_messageBox -message "Error changing loop igain" \
	    -parent $parent_win -icon error -type ok
    } else {
        set Info(Loop_IGain) $loop_igain
    }

    return 0
}

proc loop_dgain_up_down { direction parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/loop/loop_globals.tcl

    upvar #0 Info Info

    if { !$Info(Loop_Running) } {
	tk_messageBox -message "Cannot change the Loop DGain. AO Loop is not running" \
	    -parent $parent_win -icon error -type ok
	return
    }

    if { $direction == "down" && $Info(Loop_DGain) <= $Loop_DGain_Minimum } {
	return
    }

    if { $direction == "up" && $Info(Loop_DGain) >= $Loop_DGain_Maximum } {
	return
    }

    if { $direction == "up" } {
	set loop_dgain [ expr $Info(Loop_DGain) + $Loop_DGain_Increment ]
    } elseif { $direction == "down" } {
	set loop_dgain [ expr $Info(Loop_DGain) - $Loop_DGain_Increment ]
    } else {
	puts "Unknown direction in loop_dgain_up_down.tcl: $direction"
    }

    set status [catch { PCR_Cmd Loop_DGain $loop_dgain } msg ]
    if { $status }  {
	tk_messageBox -message "Error changing loop dgain" \
	    -parent $parent_win -icon error -type ok
    } else {
        set Info(Loop_DGain) $loop_dgain
    }

    return 0
}
