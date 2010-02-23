###################################
#
# mod_rate_send.tcl
#
###################################
#
# Send the requested slope modulation vector to the PCR
#
# Written  10jan07  DLM  Started with static_apply.tcl
#
proc mod_rate_send { filename parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/mod/mod_globals.tcl

    if { [ file exist $filename ] } {
	set status [catch { Mod_Cmd SendRate $filename } msg ]
	if { $status } {
	    tk_messageBox -message "Error in mod_rate_send: $msg" \
	    -parent $parent_win -icon error -type ok
	    return -1
	} else {
	}
    } else {
	tk_messageBox -message "Error in mod_rate_send:
Slope Modulation Rate file not found
$filename" \
	    -parent $parent_win -icon error -type ok
	    return -1
    }
    
    return 0
}
