###################################
#
# status_get.tcl
#
###################################
#
# Request the status values from WFC.  
#  ->Status($el)
#  ->Seeing_*($el)
#
# These values are used to setup the gui for the current
#  loop parameters and continously update the gui.
#
# Written  25mar04  DLM  Started with oap_get.tcl
#
# Altered   8feb06  DLM  Cleaned up and added see_values
#

proc status_get { request parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/status/status_globals.tcl

    global reply_data

    if { $Status_Connected } {

	if { $request == "all" } {

	    set status [catch { Status_Cmd All } msg ]
	    if { $status } {
		status_close $parent_win
		topbox_close $parent_win
		return -1
	    }

#	    puts $Status_Reply
	    set status_lines [split $Status_Reply "\n"]
	    set status_length [lsearch $status_lines "EOF"]
	    if { $status_length > 0 } {
		for { set i 0 } { $i<$status_length } { incr i } {
		    set temp [split [lindex $status_lines $i] ]
		    set Status([lindex $temp 0]) [lrange $temp 1 end]
		}
		foreach el $Status_Float {
		    set Status($el) [format $Status_Format($el) $Status($el)]
		}
		set Status_Update 1
	    } else {
		puts "  status_get: Error, nothing read"
	    }

	} else {

	    set status [catch { Status_Cmd Get $request } msg ]
	    if { $status } {
		status_close $parent_win
		topbox_close $parent_win
		return -1
	    }
	}

    }
    return 0
}
