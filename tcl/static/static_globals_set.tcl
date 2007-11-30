###################################
#
# static_globals_set.tcl
#
###################################
#
# The *_globals.tcl file is sourced by the upper most level of TCL
#   source code (ie. AO_GUI_startup.tcl) as well as the all lower
#   level of tcl code that need these variables.
#
# The *_globals_set.tcl file is sourced by only the upper most level
#   TCL code (usually *_startup.tcl) so values are set to the global
#   variables only once.  In *_globals_set.tcl file, the trace command
#   can be used to force the update of gui variables, colors, buttons,
#   etc on all gui panels.
#
###################################
#
set Static_List [list \
			 Static_Astig_0 \
			 Static_Astig_45 \
			 Static_Coma_x \
			 Static_Coma_y \
			]

foreach el $Static_List {
    set Static_Set($el) 0
    set Static_Value($el) 0
    set Static_Max($el) 150
}

set astig_0_set 0
set astig_45_set 0
set coma_x_set 0
set coma_y_set 0
set trefoil_0_set 0
set trefoil_30_set 0
set spherical_set 0
#
# Value of the current statics being removed
#
set astig_0_value 0.0
set astig_45_value 0.0
set coma_x_value 0.0
set coma_y_value 0.0
set trefoil_0_value 0.0
set trefoil_30_value 0.0
set spherical_value 0.0
#
# Set the limits of the static corrections we can apply
#
set astig_0_max 150
set astig_45_max 150
set coma_x_max 150
set coma_y_max 150
set trefoil_0_max 150
set trefoil_30_max 150
set spherical_max 150
#
# RMS of the request/applied correction
#
set Static_RMS 0
#
# Location of zernike to slope matrix
#
#   **NOTE: Chane the path and file name here for Wack-A-Speckle tests
#
set Base $PCR_HOME/tcl/static
set File 56m_all.slo_off_zernike
#
# Scripts that are executed when a variable is modified
#
trace variable Static_Set w static_set_trace
proc static_set_trace { name element op } {
#
    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
#
    upvar #0 Static_Set Static_Set
    upvar #0 Static_Value Static_Value
#
    if { $Static_Set($element) } {
	$static_page.modes.slider_$element configure -state normal
    } else {
	$static_page.modes.slider_$element configure -state disabled
	set Static_Value($element) 0
	static_rms 0
    }
}