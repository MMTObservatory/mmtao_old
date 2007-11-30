###################################
#
# static_rms.tcl
#
###################################
#
# Calculate the current RMS wavefront correction
#   the WFC
#
# Written  25apr04
#
# Altered   7jul05 Change name from static_rms.tcl
#
#  **Note: The scale widiget sends the current value of the
#          -variable as the last argument.  Other argument can
#          be included.
#
proc static_rms { value } {

    global PCR_HOME
    source $PCR_HOME/tcl/static/static_globals.tcl

    set sum 0
    foreach el $Static_List {
	set sum [ expr $sum + $Static_Value($el) * $Static_Value($el) ]
    }

    set Static_RMS [ expr int(sqrt( $sum )) ]
    return 0
}
