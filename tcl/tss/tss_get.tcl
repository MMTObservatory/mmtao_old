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

# Just for the record, the new tss_server seems to give us 48 lines,
# the last of which is just an empty line (for who knows what reason).
# 0 is the first thing, and 46 is the ".EOF"
#		set ll [llength $status_lines]
#		puts "lines in reply: $ll"
#		puts [ lindex $status_lines 0 ]
#		puts [ lindex $status_lines 46 ]
#		puts [ lindex $status_lines 47 ]

# tjt 1-9-2009   Just today this began to fail, and I changed the search
# string in the following from "EOF" to ".EOF" and now it works!?
# Don't ask me what is up.  Adding the -glob switch didn't do a thing,
# so it is as if it is doing an exact match come hell or high water.
# Typical tcl behaviour.

	    set status_length [lsearch $status_lines ".EOF"]

#		puts "tss_get length $status_length"

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
