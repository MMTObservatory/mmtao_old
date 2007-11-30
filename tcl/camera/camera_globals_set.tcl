###################################
#
# camera_globals_set.tcl
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
for { set k 0 } { $k < 4 } { incr k } {
    set Camera_Bias($k) 0
}

set Camera_Bias_Max 700

set Camera_Default_Bias(0) 460
set Camera_Default_Bias(1) 465
set Camera_Default_Bias(2) 511
set Camera_Default_Bias(3) 509
