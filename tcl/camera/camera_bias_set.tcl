###################################
#
# camera_bias_set.tcl
#
###################################
#
# Send a bias value for one quadrant to the PCR in order
#   to set the bias level on the WFSC via the SciMeasure Controller
#
# Written  24aug06 DLM
#

proc camera_bias_set { quad value parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/camera/camera_globals.tcl

    global reply_data

    set status [catch { PCR_Cmd Cam_Bias_Set $quad $value } msg ]

    after 5000

    return 0
}

