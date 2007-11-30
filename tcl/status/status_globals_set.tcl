###################################
#
# status_globals_set.tcl
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
set Status_Connected 0
set Status_Error 0
#
# Setup the list of variables that are received from the Status server.  This
#   lists must be exactly the same as the list received 
#
set Status_Int [list \
		    FSM_Tip_Connected \
		    FSM_Tip_Connection_Error \
		    FSM_Tip_Home \
		    FSM_Tip_Moving \
		    FSM_Tip_In_Position \
		    FSM_Tip_Error \
		    FSM_Tip_Enabled \
		    FSM_Tip_Negative \
		    FSM_Tip_Positive \
		    FSM_Tilt_Connected \
		    FSM_Tilt_Connection_Error \
		    FSM_Tilt_Home \
		    FSM_Tilt_Moving \
		    FSM_Tilt_In_Position \
		    FSM_Tilt_Error \
		    FSM_Tilt_Enabled \
		    FSM_Tilt_Negative \
		    FSM_Tilt_Positive \
		    OAP_Tip_Connected \
		    OAP_Tip_Connection_Error \
		    OAP_Tip_Home \
		    OAP_Tip_Moving \
		    OAP_Tip_In_Position \
		    OAP_Tip_Error \
		    OAP_Tip_Enabled \
		    OAP_Tip_Negative \
		    OAP_Tip_Positive \
		    OAP_Tip_Event_2 \
		    OAP_Tilt_Connected \
		    OAP_Tilt_Connection_Error \
		    OAP_Tilt_Home \
		    OAP_Tilt_Moving \
		    OAP_Tilt_In_Position \
		    OAP_Tilt_Error \
		    OAP_Tilt_Enabled \
		    OAP_Tilt_Negative \
		    OAP_Tilt_Positive \
		    OAP_Tilt_Event_2 \
		    Indigo_Connected \
		    Indigo_Connection_Error \
		    Indigo_Home \
		    Indigo_Moving \
		    Indigo_Enabled \
		    Indigo_Negative \
		    Indigo_Positive \
		    F15_Connected \
		    F15_Connection_Error \
		    F15_Home \
		    F15_Moving \
		    F15_Enabled \
		    F15_Negative \
		    F15_Positive \
		    Fwheel_Connected \
		    Fwheel_Connection_Error \
		    Fwheel_Home \
		    Fwheel_Position \
		    Fwheel_Enable \
		    Fwheel_Name \
		    Fwheel_Moving \
		    Power_Connected \
		    Power_Connection_Error \
		    Power_WFSC \
		    Power_Peltier \
		    Power_Stella \
		    Power_Alignment_Laser \
		    Power_F15_Laser \
		    Power_Indigo \
		    Power_Spare1 \
		    Power_Spare2 \
		    Focus_Connected \
		    Focus_Connection_Error \
		    Focus_Home \
		    Focus_Moving \
		    Focus_Enabled \
		    Focus_Negative \
		    Focus_Positive \
		    Rotation_Connected \
		    Rotation_Connection_Error \
		    Rotation_Home \
		    Rotation_Moving \
		    Rotation_Enabled \
		    Rotation_Negative \
		    Rotation_Positive \
		   ]

set Status_Float [list \
		      FSM_Tip_Position \
		      FSM_Tilt_Position \
		      OAP_Tip_Position \
		      OAP_Tilt_Position \
		      Indigo_Position \
		      F15_Position \
		      Focus_Position \
		      Rotation_Position \
		      ]

set Status_List [ concat $Status_Int $Status_Float ]
#
# Set all float values and default format
#
foreach el $Status_Int {
    set Status($el) 0
    set Status_Format($el) "%d"
}
#
# Set all float values and default format
#
foreach el $Status_Float {
    set Status($el) 0.0
    set Status_Format($el) "%.2f"
}
#
# This variable is set after all Status variables have been set so the gui
#  can be updated according to the new values of the Status variables.  The
#  update routines are in tcl/gui/gui_update.tcl
#
set Status_Update 1
