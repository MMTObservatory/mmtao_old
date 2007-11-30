###################################
#
# static_globals.tcl
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
global Static_List
global Static_Set
global Static_Value
global Static_Max
#
global astig_0_set
global astig_45_set
global coma_x_set
global coma_y_set
global trefoil_0_set
global trefoil_30_set
global spherical_set
#
# Value of the current statics being removed
#
global astig_0_value
global astig_45_value
global coma_x_value
global coma_y_value
global trefoil_0_value
global trefoil_30_value
global spherical_value
#
# Set the limits of the static corrections we can apply
#
global astig_0_max
global astig_45_max
global coma_x_max
global coma_y_max
global trefoil_0_max
global trefoil_30_max
global spherical_max
#
# RMS of the request/applied correction
#
global Static_RMS
#
# Location of zernike to slope matrix
#
global Base
global File

