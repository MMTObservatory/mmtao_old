###################################
#
# global_variables
#
###################################
#
# This file is sourced by the upper most level of TCL source code
#   (ie. AO_GUI_startup.tcl) as well as the all lower level of
#   tcl code that need these variables.
#
# The upper most level TCL code must also source the file
#   global_trace.tcl.  In this file, the trace command is used
#   to force the update of gui variables on all gui panels.
#
# Update time for PGplot image
#
global PGplot_Update_Time
#
# Values set in PGplot C routines use by Tcl/Tk for display
#
#  Spots Stuff
#
global Spots_Number
global New_Spot
global Spots_Type
global Spots_Slopes
global Spots_Arrow
global Spots_Realtime
#
# The "running" symbol to display for specfic images
#
global Spot_Symbol
#
#  Slopes Stuff
#
global Slopes_Number
global New_Slope
global Slopes_Type
global Slopes_Arrow
global Slopes_Realtime
#
# The "running" symbol to display for specfic images
#
global Slope_Symbol
#
# Characters used to indicate a new frame has been displayed
#
global PGplot_Symbol
