###################################
#
# tss_get.tcl
#
###################################
#
#
# These values are used to setup the gui for the current
#  loop parameters and continously update the gui.
#
# 2008-11-03 skip
#

proc tss_get { request parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/tss/tss_globals.tcl

    global reply_data

    if { $TSS_Connected } {

	if { $request == "all" } {

	    set status [catch { TSS_Cmd All } msg ]
	    if { $status } {
		tss_close $parent_win
		return -1
	    }

#	    puts $TSS_Reply
	    set status_lines [split $TSS_Reply "\n"]
	    set status_length [lsearch $status_lines "EOF"]
	    if { $status_length > 0 } {
		for { set i 0 } { $i<$status_length } { incr i } {
		    set temp [split [lindex $status_lines $i] ]
		    set TSS([lindex $temp 0]) [lrange $temp 1 end]
		}
		foreach el $TSS_Float {
		    set TSS_Format($el) $TSS($el)]
		}
		set TSS_Update 1
	    } else {
		puts "  tss_get: Error, nothing read"
	    }

	} else {

	    set status [catch { TSS_Cmd Get $request } msg ]
	    if { $status } {
		tss_close $parent_win
		return -1
	    }
	}

    }
    return 0
}
