###################################
#
# global_trace.tcl
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
# TCS status flags and loooong return string
#
set TCS_Connected 0
set TCS_Error 0
#
# Set all values to 0.0 and default format
#
foreach el $TCS_Float {
    set TCS($el) 0.0
    set TCS_Format($el) "%.2f"
}
#
# Make Alt and Az have more decimal places
#
set TCS_Format(alt) "%.8f"
set TCS_Format(az) "%.8f"
#
# Set all values to "*" and default format
#
foreach el $TCS_Char {
    set TCS($el) "*"
}
