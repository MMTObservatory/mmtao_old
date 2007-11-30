###################################
#
# tcs_get.tcl
#
###################################
#
# Request the tcs settings from the Field Steering Mirror
# and set the associated gui variables to these values.
# These variables are set so characteristics of the gui are
# automatically updated.  (may not be needed for these variables)
#
# Written  25mar04  DLM  Started with oap_get.tcl
#

proc tcs_get { request parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/tcs/tcs_globals.tcl

    global reply_data

    upvar #0 TCS TCS
    upvar #0 TCS_Format TCS_Format

    if { $TCS_Connected } {

	if { $request == "all" } {

	    set status [catch { TCS_Cmd All } msg ]
	    if { $status } {
		tk_messageBox -message \
		    "Error retrieving All from TCS\n $reply_data" \
		    -parent $parent_win \
		    -icon error -type ok
		return -1
	    }

	    set tcs_lines [split $TCS_Reply "\n"]
	    set tcs_length [lsearch $tcs_lines ".EOF"]
	    if { $tcs_length > 0 } {
		for { set i 0 } { $i<$tcs_length } { incr i } {
		    set temp [split [lindex $tcs_lines $i] ]
		    set TCS([lindex $temp 0]) [lrange $temp 1 end]
		}
		if { [llength $TCS(dec)] > 1 } {
		    set TCS(dec) [lrange $TCS(dec) 1 end]
		}
		foreach el $TCS_Float {
		    set TCS($el) [format $TCS_Format($el) $TCS($el)]
		}
	    } else {
		puts "  tcs_get: nothing read"
	    }

	} else {

	    set status [catch { TCS_Cmd Get $request } msg ]
	    if { $status } {
		tk_messageBox -message \
		    "Error retrieving requested value from TCS\n $reply_data" \
		    -parent $parent_win \
		    -icon error -type ok
		return -1
	    }
	}

    }
    return 0
}
