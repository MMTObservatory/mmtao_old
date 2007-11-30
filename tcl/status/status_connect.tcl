###################################
#
# status_connect.tcl
#
###################################
#
# Open a connection to the Status Server on the VME
#
# Written  17jun05  DLM  Started with fsm_init.tcl
#

proc status_connect { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/status/status_globals.tcl

    global reply_data

    set status [catch { Status_Cmd Connect } msg ]

    return $status
}
