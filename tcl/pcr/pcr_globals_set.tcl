###################################
#
# pcr_globals_set.tcl
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
#
# PCR connection status flags
#
set PCR_Connected 0
set PCR_Error 0
#
set PCR_Save_Data 0
#
set PCR_Save_List [list \
		       WFSC \
		       Slope \
		       RTR \
		       Cmd \
		       Cur \
		       Pos ]

foreach el $PCR_Save_List {
    set PCR_Save($el) 1
}