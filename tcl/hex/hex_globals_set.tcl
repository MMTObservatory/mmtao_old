###################################
#
# hex_globals_set.tcl
#
###################################
#
# The upper most level TCL code must also source this file
#   The trace command is used to force the update of gui
#   variables on all gui panels.
#
# All TCL source codes should source global_variables.tcl
#   in order for the automatic update to take place.
#
# Gui flags
#
set Hex_Move WFSC
set Hex_Incr 10
set Hex_Pos_WFSC 0
set Hex_Pos_Science 0
#
# Hex status flags and loooong return string
#
set Hex_Connected 0
set Hex_Error 0
#
# Set all values to 0.0 and default format
#
foreach el $Hex_Float {
    set Hex($el) 0.0
    set Hex_Format($el) "%.2f"
}
#
# Make Alt and Az have more decimal places
#
set Hex_Format(alt) "%.8f"
set Hex_Format(az) "%.8f"
#
# Set all values to "*" and default format
#
foreach el $Hex_Char {
    set Hex($el) "*"
}
#
trace variable Hex_Move w hex_move_set
proc hex_move_set { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    source $PCR_HOME/tcl/hex/hex_globals.tcl

    if { ![string compare WFSC $Hex_Move] } {

	$alignment_page.right.hex.move.pos_label configure \
	    -text "Spots Move Inward"
	$alignment_page.right.hex.move.neg_label configure \
	    -text "Spots Move Outward"

    } else {

	$alignment_page.right.hex.move.pos_label configure \
	    -text "Up"
	$alignment_page.right.hex.move.neg_label configure \
	    -text "Down"

    }
}
