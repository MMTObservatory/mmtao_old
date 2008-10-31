###################################
#
# science_get.tcl
#
###################################
#
#
# These values are used to setup the gui for the current
#  loop parameters and continously update the gui.
#
# 2008-10-28 skip
#

proc science_get { request parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/science/science_globals.tcl

    global reply_data

    if { $Science_Connected } {

	if { $request == "all" } {

	    set status [catch { Science_Cmd All } msg ]
	    if { $status } {
		science_close $parent_win
		return -1
	    }

#	    puts $Science_Reply
	    set status_lines [split $Science_Reply "\n"]
	    set status_length [lsearch $status_lines "EOF"]
	    if { $status_length > 0 } {
		for { set i 0 } { $i<$status_length } { incr i } {
		    set temp [split [lindex $status_lines $i] ]
		    set Science([lindex $temp 0]) [lrange $temp 1 end]
		}
		foreach el $Science_Float {
		    set Science($el) [format $Science_Format($el) $Science($el)]
		}
		set Science_Update 1
	    } else {
		puts "  science_get: Error, nothing read"
	    }

	} else {

	    set status [catch { Science_Cmd Get $request } msg ]
	    if { $status } {
		science_close $parent_win
		return -1
	    }
	}

    }
    return 0
}
