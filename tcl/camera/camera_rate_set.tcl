###################################
#
# camera_rate_set.tcl
#
###################################
#
# Set the camera rate by sending a command to the PCR command port
#
# Written  28aug06 DLM
#

proc camera_rate_set { rate parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/camera/camera_globals.tcl

    global reply_data

    set status [catch { PCR_Cmd Cam_Rate_Set $rate } msg ]

    return 0
}
