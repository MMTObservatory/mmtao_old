###################################
#
# science_connect.tcl
#
###################################
#
# Open a connection to the Science Server on the VME
#
# 2008-10-28 skip
#

proc science_connect { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/science/science_globals.tcl

    global reply_data

    set status [catch { Science_Cmd Connect } msg ]

    return $status
}
