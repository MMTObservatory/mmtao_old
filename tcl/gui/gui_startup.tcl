#++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# gui_startup.tcl for the PCR GUI
#
# This script reads all of the TclTk files needed to run
#   the main MMTAO gui.
#
#   1) All proceedures are sourced so all proc's are available to
#      all other proc's.
#   2) Global variables are created (files called *_globals.tcl) and 
#      set (files called *_globals_set.tcl).  Any proceedure which needs
#      a set of global variables then only sources the *_globals_set.tcl
#      file.
#   3) The initial main tcl window is destroyed
#   4) The proc that creates the main window is called
#
# Altered  22jan06  Started with gui_startup.tcl for the VME
#
#++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# The tcsh shell environment variable contains the version of the
#   current gui.  This version is made global so all proc's know
#   where *_globals_set.tcl files live
#
global env
global PCR_HOME
set PCR_HOME $env(PCR_HOME)
global MMTAO_HOME
set MMTAO_HOME $env(MMTAO_HOME)
global USER_HOME
set USER_HOME $env(HOME)
global MMTAO_DATA_DIR
set MMTAO_DATA_DIR $env(MMTAO_DATA_DIR)
#
# global_win_names contain the names of various windows that can be
#   created.  This names are used as a check when the window is created
#   to make sure the correct call has been used.
#
source $PCR_HOME/tcl/window/window_globals.tcl
source $PCR_HOME/tcl/window/window_globals_set.tcl
#
# Read windowing procedures
#
source $PCR_HOME/tcl/window/browse_file.tcl
source $PCR_HOME/tcl/window/browse_dir.tcl
source $PCR_HOME/tcl/window/open_window.tcl
source $PCR_HOME/tcl/window/wait_window.tcl
#
# Read bitmap globals
#
source $PCR_HOME/tcl/bitmaps/bitmaps_globals.tcl
source $PCR_HOME/tcl/bitmaps/bitmaps_globals_set.tcl
#
# Read debug settings procedures
#
source $PCR_HOME/tcl/debug/debug_globals.tcl
source $PCR_HOME/tcl/debug/debug_globals_set.tcl
#
# Setup traces on specific flag variable to update the gui
#
source $PCR_HOME/tcl/gui/gui_update.tcl
#
# Client Routine to open and close connections to server
#   and to start automatic updating.  The servers are msg,
#   info, tcs, hex and motor.
#
source $PCR_HOME/tcl/client/client_globals.tcl
source $PCR_HOME/tcl/client/client_globals_set.tcl
source $PCR_HOME/tcl/client/client_close.tcl
source $PCR_HOME/tcl/client/client_start.tcl
#
# PCR Info routines
#
source $PCR_HOME/tcl/info/info_globals.tcl
source $PCR_HOME/tcl/info/info_globals_set.tcl
source $PCR_HOME/tcl/info/info_close.tcl
source $PCR_HOME/tcl/info/info_connect.tcl
source $PCR_HOME/tcl/info/info_get.tcl
source $PCR_HOME/tcl/info/info_looper.tcl
source $PCR_HOME/tcl/info/info_reset_seeing.tcl
#
# Topbox Status routines
#
source $PCR_HOME/tcl/status/status_globals.tcl
source $PCR_HOME/tcl/status/status_globals_set.tcl
source $PCR_HOME/tcl/status/status_close.tcl
source $PCR_HOME/tcl/status/status_connect.tcl
source $PCR_HOME/tcl/status/status_get.tcl
source $PCR_HOME/tcl/status/status_looper.tcl
#
# TCS routines
#
source $PCR_HOME/tcl/tcs/tcs_globals.tcl
source $PCR_HOME/tcl/tcs/tcs_globals_set.tcl
source $PCR_HOME/tcl/tcs/tcs_close.tcl
source $PCR_HOME/tcl/tcs/tcs_connect.tcl
source $PCR_HOME/tcl/tcs/tcs_get.tcl
source $PCR_HOME/tcl/tcs/tcs_looper.tcl
source $PCR_HOME/tcl/tcs/tcs_move.tcl
source $PCR_HOME/tcl/tcs/tcs_set.tcl
#
# Hexapod routines
#
source $PCR_HOME/tcl/hex/hex_globals.tcl
source $PCR_HOME/tcl/hex/hex_globals_set.tcl
source $PCR_HOME/tcl/hex/hex_get.tcl
source $PCR_HOME/tcl/hex/hex_focus.tcl
#
# Main AO GUI window
#
source $PCR_HOME/tcl/main/main_globals.tcl
source $PCR_HOME/tcl/main/main_globals_set.tcl
source $PCR_HOME/tcl/main/main_set_save_dir.tcl
source $PCR_HOME/tcl/main/main_window.tcl
#
# Display page and routines
#
source $PCR_HOME/tcl/display/display_globals.tcl
source $PCR_HOME/tcl/display/display_globals_set.tcl
source $PCR_HOME/tcl/display/display_page.tcl
source $PCR_HOME/tcl/display/display_single.tcl
source $PCR_HOME/tcl/display/display_slopes.tcl
source $PCR_HOME/tcl/display/display_spots.tcl
source $PCR_HOME/tcl/display/display_wfsc_fits.tcl
#
# PGplot display routines
#
source $PCR_HOME/tcl/pgplot/pgplot_globals.tcl
source $PCR_HOME/tcl/pgplot/pgplot_globals_set.tcl
source $PCR_HOME/tcl/pgplot/pgplot_open.tcl
#
# DClient and routines
#
source $PCR_HOME/tcl/dclient/dclient_globals.tcl
source $PCR_HOME/tcl/dclient/dclient_globals_set.tcl
source $PCR_HOME/tcl/dclient/dclient_dm_save.tcl
source $PCR_HOME/tcl/dclient/dclient_wfsc_save.tcl
#
# Camera routines
#
source $PCR_HOME/tcl/camera/camera_globals.tcl
source $PCR_HOME/tcl/camera/camera_globals_set.tcl
source $PCR_HOME/tcl/camera/camera_current_bias.tcl
source $PCR_HOME/tcl/camera/camera_default_bias.tcl
source $PCR_HOME/tcl/camera/camera_page.tcl
source $PCR_HOME/tcl/camera/camera_bias_set.tcl
source $PCR_HOME/tcl/camera/camera_biasfile_read.tcl
source $PCR_HOME/tcl/camera/camera_biasfile_write.tcl
source $PCR_HOME/tcl/camera/camera_rate_set.tcl
source $PCR_HOME/tcl/camera/camera_start_stop.tcl
source $PCR_HOME/tcl/camera/camera_temperature_get.tcl
#
# *NOTE: Should get rid of MMTAO_DATA_DIR  24may04 DLM
#        Needed for background routines
global MMTAO_DATA_DIR
set MMTAO_DATA_DIR $env(MMTAO_DATA_DIR)
#
# Camera Background routines
#
source $PCR_HOME/tcl/bg/bg_globals.tcl
source $PCR_HOME/tcl/bg/bg_globals_set.tcl
source $PCR_HOME/tcl/bg/bg_acquire.tcl
source $PCR_HOME/tcl/bg/bg_dynamic_on_off.tcl
source $PCR_HOME/tcl/bg/bg_load.tcl
source $PCR_HOME/tcl/bg/bg_on_off.tcl
source $PCR_HOME/tcl/bg/bg_page.tcl
source $PCR_HOME/tcl/bg/bg_zero.tcl
#
# Server page
#
source $PCR_HOME/tcl/server/server_globals.tcl
source $PCR_HOME/tcl/server/server_globals_set.tcl
source $PCR_HOME/tcl/server/server_page.tcl
#
# Alignment page
#
source $PCR_HOME/tcl/alignment/alignment_globals.tcl
source $PCR_HOME/tcl/alignment/alignment_globals_set.tcl
source $PCR_HOME/tcl/alignment/alignment_page.tcl
#
# Static Slope Offset routines
#
source $PCR_HOME/tcl/static/static_globals.tcl
source $PCR_HOME/tcl/static/static_globals_set.tcl
source $PCR_HOME/tcl/static/static_apply.tcl
source $PCR_HOME/tcl/static/static_display.tcl
source $PCR_HOME/tcl/static/static_page.tcl
source $PCR_HOME/tcl/static/static_rms.tcl
#
# Slope Modulation routines
#
source $PCR_HOME/tcl/mod/mod_globals.tcl
source $PCR_HOME/tcl/mod/mod_globals_set.tcl
source $PCR_HOME/tcl/mod/mod_offset_send.tcl
source $PCR_HOME/tcl/mod/mod_page.tcl
source $PCR_HOME/tcl/mod/mod_rate_send.tcl
source $PCR_HOME/tcl/mod/mod_start_stop.tcl
#
# Seeing routines
#
source $PCR_HOME/tcl/seeing/seeing_globals.tcl
source $PCR_HOME/tcl/seeing/seeing_globals_set.tcl
source $PCR_HOME/tcl/seeing/seeing_get_average.tcl
source $PCR_HOME/tcl/seeing/seeing_looper.tcl
source $PCR_HOME/tcl/seeing/seeing_page.tcl
#
# Save Data routines
#
source $PCR_HOME/tcl/save/save_globals.tcl
source $PCR_HOME/tcl/save/save_globals_set.tcl
source $PCR_HOME/tcl/save/save_page.tcl
#
# Seeing dependent gain routines
#
source $PCR_HOME/tcl/gain/gain_globals.tcl
source $PCR_HOME/tcl/gain/gain_globals_set.tcl
source $PCR_HOME/tcl/gain/gain_page.tcl
source $PCR_HOME/tcl/gain/gain_set.tcl
#
# PCR Server routines
#
source $PCR_HOME/tcl/pcr/pcr_globals.tcl
source $PCR_HOME/tcl/pcr/pcr_globals_set.tcl
source $PCR_HOME/tcl/pcr/pcr_cam_bg_send.tcl
source $PCR_HOME/tcl/pcr/pcr_close.tcl
source $PCR_HOME/tcl/pcr/pcr_connect.tcl
source $PCR_HOME/tcl/pcr/pcr_dm_connect.tcl
source $PCR_HOME/tcl/pcr/pcr_dm_save.tcl
source $PCR_HOME/tcl/pcr/pcr_dm_stop.tcl
source $PCR_HOME/tcl/pcr/pcr_gain_up_down.tcl
source $PCR_HOME/tcl/pcr/pcr_request.tcl
source $PCR_HOME/tcl/pcr/pcr_save_data.tcl
source $PCR_HOME/tcl/pcr/pcr_server_connect.tcl
source $PCR_HOME/tcl/pcr/pcr_slope_offset_send.tcl
source $PCR_HOME/tcl/pcr/pcr_wfsc_connect.tcl
source $PCR_HOME/tcl/pcr/pcr_wfsc_reset.tcl
source $PCR_HOME/tcl/pcr/pcr_wfsc_stop.tcl
#
# Topbox Server routines
#
source $PCR_HOME/tcl/topbox/topbox_globals.tcl
source $PCR_HOME/tcl/topbox/topbox_globals_set.tcl
source $PCR_HOME/tcl/topbox/topbox_close.tcl
source $PCR_HOME/tcl/topbox/topbox_connect.tcl
source $PCR_HOME/tcl/topbox/topbox_connect_device.tcl
source $PCR_HOME/tcl/topbox/topbox_enable.tcl
source $PCR_HOME/tcl/topbox/topbox_home.tcl
source $PCR_HOME/tcl/topbox/topbox_move_abs.tcl
source $PCR_HOME/tcl/topbox/topbox_move_rel.tcl
source $PCR_HOME/tcl/topbox/topbox_page.tcl
source $PCR_HOME/tcl/topbox/topbox_power.tcl
source $PCR_HOME/tcl/topbox/topbox_request.tcl
source $PCR_HOME/tcl/topbox/topbox_update.tcl
#
# Loop on/off/gain routines 
#
source $PCR_HOME/tcl/loop/loop_globals.tcl
source $PCR_HOME/tcl/loop/loop_globals_set.tcl
source $PCR_HOME/tcl/loop/loop_data_on_off.tcl
source $PCR_HOME/tcl/loop/loop_data_to_dds.tcl
source $PCR_HOME/tcl/loop/loop_gain_up_down.tcl
source $PCR_HOME/tcl/loop/loop_open_close.tcl
source $PCR_HOME/tcl/loop/loop_set_gain.tcl
#
# AO Process (Server Processes) starting routines
#
source $PCR_HOME/tcl/process/process_globals.tcl
source $PCR_HOME/tcl/process/process_globals_set.tcl
source $PCR_HOME/tcl/process/process_looper.tcl
source $PCR_HOME/tcl/process/process_start.tcl
source $PCR_HOME/tcl/process/process_start_all.tcl
#
# FSM routines
#
source $PCR_HOME/tcl/fsm/fsm_globals.tcl
source $PCR_HOME/tcl/fsm/fsm_globals_set.tcl
source $PCR_HOME/tcl/fsm/fsm_move_camera.tcl
source $PCR_HOME/tcl/fsm/fsm_move_pupil.tcl
source $PCR_HOME/tcl/fsm/fsm_move_stella.tcl
source $PCR_HOME/tcl/fsm/fsm_move_wfsc.tcl
#
# Reconstructor routines
#
source $PCR_HOME/tcl/recon/recon_globals.tcl
source $PCR_HOME/tcl/recon/recon_globals_set.tcl
source $PCR_HOME/tcl/recon/recon_set.tcl
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Tix automatically set the default font and colors to the "Tix Scheme"
#   This command reset the scheme back to the default "TK Scheme"
#
tix resetoptions TK TK
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# Build Main AO Window
#
wm withdraw .
#
# First, try to open connection all servers and start automatic updating
#
#   The servers are:
#     1) TCS Server
#     2) PCR Server
#     3) PCR (the pcr and dds processes)
#     4) Topbox Server
#
client_start 1 1 1 1 .
#
# Create a data directory
#
main_set_save_dir .
#
# Create the main AO GUI window
#
open_window main_window $main_win
#
# Second, start the processes that where not connected but were request to
#   connect above by client_start and needed to run the AO system
#
#   The processes are:
#     1) PCR_Server
#     2) PCR and DDS
#     3) Topbox Server
#
process_start_all 1 1 1 $main_win
