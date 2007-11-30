###################################
#
# hex_set.tcl
#
###################################
#
# Request the hex settings from the Field Steering Mirror
# and set the associated gui variables to these values.
# These variables are set so characteristics of the gui are
# automatically updated.  (may not be needed for these variables)
#
# Written  25mar04  DLM  Started with oap_get.tcl
#

proc hex_set {args} {
#
# Check for correct number of arguments
#
    set num_args 2
    set args [split $args { }]
    if { [llength $args] < $num_args } {
	set error_msg "Wrong number of args in call to hex_set. \
		Expected $num_args and found [expr [llength $args]+1]"
	tk_messageBox -parent . -title "Argument Error" -type ok -icon error \
		-message $error_msg
	return 1
    }
#
# Set the passed args to the proper local variables
#
    set request [lindex $args 0]
    if { [llength $args] == 3 } {
	set delta [lindex $args 1]
	set parent_win [lindex $args 2]
    } elseif { [llength $args] == 4 } {
	set delta_1 [lindex $args 1]
	set delta_2 [lindex $args 2]
	set parent_win [lindex $args 3]
    } else {
	set error_msg "Too many args in call to hex_set. Found [expr [llength $args]+1]"
	tk_messageBox -parent . -title "Argument Error" -type ok -icon error \
	    -message $error_msg
	return -1
    }
#
    global PCR_HOME
    source $PCR_HOME/tcl/hex/hex_globals.tcl

    global reply_data

    global error_msg

    upvar #0 Hex Hex

    if { $Hex_Connected } {
	if { $request == "setaaao" || $request == "setrdao" } {
	    hex_get all $parent_win
	    if { $request == "setaaao" } {
		set value_1 [expr $Hex(ao_off_alt) + $delta_1 ]
		set value_2 [expr $Hex(ao_off_az) + $delta_2 ]
	    }
	    if { $request == "setrdao" } {
		set value_1 [expr $Hex(ao_off_ra) + $delta_1 ]
		set value_2 [expr $Hex(ao_off_dec) + $delta_2 ]
	    }
	    if { $value_1 > 50.0 || $value_1 < -50.0 } {
		set error_msg "Error in hex_set: Alt/RA too large => $value_1"
		tk_messageBox -message $error_msg \
		    -parent $parent_win \
		    -icon error -type ok
		return -1
	    }
	    if { $value_2 > 50.0 || $value_2 < -50.0 } {
		set error_msg "Error in hex_set: Az/Dec too large => $value_2"
		tk_messageBox -message $error_msg \
		    -parent $parent_win \
		    -icon error -type ok
		return -1
	    }
	    set string "$request\n$value_1\n$value_2"
	    set status [catch { Hex_Cmd Set $string } msg ]
	    if { $status } {
		set error_msg "Error retrieving requested value from Hex\n $reply_data"
		tk_messageBox -message $error_msg \
		    -parent $parent_win \
		    -icon error -type ok
		return -1
	    }
	} elseif { $request == "slew" } {
	    set value_1 [expr $Hex(alt) + $delta_1 ]
	    set value_2 [expr $Hex(az) + $delta_2 ]
	    set string "$request\n$value_1\n$value_2\n$Hex(rot)"
	    set status [catch { Hex_Cmd Set $string } msg ]
	    if { $status } {
		set error_msg "Error retrieving requested value from Hex\n $reply_data"
		tk_messageBox -message $error_msg \
		    -parent $parent_win \
		    -icon error -type ok
		return -1
	    }
	} else {
	    set error_msg "hex_set: request must be either setaaao or setrdao"
	    tk_messageBox -message $error_msg \
		-parent $parent_win \
		-icon error -type ok
	    return -1
	}
    }
    return 0
}
