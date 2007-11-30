###################################
#
# fsm_move_camera.tcl
#
###################################
#
# Move the move the fsm/oap2 for motion on the appropriate camera
#
# Written  11apr06  DLM Cleaned up
#
##################################################

proc fsm_move_camera { direction parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/info/info_globals.tcl
    source $PCR_HOME/tcl/fsm/fsm_globals.tcl
#
    global reply_data
#
    upvar #0 Motor Motor
    upvar #0 Info Info
#
    if { $direction == "up" } {
	if { $FSM_Camera == "WFSC" } {
	    fsm_move_wfsc $FSM_Incr 0 $Motor(Rotation_Position) $parent_win 
	} elseif { $FSM_Camera == "Stella" } {
	    fsm_move_stella $FSM_Incr 0 $parent_win 
	} elseif { $FSM_Camera == "Pupil" } {
	    fsm_move_pupil $FSM_Incr 0 $Motor(Rotation_Position) $parent_win
	}
    } elseif { $direction == "down" } {
	if { $FSM_Camera == "WFSC" } {
	    fsm_move_wfsc [expr -1 * $FSM_Incr] 0 $Motor(Rotation_Position) $parent_win 
	} elseif { $FSM_Camera == "Stella" } {
	    fsm_move_stella [expr -1 * $FSM_Incr] 0 $parent_win 
	} elseif { $FSM_Camera == "Pupil" } {
	    fsm_move_pupil [expr -1 * $FSM_Incr] 0 $Motor(Rotation_Position) $parent_win
	}
    } elseif { $direction == "right" } {
	if { $FSM_Camera == "WFSC" } {
	    fsm_move_wfsc 0 $FSM_Incr $Motor(Rotation_Position) $parent_win
	} elseif { $FSM_Camera == "Stella" } {
	    fsm_move_stella 0 $FSM_Incr $parent_win
	} elseif { $FSM_Camera == "Pupil" } {
	    fsm_move_pupil 0 $FSM_Incr $Motor(Rotation_Position) $parent_win
	}
    } elseif { $direction == "left" } {
	if { $FSM_Camera == "WFSC" } {
	    fsm_move_wfsc 0 [expr -1 * $FSM_Incr] $Motor(Rotation_Position) $parent_win
	} elseif { $FSM_Camera == "Stella" } {
	    fsm_move_stella 0 [expr -1 * $FSM_Incr] $parent_win
	} elseif { $FSM_Camera == "Pupil" } {
	    fsm_move_pupil 0 [expr -1 * $FSM_Incr] $Motor(Rotation_Position) $parent_win 
	}
    } else {
	tk_messageBox -message "Unknown direction in fsm_move_camera: $direction" \
	    -parent $parent_win -icon error -type ok
	return -1
    }
    return 0
}
