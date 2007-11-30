###################################
#
# hex_quit.tcl
#
###################################
#
# Close the Hex connection.
#
# Written   3feb07  DLM  Started with tcs_close.tcl
#

proc hex_quit { parent_win } {

    global reply_data

    set status [catch { Hex_Cmd Quit } msg ]

    return 0
}
