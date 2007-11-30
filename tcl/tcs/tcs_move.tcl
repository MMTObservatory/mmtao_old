###################################
#
# tcs_move.tcl
#
###################################
#
# Move the telescope some alt/az or ra/dec
#
# Written  12feb04 DLM
#
proc tcs_move { tip_delta tilt_delta angle parent_win } {

#
# Move the telescope
#   **These request values are in ArcSec
#   **Tip/Tilt as viewed on the Apogee camera and ARIES (MIRAC?)
#
# Perform the rotation
#

    puts "$tip_delta $tilt_delta $angle"
    set radians [expr $angle * 3.14159 / 180. ]
    set alt [expr $tip_delta * cos($radians) - $tilt_delta * sin($radians) ]
    set az [expr $tip_delta * sin($radians) + $tilt_delta * cos($radians)]
#
    puts "TCS alt  = [format %.3f $alt]"
    puts "TCS az   = [format %.3f $az]"
#
# Send request to TCS
#

    tcs_set setaaao $alt $az $parent_win

    return 0
}
