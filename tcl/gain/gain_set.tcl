###################################
#
# gain_set.tcl
#
###################################
#
# Determine the seeing dependent gain
#
# Written  7apr06  DLM
#
###################################


proc gain_set { parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/info/info_globals.tcl
    source $PCR_HOME/tcl/seeing/seeing_globals.tcl

    global reply_data

    if { $Info_Connected } {

	set status [catch { Info_Cmd See_Vals } msg ]
	if { $status } {
	    tk_messageBox -message \
		"Error retrieving See_Vals from Info\n $reply_data" \
		-parent $parent_win \
		-icon error -type ok
	    return -1
	}

	set info_lines [split $Info_Reply "\n"]
	set Seeing_Length [lsearch $info_lines "EOF"]
	if { $Seeing_Length > 0 } {
	    for { set i 0 } { $i<$Seeing_Length } { incr i } {
		set Seeing_String($i) [split [lindex $info_lines $i] ]
		set Seeing_Label($i) [lrange $Seeing_String($i) 3 3 ]
		set temp [split $Seeing_Label($i) ":" ]
		if { [ string index [lrange $temp 0 0] 0 ] == 0 } {
		    set hours [ string range [lrange $temp 0 0] 1 end ]
		} else {
		    set hours [lrange $temp 0 0]
		}
		if { [ string index [lrange $temp 1 1] 0 ] == 0 } {
		    set minutes [ string range [lrange $temp 1 1] 1 end ]
		} else {
		    set minutes [lrange $temp 1 1]
		}
		if { [ string index [lrange $temp 2 2] 0 ] == 0 } {
		    set seconds [ string range [lrange $temp 2 2] 1 end ]
		} else {
		    set seconds [lrange $temp 2 2]
		}
		set Seeing_Time($i) [expr $hours + $minutes/60. + $seconds/3600.  ]
		set Seeing_Value($i) [lrange $Seeing_String($i) end end]
	    }
	}
    }

    return 0
}
