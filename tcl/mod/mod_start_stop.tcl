###################################
#
# mod_start_stop.tcl
#
###################################
#
# Request the PCR to start or stop slope modulation
#
# Written  10jan07  DLM  Started with static_apply.tcl
#
proc mod_start_stop { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/mod/mod_globals.tcl

    puts "Mod_Running = $Mod_Running"

    if { $Mod_Running } {
	set status [catch { Mod_Cmd Stop } msg ]
	if { $status } {
	    tk_messageBox -message "Error in mod_start_stop: $msg" \
	    -parent $parent_win -icon error -type ok
	    return -1
	}
    } else {
	set status [catch { Mod_Cmd Start } msg ]
	if { $status } {
	    tk_messageBox -message "Error in mod_start_stop: $msg" \
	    -parent $parent_win -icon error -type ok
	    return -1
	}
    }
    
    return 0
}
