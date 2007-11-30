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
proc mod_rate_send { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/mod/mod_globals.tcl

    set filename [ file join $Mod_Rate_Dir ${Mod_Rate_Value}.mod_rate ]

    if { [ file exist $filename ] } {
	set status [catch { Mod_Cmd SendRate $filename } msg ]
	if { $status } {
	    tk_messageBox -message "Error in mod_rate_send: $msg" \
	    -parent $parent_win -icon error -type ok
	    set Mod_Rate_Value $Mod_Rate_File
	    return -1
	} else {
	    set Mod_Rate_File $Mod_Rate_Value
	}
    } else {
	tk_messageBox -message "Error in mod_rate_send:
Slope Modulation Rate file not found
$filename" \
	    -parent $parent_win -icon error -type ok
	    set Mod_Rate_Value $Mod_Rate_File
	    return -1
    }
    
    return 0
}
