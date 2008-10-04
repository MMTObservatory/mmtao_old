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
# TCS status flags and loooong return string
#
global TCS_Connected
global TCS_Error
global TCS_Reply

global TCS_Char
set TCS_Char [list \
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

global TCS_Float
set TCS_Float [list \
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

global TCS_List
set TCS_List [ concat $TCS_Float $TCS_Char]

global TCS
global TCS_Format

