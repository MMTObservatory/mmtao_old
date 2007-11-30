###################################
#
# topbox_connect_device.tcl
#
###################################
#
# Send a request to connect to a Topbox_Server device 
#
# Written  21sep07  DLM  Started with topbox_enable.tcl
#

proc topbox_connect_device { device parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl

    upvar #0 Status Status

    global reply_data

    set status [catch { Topbox_Cmd Send "${device} Connect" } msg ]

    return $status
}
