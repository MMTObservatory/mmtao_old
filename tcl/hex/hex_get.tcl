###################################
#
# hex_get.tcl
#
###################################
#
# Request information from the Hexapod server
# and set the associated gui variables to these values.
# These variables are set so characteristics of the gui are
# automatically updated.  (may not be needed for these variables)
#
# Written  3feb07  DLM  Started with tcs_get.tcl
#

proc hex_get { request parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/hex/hex_globals.tcl

    global reply_data

    upvar #0 Hex Hex
    upvar #0 Hex_Format Hex_Format

    if { $request == "all" } {

	set status [catch { Hex_Cmd All } msg ]
	if { $status } {
	    tk_messageBox -message \
		"Error retrieving All from Hexapod\n $reply_data" \
		-parent $parent_win \
		-icon error -type ok
	    return -1
	}

	set hex_lines [split $Hex_Reply "\n"]
	set hex_length [lsearch $hex_lines ".EOF"]
	if { $hex_length > 0 } {
	    for { set i 0 } { $i<$hex_length } { incr i } {
		set temp [split [lindex $hex_lines $i] ]
		set Hex([lindex $temp 0]) [lrange $temp 1 end]
	    }
	    if { [llength $Hex(dec)] > 1 } {
		set Hex(dec) [lrange $Hex(dec) 1 end]
	    }
	    foreach el $Hex_Float {
		set Hex($el) [format $Hex_Format($el) $Hex($el)]
	    }
	} else {
	    puts "  hex_get: nothing read"
	}

    } else {

	set status [catch { Hex_Cmd Get $request } msg ]
	if { $status } {
	    tk_messageBox -message \
		"Error retrieving requested value from Hexapod\n $reply_data" \
		-parent $parent_win \
		-icon error -type ok
	    return -1
	}
    }

    return 0
}
