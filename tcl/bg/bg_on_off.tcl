###################################
#
# bg_on_off.tcl
#
###################################
#
# Turn on or off the subtraction of a WFSC background.  A background
#   frame must already be loaded into the PCR
#
# Written 25apr07  Started with bg_load.tcl
#

proc bg_on_off { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/bg/bg_globals.tcl

    upvar #0 Info Info

    global reply_data

    if { [string compare Zero_Background $BG_Current ] != 0 } {

	puts "Info(Background_Static) $Info(Background_Static)"

	if { $Info(Background_Static) } {

	    set status [catch { BG_Cmd Off } msg ]
	    if { $status } {
		tk_messageBox -message "Background not turned off" \
		    -parent $parent_win -icon error -type ok
		return -1
	    }

	} else {

	    set status [catch { BG_Cmd On } msg ]
	    if { $status } {
		tk_messageBox -message "Background not turned on" \
		    -parent $parent_win -icon error -type ok
		return -1
	    }

	}
    }

    return 0
	
}
