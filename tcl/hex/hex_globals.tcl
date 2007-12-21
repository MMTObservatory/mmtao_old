###################################
#
# hex_globals.tcl
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
# Gui flags
#
global Hex_Move
global Hex_Incr
#
# Hex status flags and loooong return string
#
global Hex_Connected
global Hex_Error
global Hex_Reply

global Hex_Char
set Hex_Char [list \
		  cat_id \
		  cat_ra \
		  cat_dec \
		  cat_rapm \
		  cat_decpm \
		  cat_epoch \
		  cat_epoch_str \
		  ut \
		  ra \
		  dec ]

global Hex_Float
set Hex_Float [list \
		   az \
		   alt \
		   rot \
		   pa \
		   rotpa \
		   offrots \
		   ao_off_alt \
		   ao_off_az \
		   ao_off_ra \
		   ao_off_dec ]

global Hex_List
set Hex_List [ concat $Hex_Float $Hex_Char]

foreach el $Hex_List {
    global Hex($el)
}

foreach el $Hex_Float {
    global Hex_Format($el)
}
