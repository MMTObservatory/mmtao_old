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
    source $PCR_HOME/tcl/main/main_globals.tcl

    upvar #0 Info Info

#    if { !$Info(Loop_Running) } {
#	tk_messageBox -message "Cannot change the Loop PGain. AO Loop is not running" \
#	    -parent $parent_win -icon error -type ok
#	return
#    }

    if { $direction == "down" && $Info(P_Gain) <= $Loop_PGain_Minimum } {
	return
    }

    if { $direction == "up" && $Info(P_Gain) >= $Loop_PGain_Maximum } {
	return
    }

    if { $direction == "up" } {
	set loop_pgain [ expr $Info(P_Gain) + $Loop_PGain_Increment ]
    } elseif { $direction == "down" } {
	set loop_pgain [ expr $Info(P_Gain) - $Loop_PGain_Increment ]
    } else {
	puts "Unknown direction in loop_pgain_up_down.tcl: $direction"
    }

    set status [catch { PCR_Cmd Loop_PGain $loop_pgain } msg ]
    if { $status }  {
	tk_messageBox -message "Error changing loop pgain" \
	    -parent $parent_win -icon error -type ok
    } else {
        set Info(P_Gain) $loop_pgain
        set Main_P_Gain  $loop_pgain
    }

    return 0
}

proc loop_igain_up_down { direction parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/loop/loop_globals.tcl
    source $PCR_HOME/tcl/main/main_globals.tcl

    upvar #0 Info Info

#    if { !$Info(Loop_Running) } {
#	tk_messageBox -message "Cannot change the Loop IGain. AO Loop is not running" \
#	    -parent $parent_win -icon error -type ok
#	return
#    }

    if { $direction == "down" && $Info(I_Gain) <= $Loop_IGain_Minimum } {
	return
    }

    if { $direction == "up" && $Info(I_Gain) >= $Loop_IGain_Maximum } {
	return
    }

    if { $direction == "up" } {
	set loop_igain [ expr $Info(I_Gain) + $Loop_IGain_Increment ]
    } elseif { $direction == "down" } {
	set loop_igain [ expr $Info(I_Gain) - $Loop_IGain_Increment ]
    } else {
	puts "Unknown direction in loop_igain_up_down.tcl: $direction"
    }

    set status [catch { PCR_Cmd Loop_IGain $loop_igain } msg ]
    if { $status }  {
	tk_messageBox -message "Error changing loop igain" \
	    -parent $parent_win -icon error -type ok
    } else {
        set Info(I_Gain) $loop_igain
        set Main_I_Gain  $loop_igain
    }

    return 0
}

proc loop_dgain_up_down { direction parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/loop/loop_globals.tcl
    source $PCR_HOME/tcl/main/main_globals.tcl

    upvar #0 Info Info

#    if { !$Info(Loop_Running) } {
#	tk_messageBox -message "Cannot change the Loop DGain. AO Loop is not running" \
#	    -parent $parent_win -icon error -type ok
#	return
#    }

    if { $direction == "down" && $Info(D_Gain) <= $Loop_DGain_Minimum } {
	return
    }

    if { $direction == "up" && $Info(D_Gain) >= $Loop_DGain_Maximum } {
	return
    }

    if { $direction == "up" } {
	set loop_dgain [ expr $Info(D_Gain) + $Loop_DGain_Increment ]
    } elseif { $direction == "down" } {
	set loop_dgain [ expr $Info(D_Gain) - $Loop_DGain_Increment ]
    } else {
	puts "Unknown direction in loop_dgain_up_down.tcl: $direction"
    }

    set status [catch { PCR_Cmd Loop_DGain $loop_dgain } msg ]
    if { $status }  {
	tk_messageBox -message "Error changing loop dgain" \
	    -parent $parent_win -icon error -type ok
    } else {
        set Info(D_Gain) $loop_dgain
        set Main_D_Gain  $loop_dgain
    }

    return 0
}

proc loop_set_pid_off { parent_win } {

    set status [catch { PCR_Cmd set_pid_off } msg ]
    if { $status }  {
	tk_messageBox -message "Error setting pid off" \
	    -parent $parent_win -icon error -type ok
    }

    loop_pgain_set 0.0 $parent_win
    loop_igain_set 0.0 $parent_win
    loop_dgain_set 0.0 $parent_win
    return 0
}

proc loop_set_pid_on { parent_win } {

    set status [catch { PCR_Cmd set_pid_on } msg ]
    if { $status }  {
	tk_messageBox -message "Error setting pid on" \
	    -parent $parent_win -icon error -type ok
    }

    return 0
}

proc loop_pgain_set { gain parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/loop/loop_globals.tcl
    source $PCR_HOME/tcl/main/main_globals.tcl

    upvar #0 Info Info

#    if { !$Info(Loop_Running) } {
#	tk_messageBox -message "Cannot change the Loop PGain. AO Loop is not running" \
#	    -parent $parent_win -icon error -type ok
#	return
#    }

    if { $gain < $Loop_PGain_Minimum } {
        return 0
    }

    if { $gain > $Loop_PGain_Maximum } {
        return 0
    }

    set loop_pgain $gain

    set status [catch { PCR_Cmd Loop_PGain $loop_pgain } msg ]
    if { $status }  {
	tk_messageBox -message "Error changing loop pgain" \
	    -parent $parent_win -icon error -type ok
    } else {
        set Info(P_Gain) $loop_pgain
        set Main_P_Gain  $loop_pgain
    }

    return 0
}

proc loop_igain_set { gain parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/loop/loop_globals.tcl
    source $PCR_HOME/tcl/main/main_globals.tcl

    upvar #0 Info Info

#    if { !$Info(Loop_Running) } {
#	tk_messageBox -message "Cannot change the Loop IGain. AO Loop is not running" \
#	    -parent $parent_win -icon error -type ok
#	return
#    }

    if { $gain < $Loop_IGain_Minimum } {
        return 0
    }

    if { $gain > $Loop_IGain_Maximum } {
        return 0
    }

    set loop_igain $gain

    set status [catch { PCR_Cmd Loop_IGain $loop_igain } msg ]
    if { $status }  {
	tk_messageBox -message "Error changing loop igain" \
	    -parent $parent_win -icon error -type ok
    } else {
        set Info(I_Gain) $loop_igain
        set Main_I_Gain  $loop_igain
    }

    return 0
}

proc loop_dgain_set { gain parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/loop/loop_globals.tcl
    source $PCR_HOME/tcl/main/main_globals.tcl

    upvar #0 Info Info

#    if { !$Info(Loop_Running) } {
#	tk_messageBox -message "Cannot change the Loop DGain. AO Loop is not running" \
#	    -parent $parent_win -icon error -type ok
#	return
#    }

    if { $gain < $Loop_DGain_Minimum } {
        return 0
    }

    if { $gain > $Loop_DGain_Maximum } {
        return 0
    }

    set loop_dgain $gain

    set status [catch { PCR_Cmd Loop_DGain $loop_dgain } msg ]
    if { $status }  {
	tk_messageBox -message "Error changing loop dgain" \
	    -parent $parent_win -icon error -type ok
    } else {
        set Info(D_Gain) $loop_dgain
        set Main_D_Gain  $loop_dgain
    }

    return 0
}
