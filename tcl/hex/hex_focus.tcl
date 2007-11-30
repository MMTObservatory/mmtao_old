###################################
#
# hex_focus.tcl
#
###################################
#
# Request information from the Hexapod server
# and set the associated gui variables to these values.
# These variables are set so characteristics of the gui are
# automatically updated.  (may not be needed for these variables)
#
# Written  3feb07  DLM  Started with hex_get.tcl
#

proc hex_focus { request parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/hex/hex_globals.tcl

    upvar #0 Info Info

    global reply_data

    if { $Info(Loop_Running) && [expr abs($request)] > 10 } {

	set status \
	    [tk_messageBox -parent $parent_win -icon question -type yesno \
		 -message "Loop is closed!\nAre you sure you want to make\na $request micron move?" ]

	if { $status == "no" } {

	    return 0
	}

    }

    set status [catch { Hex_Cmd Focus $request } msg ]
    if { $status } {
	tk_messageBox -message \
	    "Error changing Hexapod focus\n $reply_data" \
	    -parent $parent_win \
	    -icon error -type ok
	return -1
    }

    return 0
}
