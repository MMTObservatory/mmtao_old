###################################
#
# tss_connect.tcl
#
###################################
#
# Open a connection to the TSS Server on the VME
#
# 2008-11-03 skip
#

proc tss_connect { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/tss/tss_globals.tcl

    global reply_data

    set status [catch { TSS_Cmd Connect } msg ]

    return $status
}
