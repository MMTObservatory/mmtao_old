###################################
#
# tcs_quit.tcl
#
###################################
#
# Close the TCS connection.
#
# Written   25mar04    DLM  Started with oap_close.tcl
#

proc tcs_quit { parent_win } {

    global reply_data

    set status [catch { TCS_Cmd Quit } msg ]

    return 0
}
