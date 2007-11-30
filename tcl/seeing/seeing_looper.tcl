###################################
#
# info_looper.tcl
#
###################################
#
# Self calling prodecure to update WFC information on the GUI
#
# Written   26may04    DLM
#
# Altered    2feb06  DLM  Change so the loop is infinite, but the call to retieve
#                           xinformation only occurs if we are connected to the
#                           Info server.
#

proc seeing_looper { parent_win } {
      
    global PCR_HOME
    source $PCR_HOME/tcl/info/info_globals.tcl
    source $PCR_HOME/tcl/seeing/seeing_globals.tcl

    if { $Info_Connected } {
	seeing_get_average $parent_win
    }

    set Seeing_Gain [ expr $Seeing_Average -0.2 ]

    if { $Gain_Auto } {
	if { $Seeing_Gain > 0.25 } {
	    loop_set_gain $Seeing_Gain $parent_win
	}
    }

    if { $Seeing_Auto } {
	after $Seeing_Update_Rate seeing_looper $parent_win
    }

}

