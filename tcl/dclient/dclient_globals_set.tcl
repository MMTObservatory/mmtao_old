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
set DClient_DM_Save_List [list \
			      Spots \
			      Cmd \
			      Slope \
			      Cur \
			      RTR \
			      Pos ]

foreach el $DClient_DM_Save_List {
    set DClient_DM_Save($el) 1
}

set DClient_List [list WFSC Spots Slopes Slope RTR Pos Cmd Cur ]

#foreach el $DClient_List {
#    set Display_RT($el) 0
#    set Display_RT_Rate($el) 200
#}

set DClient_WFSC_Frames 1000
set DClient_DDS_Frames 1000

set DClient_All "Check All"
