###################################
#
# science_globals_set.tcl
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
set Science_Connected 0
set Science_Error 0
#
# Setup the list of variables that are received from the Science server.  This
#   lists must be exactly the same as the list received 
#
set Science_Int [list \
		    Loop_Running \
		    WFSC_Frequency \
		    SC_Accepting \
		    offset_closed_delay1 \
		    offset_closed_delay2 \
		    offset_closed_delay3 \
		    offset_closed_delay4 \
		    offset_closed_delay5 \
		   ]

set Science_Float [list \
		      az \
		      alt \
		      rot \
		      pa \
		      rotpa \
		      offrots \
		      ao_off_alt \
		      ao_off_az \
		      ao_off_ra \
		      ao_off_dec \
		      Loop_Gain \
		      WFSC_Rotation \
		      WFSC_Focus \
		      Static_Astig_0 \
		      Static_Astig_45 \
		      Static_Coma_x \
		      Static_Coma_y \
		      Static_Trefoil_0 \
		      Static_Trefoil_30 \
		      Static_Spherical \
		      Seeing_Value \
		      Background_Dynamic \
		      Background_Static \
		      Hexapod_Focus \
		      ]

set Science_Char [list \
		      cat_id \
		      cat_ra \
		      cat_dec \
		      cat_rapm \
		      cat_decpm \
		      cat_epoch \
		      cat_epoch_str \
		      ut \
		      ra \
		      dec \
		      Background_File \
		      RTR_Reconstructor \
		      ]

set Science_List [ concat $Science_Int $Science_Float $Science_Char]
#
# Set all float values and default format
#
foreach el $Science_Int {
    set Science($el) 0
    set Science_Format($el) "%d"
}
#
# Set all float values and default format
#
foreach el $Science_Float {
    set Science($el) 0.0
    set Science_Format($el) "%.2f"
}
set Science_Format(alt) "%.8f"
set Science_Format(az) "%.8f"
#
# This variable is set after all Science variables have been set so the gui
#  can be updated according to the new values of the Science variables.  The
#  update routines are in tcl/gui/gui_update.tcl
#
set Science_Update 1
