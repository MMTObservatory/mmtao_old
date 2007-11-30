###################################
#
# topbox_globals_set.tcl
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
# Topbox connection status flags
#
set Topbox_Connected 0
set Topbox_Error 0
#
# Default Increment for motion
#
set Topbox_FSM_Incr 1.0
set Topbox_OAP_Incr 1.0
set Topbox_F15_Incr 1.0
set Topbox_Indigo_Incr 1.0
set Topbox_Rotation_Incr 1.0
set Topbox_Focus_Incr 1.0
#
# Position setting for the Indigo Stage
#
set Indigo_Stow 2.0
set Indigo_Camera 87.0
set Indigo_Laser 161.29
#
# Position setting for the F15 Stage
#
set F15_Stow 2.0
set F15_ND 70.0
set F15_Laser 166.0
#
# Filter Wheel settings
#
set Topbox_ND_List [list \
			"Clear" \
			"ND-0.5" \
			"ND-1.0" \
			"ND-1.5" \
			"ND-2.0" \
			"ND-2.5" \
			"ND-3.0" \
			"Z-Band" \
		       ]

set Topbox_ND_Numbers [list \
			   5 \
			   4 \
			   3 \
			   2 \
			   1 \
			   8 \
			   7 \
			   6 \
			  ]

#
# Set all name in Topbox_ND_Name
#
for {set i 0} {$i < 8} {set i [expr $i + 1]} {
    set Topbox_ND_Name($i) [lindex $Topbox_ND_List $i]
    set Topbox_ND_Value($i) [lindex $Topbox_ND_Numbers $i]
}

set Topbox_ND_Select 0
