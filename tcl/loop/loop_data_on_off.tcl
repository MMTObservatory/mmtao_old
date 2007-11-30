################################################## 
#
# loop_data_on_off.tcl
#
################################################## 
#
# Routine to either turn on or turn off loop data going the DDS
#
# Written  8apr06  DLM
#
##################################################

proc loop_data_on_off { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/loop/loop_globals.tcl

    upvar #0 Info Info

    if { $Info(Loop_Data_To_DDS) } {
	loop_data_to_dds 0 $parent_win
    } else {
	loop_data_to_dds 327 $parent_win
    }

    return 0
}
