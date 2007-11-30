###################################
#
# topbox_globals.tcl
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
global Topbox_Connected
global Topbox_Error
#
# Default Increment for motion
#
global Topbox_FSM_Incr
global Topbox_OAP_Incr
global Topbox_F15_Incr
global Topbox_Indigo_Incr
global Topbox_Rotation_Incr
global Topbox_Focus_Incr
#
# Position setting for the Indigo Stage
#
global Indigo_Stow
global Indigo_Camera
global Indigo_Laser
#
# Position setting for the F15 Stage
#
global F15_Stow
global F15_ND
global F15_Laser
#
# Filter Wheel settings
#
global Topbox_ND_Select
global Topbox_ND_Name
global Topbox_ND_Value
