###################################
#
# fsm_move_ra_dec.tcl
#
###################################
#
# Convert from RA/DEC to Alt/Az and send request to FSM
#
# Written  27jan04 DLM
#
# Altered  18sep07  DLM Added new calls to the Topbox_Server
#
##################################################

proc fsm_move_ra_dec { ra_delta dec_delta parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/tcs/tcs_globals.tcl
    source $PCR_HOME/tcl/fsm/fsm_globals.tcl

    upvar #0 TCS TCS
#
# Convert from RA/DEC to Alt/Az
#
#   Here is it assumed that a delta RA of 1" is really
#     1" on the sky rather than 1" in RA because distance
#     in RA is dependent on the DEC.
#
#   Thus, here we just need to rotate the RA/DEC system
#     into the Alt/Az system and project the same magnitude
#     of motion in the RA/DEC system to the Alt/Az system
#
# Parallactic Angle from TCS
#
    set pa [expr $TCS(pa) - $TCS(rot)]
#
    set radians [expr $pa * 3.14159 / 180. ]
#
    set tip_delta [expr $ra_delta * sin($radians) + $dec_delta * cos($radians) ]
    set tilt_delta [expr $ra_delta * cos($radians) - $dec_delta * sin($radians) ]
#
    puts "$ra_delta $dec_delta $tip_delta $tilt_delta"
    topbox_move_rel "FSM_Tip" $tip_delta $parent_win
    topbox_move_rel "FSM_Tilt" $tilt_delta $parent_win

    return 0
}
