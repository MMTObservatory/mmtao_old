###################################
#
# sc_update.tcl
#
###################################
#
# This routine sets up the trace on several variables such that
#   when they are accessed, the gui is updated
#
# Variables:
#   Info_Update - set in info_get.tcl after all the Info variables
#                 have been set
#
###################################
#
# Info Variables
#
trace variable Info_Update w info_update_set
proc info_update_set { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl

    upvar #0 Info Info
#
# Check the status of the Camera and set label and buttons accordingly.
#   This check is a trace on Info(WFSC_Running) because it comes after
#   Info(Initialized) in InfoList so the most recent information is used
#   to setup the gui
#
# If the Main Window exists then update the Camera label
#
    if { [winfo exists $sc_win] } {
	if { $Info(Loop_Running) } {
	    #
	    # Loop is Running
	    #
	    $sc_win.info.running configure -text "Loop Closed" -bg green
	} else {
	    #
	    # Loop Ready but NON Running
	    #
	    $sc_win.info.running configure -text "Loop Open" -bg yellow
	}
    }
}
