###################################
#
# tss_globals_set.tcl
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
set TSS_Connected 0
set TSS_Error 0
#
# Setup the list of variables that are received from the TSS server.  This
#   lists must be exactly the same as the list received 
#
set TSS_Int [list \
		    total_current \
		   ]

set TSS_Float [list \
		      dummy_float \
		      ]

set TSS_Char [list \
		      dummy_char \
		      ]

set TSS_List [ concat $TSS_Int $TSS_Float $TSS_Char]
#
# Set all float values and default format
#
foreach el $TSS_Int {
    set TSS($el) 0
    set TSS_Format($el) "%d"
}
#
# Set all float values and default format
#
foreach el $TSS_Float {
    set TSS($el) 0.0
    set TSS_Format($el) "%.2f"
}
#
# This variable is set after all TSS variables have been set so the gui
#  can be updated according to the new values of the TSS variables.  The
#  update routines are in tcl/gui/gui_update.tcl
#
set TSS_Update 1
