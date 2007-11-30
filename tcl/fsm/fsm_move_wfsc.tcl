###################################
#
# fsm_move_wfsc.tcl
#
###################################
#
# Move the fsm to move the spots on the WFSC
#
# Written  11feb06  DLM
#
# Altered  11apr06  DLM Cleaned up
#
# Altered  18sep07  DLM Added new calls to the Topbox_Server
#
##################################################

proc fsm_move_wfsc { tip tilt rot_angle parent_win } {

    global reply_data

    # NOTE: set rotation so + motion is up and right
    set angle [expr ($rot_angle + 180) * 3.14159 / 180 ]

# Move FSM

    set fsm_tip [format "%.3f" [expr $tip * cos($angle) - $tilt * sin($angle) ]]
    set fsm_tilt [format "%.3f" [expr $tip * sin($angle) + $tilt * cos($angle) ]]

    topbox_move_rel "FSM_Tip" $fsm_tip $parent_win
    topbox_move_rel "FSM_Tilt" $fsm_tilt $parent_win

}
