###################################
#
# pcr_connect.tcl
#
###################################
#
# Connect to the PCR (DServ)
#
# Written  17jun06  Started with tcs_connect.tcl
#

proc pcr_connect { server parent_win } {

    global AO_GUI_HOME
    source $AO_GUI_HOME/../pcr/tcl/pcr/pcr_globals.tcl

    global reply_data

    set status [catch { PCR_Cmd ${server}_Connect } msg ]

    return 0
}
