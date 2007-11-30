###################################
#
# info_get.tcl
#
###################################
#
# Request the info values from WFC.  
#  ->Info($el)
#  ->Seeing_*($el)
#
# These values are used to setup the gui for the current
#  loop parameters and continously update the gui.
#
# Written  25mar04  DLM  Started with oap_get.tcl
#
# Altered   8feb06  DLM  Cleaned up and added see_values
#

proc info_get { request parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/info/info_globals.tcl
    source $PCR_HOME/tcl/seeing/seeing_globals.tcl

    global reply_data

    if { $Info_Connected } {

	if { $request == "all" } {

	    set status [catch { Info_Cmd All } msg ]
	    if { $status } {
		info_close $parent_win
		pcr_close $parent_win
		return -1
	    }

#	    puts $Info_Reply
	    set info_lines [split $Info_Reply "\n"]
	    set info_length [lsearch $info_lines "EOF"]
	    if { $info_length > 0 } {
		for { set i 0 } { $i<$info_length } { incr i } {
		    set temp [split [lindex $info_lines $i] ]
		    set Info([lindex $temp 0]) [lrange $temp 1 end]
		}
		foreach el $Info_Float {
		    set Info($el) [format $Info_Format($el) $Info($el)]
		}
		set Info_Update 1
	    } else {
		puts "  info_get: Error, nothing read"
	    }

	} elseif { $request == "see_vals" } {

	    set status [catch { Info_Cmd See_Vals } msg ]
	    if { $status } {
		info_close $parent_win
		pcr_close $parent_win
		return -1
	    }

	    set info_lines [split $Info_Reply "\n"]
	    set Seeing_Length [lsearch $info_lines "EOF"]
	    if { $Seeing_Length > 0 } {
		for { set i 0 } { $i<$Seeing_Length } { incr i } {
		    set Seeing_String($i) [split [lindex $info_lines $i] ]
		    set Seeing_Label($i) [lrange $Seeing_String($i) 3 3 ]
		    set temp [split $Seeing_Label($i) ":" ]
		    set Seeing_Time($i) [expr [lrange $temp 0 0] + [lrange $temp 1 1]/60. + [lrange $temp 2 2]/3600.  ]
		    set Seeing_Value($i) [lrange $Seeing_String($i) end end]
		}
	    }

	} else {

	    set status [catch { Info_Cmd Get $request } msg ]
	    if { $status } {
		info_close $parent_win
		pcr_close $parent_win
		return -1
	    }
	}

    }
    return 0
}
