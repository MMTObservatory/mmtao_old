###################################
#
# mod_offset_send.tcl
#
###################################
#
# Send the requested slope offset vector to the PCR
#
# Written  10jan07  DLM  Started with static_apply.tcl
#
proc mod_offset_send { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/mod/mod_globals.tcl

    set matrix [ file join $Mod_Offset_Dir $Mod_Offset_File ]

    if { [ file exist $matrix ] } {
	set status [catch { Mod_Cmd ReadMatrix $matrix } msg ]
	if { $status } {
	    tk_messageBox -message "Error in mod_offset_send: $msg" \
	    -parent $parent_win -icon error -type ok
	    set Mod_Offset_Value $Mod_Offset_Current
	    return -1
	}
	set status [catch { Mod_Cmd SendOffsets $Mod_Offset_Value } msg ]
	if { $status } {
	    tk_messageBox -message "Error in mod_offset_send: $msg" \
	    -parent $parent_win -icon error -type ok
	    set Mod_Offset_Value $Mod_Offset_Current
	    return -1
	} else {
	    set Mod_Offset_Current $Mod_Offset_Value
	}
    } else {
	tk_messageBox -message "Error in mod_offset_send:
Slope Modulation Offset Matrix file not found
$matrix" \
	    -parent $parent_win -icon error -type ok
	    set Mod_Offset_Value $Mod_Offset_Current
    }
    
    return 0
}
