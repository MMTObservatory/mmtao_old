###################################
#
# info_connect.tcl
#
###################################
#
# Open a connection to the Info Server on the VME
#
# Written  17jun05  DLM  Started with fsm_init.tcl
#

proc info_connect { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/info/info_globals.tcl

    global reply_data

    set status [catch { Info_Cmd Connect } msg ]

    return $status
}
