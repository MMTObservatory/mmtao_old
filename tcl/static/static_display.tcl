###################################
#
# static_display.tcl
#
###################################
#
# Apply (send to SSC's) the requested static statics corrections
#
# Written  25apr04
#
# Altered   7jul06  DLM  Change name from static_apply.tcl
#
proc static_display { rtr_dir rtr_name parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/static/static_globals.tcl

    set matrix [ file join $rtr_dir $rtr_name ]
    if { [ file exist $matrix ] } {
	set status [catch { Static_Cmd ReadMatrix $matrix } msg ]
	set status [catch { Static_Cmd Display \
			       $Static_Value(Static_Astig_45) \
			       $Static_Value(Static_Astig_0) \
			       $Static_Value(Static_Coma_y) \
			       $Static_Value(Static_Coma_x) \
			       0 \
			       0 \
			       0 } msg ]
    } else {
	tk_messageBox -message "Slope Offset Matrix file not found $matrix" \
	    -parent $parent_win -icon error -type ok
    }
    
    return 0
}
