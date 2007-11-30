###################################
#
# recon_set.tcl
#
###################################
#
# Send a request to change the reconstructor loaded in the PCR.
#   The names are define by Vidhya.
#
# Written  19apr07
#

proc recon_set { name parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/recon/recon_globals.tcl

    global reply_data

    set status [catch { PCR_Cmd Recon_Set $name } msg ]

    return 0
}

