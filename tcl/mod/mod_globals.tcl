###################################
#
# mod_globals.tcl
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
# Directory and default file for slope offset vectors
#
global Mod_Offset_Dir
global Mod_Offset_File
#
# Variable for the current and requested offset vector loaded in the PCR
#
global Mod_Offset_Value
global Mod_Offset_Current
#
# Directory containing modulation rate files
#
global Mod_Rate_Dir
global Mod_Rate_File
#
# Variables for the possible rates and current offset modulation rate loaded
#   in the PCR
#
global Mod_Rate_List
global Mod_Rate_File
global Mod_Rate_Value
#
# Flag indicating PCR slope modulation is running
#
global Mod_Running
