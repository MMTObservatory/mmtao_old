###################################
#
# global_trace.tcl
#
###################################
#
# The upper most level TCL code must also source this file
#   The trace command is used to force the update of gui
#   variables on all gui panels.
#
# All TCL source codes should source global_variables.tcl
#   in order for the automatic update to take place.
#
# Update interval for PGplot image
#
set PGplot_Update_Time 100
#
# Values set in PGplot C routines use by Tcl/Tk for display
#
#  Spots Stuff
#
set Spots_Number 0
set New_Spot 0
set Spots_Type 0
set Spots_Slopes 0
set Spots_Arrow 0
set Spots_Realtime 0
#
# Symbol to show new Spot images are being received
#
set Spot_Symbol "?"
#
#  Slopes Stuff
#
set Slopes_Number 0
set New_Slope 0
set Slopes_Type 0
set Slopes_Arrow 0
set Slopes_Realtime 0
#
# Symbol to show new Slopes are being received
#
set Slope_Symbol "?"
#
# Characters used to indicate a new frame has been displayed
#
set PGplot_Symbol(0) "|"
set PGplot_Symbol(1) "/"
set PGplot_Symbol(2) "--"
set PGplot_Symbol(3) "\\"
set PGplot_Symbol(4) "|"
set PGplot_Symbol(5) "/"
set PGplot_Symbol(6) "--"
set PGplot_Symbol(7) "\\"
