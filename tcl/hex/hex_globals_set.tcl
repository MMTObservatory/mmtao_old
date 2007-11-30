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
