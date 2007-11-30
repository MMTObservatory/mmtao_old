###################################
#
# voltmeter_monitor.tcl
#
###################################
#
# Monitor one channel on the HP voltmeter and update its value on
#   the GUI
#
# Written  12may06
#

proc voltmeter_monitor { channel parent_win } {

    global AO_GUI_HOME
    source $AO_GUI_HOME/tcl/voltmeter/voltmeter_globals.tcl

    global reply_data

    voltmeter_set "CONF:RES (@1${channel})" $parent_win
    voltmeter_set "ROUT:MON (@1${channel})" $parent_win
    voltmeter_set "ROUT:MON:STAT ON" $parent_win
    after 500
    set reply [voltmeter_get "ROUT:MON:DATA?" $parent_win]
    set Ohm($channel) [format $Ohm_Format($channel) $reply]
    after 500
    voltmeter_get "ROUT:MON:DATA?" $parent_win
    set Ohm($channel) [format $Ohm_Format($channel) $reply]
    after 500
    voltmeter_get "ROUT:MON:DATA?" $parent_win
    set Ohm($channel) [format $Ohm_Format($channel) $reply]
    after 500
    voltmeter_get "ROUT:MON:DATA?" $parent_win
    set Ohm($channel) [format $Ohm_Format($channel) $reply]

    return 0
}
