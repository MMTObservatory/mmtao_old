###################################
#
# topbox_connect.tcl
#
###################################
#
# Connect to the Topbox
#
# Written  14sep07  Started with pcr_connect.tcl
#

proc topbox_connect { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl

    global reply_data

    set status [catch { Topbox_Cmd Connect } msg ]

    return 0
}
