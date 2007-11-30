###################################
#
# camera_default_bias.tcl
#
###################################
#
# Set the slides to the default WFSC Bias Level
#
# Written  27sep07 DLM
#

proc camera_default_bias { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/camera/camera_globals.tcl

    for { set k 0 } { $k < 4 } { incr k } {
	set Camera_Bias($k) $Camera_Default_Bias($k)
    }

    return 0
}
