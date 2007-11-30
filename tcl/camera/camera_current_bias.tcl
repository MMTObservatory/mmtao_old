###################################
#
# camera_current_bias.tcl
#
###################################
#
# Set the slides to the current WFSC Bias Level
#
# Written  29jan07 DLM
#

proc camera_current_bias { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/camera/camera_globals.tcl

    upvar #0 Info Info

    for { set k 0 } { $k < 4 } { incr k } {
	set Camera_Bias($k) $Info(WFSC_Bias_${k})
    }

    return 0
}
