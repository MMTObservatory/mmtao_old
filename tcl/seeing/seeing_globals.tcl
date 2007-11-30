###################################
#
# seeing_globals.tcl
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
# The average seeing calculated by Info_AverageSeeing
#
global Seeing_Average
global Seeing_Gain
global Seeing_Auto
global Seeing_Update_Rate
global Gain_Auto
#
# Seeing value arrays
#
global Seeing_Length
global Seeing_String
global Seeing_Label
global Seeing_Time
global Seeing_Value
