###################################
#
# mod_globals_set.tcl
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
#   This should be a mirror mode file rather than
#   a zernike mode file

set Mod_Offset_Dir $PCR_HOME/data
set Mod_Offset_File 56m_all.slo_off_zernike
#
# Variable for the current offset vector loaded in the PCR
#
set Mod_Offset_Value None
set Mod_Offset_Current None
#
# Directory and default file for slope offset vectors
#   This should be a mirror mode file rather than
#   a zernike mode file
#
global MMTAO_HOME
set Mod_Rate_Dir ${MMTAO_HOME}/RTR/time_modulation
set Mod_Rate_File 56m_all.slo_off_zernike
#
# Variables for the possible rates and current offset modulation rate loaded
#   in the PCR
#
set Mod_Rate_List [list \
			 None \
			 1_Hz \
			 2_Hz \
			 5_Hz \
			 11_Hz \
			 55_Hz \
			]

set Mod_Rate_File None
set Mod_Rate_Value None
#
# Flag indicating PCR slope modulation is running
#
set Mod_Running 0
