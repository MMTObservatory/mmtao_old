###################################
#
# client_close.tcl
#
###################################
#
# Procedure to close all connections opened when GUI was started
#
# Written   24may04    DLM
#
proc client_close { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/client/client_globals.tcl
#
# Close connection to the TCS
#
#    tcs_close $parent_win
#
# Close the connection to the Message Server on the VME
#
#    msg_close $parent_win
#
# Close the connection to the Info Server on the VME
#
    info_close $parent_win
#
# Close the connection to the Topbox Server
#
#    Motor_Client disconnect
#
}
