###################################
#
# fsm_move_stella.tcl
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

proc fsm_move_stella { tip tilt parent_win } {

    global reply_data

# Move FSM for motion on Stella

    topbox_move_rel "FSM_Tip" $tip $parent_win
    topbox_move_rel "FSM_Tilt" [expr -1.0 * $tilt] $parent_win

}
