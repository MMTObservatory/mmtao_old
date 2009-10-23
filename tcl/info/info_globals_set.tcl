###################################
#
# info_globals_set.tcl
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
set Info_Connected 0
set Info_Error 0
#
# Setup the list of variables that are received from the Info server.  This
#   lists must be exactly the same as the list received 
#
set Info_Int [list \
		  PCR_Client_Running \
		  PCR_CMD_Connected \
		  PCR_CMD_Error \
		  PCR_WFSC_Connected \
		  PCR_WFSC_Error \
		  PCR_WFSC_Rate \
		  PCR_WFSC_Receiving_Data \
		  PCR_DDS_Connected \
		  PCR_DDS_Error \
		  PCR_DDS_Receiving_Data \
		  PCR_DDS_Rate \
		  PCR_ERROR_Connected \
		  PCR_ERROR_Error \
		  DServ_WFSC_Running \
		  DServ_DDS_Running \
		  WFSC_Initialized \
		  WFSC_Running \
		  WFSC_Frequency \
		  WFSC_Bias_0 \
		  WFSC_Bias_1 \
		  WFSC_Bias_2 \
		  WFSC_Bias_3 \
		  WFSC_Maximum_Pixel \
		  WFSC_Average_Counts \
		  WFSC_Temperature \
		  DM_Running \
		  Static_Running \
		  Static_Astig_0 \
		  Static_Astig_45 \
		  Static_Coma_x \
		  Static_Coma_y \
		  Static_Trefoil_0 \
		  Static_Trefoil_30 \
		  Static_Spherical \
		  Modulation_Running \
		  Seeing_Running \
		  Seeing_Modulating \
		  Seeing_Total_Modes \
		  Seeing_Mode \
		  Seeing_Period \
		  Seeing_Frequency \
		  Seeing_Gain_Change \
		  Science_Observing \
		  Background_Dynamic \
		  Background_Static \
		  Loop_Running ]

set Info_Float [list \
		    WFSC_Period \
		    WFSC_Temp_1 \
		    WFSC_Temp_2 \
		    WFSC_Pupil_Tip \
		    WFSC_Pupil_Tilt \
		    WFSC_Tip \
		    WFSC_Tilt \
		    WFSC_Focus \
		    WFSC_Astig_0 \
		    WFSC_Astig_45 \
		    WFSC_Coma_X \
		    WFSC_Coma_Y \
		    WFSC_Trefoil_0 \
		    WFSC_Trefoil_30 \
		    WFSC_Spherical \
		    DM_Tip \
		    DM_Tilt \
		    DM_Focus \
		    DM_Astig_0 \
		    DM_Astig_45 \
		    DM_Coma_X \
		    DM_Coma_Y \
		    DM_Trefoil_0 \
		    DM_Trefoil_30 \
		    DM_Spherical \
		    Seeing_Scale_Factor \
		    Seeing_Period \
		    Seeing_Value \
		    Strehl_Value \
		    Loop_Gain \
		    P_Gain \
		    I_Gain \
		    D_Gain \
		    Loop_Save_Data ]

set Info_Char [list \
		   MMTAO_Home \
		   PCR_HOME \
		   Setup_Dir \
		   Recon_Name \
		   WFSC_Size \
		   WFSC_Source \
		   Seeing_Mode_File \
		   Background_File ]

set Info_List [ concat $Info_Int $Info_Float $Info_Char]
#
# Set all float values and default format
#
foreach el $Info_Int {
    set Info($el) 0
    set Info_Format($el) "%d"
}
#
# Set all float values and default format
#
foreach el $Info_Float {
    set Info($el) 0.0
    set Info_Format($el) "%.2f"
}
set Info_Format(WFSC_Period) "%.1f"
set Info_Format(User_Gain) "%.3f"
set Info_Format(Loop_Gain) "%.2f"
set Info_Format(P_Gain) "%.2f"
set Info_Format(I_Gain) "%.2f"
set Info_Format(D_Gain) "%.2f"
#
# Set all char values and default format
#
foreach el $Info_Char {
    set Info($el) *
    set Info_Format($el) "%s"
}
#
# This variable is set after all Info variables have been set so the gui
#  can be updated according to the new values of the Info variables.  The
#  update routines are in tcl/gui/gui_update.tcl
#
set Info_Update 1
