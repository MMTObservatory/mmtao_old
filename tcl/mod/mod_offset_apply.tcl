###################################
#
# mod_offset_apply.tcl
#
###################################
#
# Send the requested slope offset vector to the PCR
#
# Written  10jan07  DLM  Started with static_apply.tcl
#
proc mod_offset_apply { mode_num parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/mod/mod_globals.tcl

    set matrix [ file join $Mod_Offset_File $Mod_Offset_File ]
    if { [ file exist $matrix ] } {
	set status [catch { Mod_Cmd ReadMatrix $matrix } msg ]
	if { $status } {
	    tk_messageBox -message "Error in mod_offset_apply: $msg" \
	    -parent $parent_win -icon error -type ok
	    return -1
	}
	set status [catch { Mod_Cmd Apply $mode_num } msg ]
	if { $status } {
	    tk_messageBox -message "Error in mod_offset_apply: $msg" \
	    -parent $parent_win -icon error -type ok
	    return -1
	}
    } else {
	tk_messageBox -message "Error in mod_offset_apply:
Slope Modulation Offset Matrix file not found
$matrix" \
	    -parent $parent_win -icon error -type ok
    }
    
    return 0
}
