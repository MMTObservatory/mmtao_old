###################################
#
# rotator_looper.tcl
#
###################################
#
# Self calling prodecure to update Rotator stage position on GUI
#
# Written   25mar04  DLM  Started with oap_looper.tcl
#
# Altered   18dec07  DLM  Converted to PCR version of the GUI
#

proc rotator_looper { parent_win } {
      
    global PCR_HOME
    source $PCR_HOME/tcl/rotator/rotator_globals.tcl

    if { $Rotator_Tracking } {
	rotator_tracking $parent_win
    }

    after 1000 rotator_looper $parent_win

}

