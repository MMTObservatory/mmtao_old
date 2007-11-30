###################################
#
# fsm_move_pupil.tcl
#
###################################
#
# Move the pupil the requested distance in pixel in 
#  tip and tilt direction
#
# Written  16sept03  DLM
#
# Altered  11apr06  DLM Cleaned up
#
# Altered  18sep07  DLM Added new calls to the Topbox_Server
#
##################################################

proc fsm_move_pupil { pupil_tip pupil_tilt rot_angle parent_win } {

    global reply_data

    # NOTE: set rotation so + motion is up and right
    set angle [expr ($rot_angle + 180) * 3.14159 / 180 ]

# Move FSM

    set fsm_tip [format "%.3f" [expr $pupil_tip * cos($angle) - $pupil_tilt * sin($angle) ]]
    set fsm_tilt [format "%.3f" [expr $pupil_tip * sin($angle) + $pupil_tilt * cos($angle) ]]

    topbox_move_rel "FSM_Tip" $fsm_tip $parent_win
    topbox_move_rel "FSM_Tilt" $fsm_tilt $parent_win

# Move OAP to compensate

    set oap_tip [format "%.3f" [expr $pupil_tip * cos($angle) - $pupil_tilt * sin($angle) ]]
    set oap_tilt [format "%.3f" [expr -1.0 * $pupil_tip * sin($angle) - $pupil_tilt * cos($angle) ]]

    topbox_move_rel "OAP_Tip" $oap_tip $parent_win
    topbox_move_rel "OAP_Tilt" $oap_tilt $parent_win

}
