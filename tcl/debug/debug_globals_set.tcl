###################################
#
# debug_globals_set.tcl
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
foreach el $Debug_List {
    set debug_$el 0
}
#
# Set individual debug variable to 1 here
#
set debug_Info 0
set debug_VoltMeter 0
set debug_PCR 0
set debug_DClient 0
set debug_Sky 0
set debug_IDL 0
set debug_Static 0
set debug_Mod 0
set debug_BG 0
set debug_Utility 0
set debug_PGplot 0
set debug_Hex 0
set debug_Status 0
set debug_Topbox 0
set debug_Science 0
set debug_TSS 0
