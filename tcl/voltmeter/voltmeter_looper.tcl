###################################
#
# voltmeter_looper.tcl
#
###################################
#
# Self calling prodecure to update VoltMeter values on GUI
#
# Written   25mar04    DLM  Started with oap_looper.tcl
#

proc voltmeter_looper { parent_win } {
      
    global AO_GUI_HOME
    source $AO_GUI_HOME/tcl/window/window_globals.tcl
    source $AO_GUI_HOME/tcl/voltmeter/voltmeter_globals.tcl

    voltmeter_measure $parent_win

    if { $VoltMeter_Update && [winfo exists $volt_win] } {
	after 500 voltmeter_looper $parent_win
    }

}
