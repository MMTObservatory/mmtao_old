###################################
#
# voltmeter_close.tcl
#
###################################
#
# Close the VoltMeter connections.
#
# Written   25mar04    DLM  Started with oap_close.tcl
#

proc voltmeter_close { parent_win } {

    global reply_data

    set status [catch { VoltMeter_Cmd Close } msg ]

    return 0
}
