###################################
#
# rotator_tracking.tcl
#
###################################
#
# Keep the WFSC tracking the Rotator angle
#
# Written  8may04  DLM
#
# Altered 18dec07  DLM  Converted to PCR version of the GUI
#

proc rotator_tracking { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/rotator/rotator_globals.tcl

    global reply_data

    upvar #0 TCS TCS
    upvar #0 Status Status

    if { [ expr abs( $TCS(rot) - $Status(Rotation_Position) ) ] > 0.1 } {
	topbox_move_abs "Rotation" $TCS(rot)  $parent_win
    }

    return 0
}
