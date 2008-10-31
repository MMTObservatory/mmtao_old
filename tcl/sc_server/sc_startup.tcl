#++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# gui_startup.tcl
#
# This script reads all of the TclTk files needed to run
#   the MMTAO Science Camera Server gui.
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
#++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# The tcsh shell environment variable contains the version of the
#   current gui.  This version is made global so all proc's know
#   where *_globals_set.tcl files live
#
global env
global PCR_HOME
set PCR_HOME $env(PCR_HOME)
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
# Read windowing procedures
#
source $PCR_HOME/tcl/debug/debug_globals.tcl
source $PCR_HOME/tcl/debug/debug_globals_set.tcl
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
# Info routines
#
source $PCR_HOME/tcl/info/info_globals.tcl
source $PCR_HOME/tcl/info/info_globals_set.tcl
source $PCR_HOME/tcl/info/info_close.tcl
source $PCR_HOME/tcl/info/info_connect.tcl
source $PCR_HOME/tcl/info/info_get.tcl
source $PCR_HOME/tcl/info/info_looper.tcl
source $PCR_HOME/tcl/info/info_reset_seeing.tcl
#
# Status routines
#
source $PCR_HOME/tcl/status/status_globals.tcl
source $PCR_HOME/tcl/status/status_globals_set.tcl
source $PCR_HOME/tcl/status/status_close.tcl
source $PCR_HOME/tcl/status/status_connect.tcl
source $PCR_HOME/tcl/status/status_get.tcl
source $PCR_HOME/tcl/status/status_looper.tcl
#
# Topbox Server routines
#
source $PCR_HOME/tcl/topbox/topbox_globals.tcl
source $PCR_HOME/tcl/topbox/topbox_globals_set.tcl
source $PCR_HOME/tcl/topbox/topbox_close.tcl
source $PCR_HOME/tcl/topbox/topbox_connect.tcl
source $PCR_HOME/tcl/topbox/topbox_move_abs.tcl
source $PCR_HOME/tcl/topbox/topbox_move_rel.tcl
source $PCR_HOME/tcl/topbox/topbox_request.tcl
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
source $PCR_HOME/tcl/hex/hex_close.tcl
source $PCR_HOME/tcl/hex/hex_connect.tcl
source $PCR_HOME/tcl/hex/hex_get.tcl
source $PCR_HOME/tcl/hex/hex_looper.tcl
source $PCR_HOME/tcl/hex/hex_set.tcl
#
# Science Camera Server window and routines
#
source $PCR_HOME/tcl/sc_server/sc_globals.tcl
source $PCR_HOME/tcl/sc_server/sc_globals_set.tcl
source $PCR_HOME/tcl/sc_server/sc_astig.tcl
source $PCR_HOME/tcl/sc_server/sc_focus.tcl
source $PCR_HOME/tcl/sc_server/sc_go_home.tcl
source $PCR_HOME/tcl/sc_server/sc_log.tcl
source $PCR_HOME/tcl/sc_server/sc_log_open.tcl
source $PCR_HOME/tcl/sc_server/sc_looper.tcl
source $PCR_HOME/tcl/sc_server/sc_offset_open.tcl
source $PCR_HOME/tcl/sc_server/sc_offset_closed.tcl
source $PCR_HOME/tcl/sc_server/sc_processing.tcl
source $PCR_HOME/tcl/sc_server/sc_server.tcl
source $PCR_HOME/tcl/sc_server/sc_set_home.tcl
source $PCR_HOME/tcl/sc_server/sc_update.tcl
source $PCR_HOME/tcl/sc_server/sc_window.tcl
#
# PCR routines
#
source $PCR_HOME/tcl/pcr/pcr_globals.tcl
source $PCR_HOME/tcl/pcr/pcr_globals_set.tcl
source $PCR_HOME/tcl/pcr/pcr_close.tcl
source $PCR_HOME/tcl/pcr/pcr_connect.tcl
source $PCR_HOME/tcl/pcr/pcr_gain_up_down.tcl
source $PCR_HOME/tcl/pcr/pcr_request.tcl
source $PCR_HOME/tcl/pcr/pcr_server_connect.tcl
source $PCR_HOME/tcl/pcr/pcr_slope_offset_send.tcl
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
# FSM routines
#
source $PCR_HOME/tcl/fsm/fsm_globals.tcl
source $PCR_HOME/tcl/fsm/fsm_globals_set.tcl
source $PCR_HOME/tcl/fsm/fsm_move_camera.tcl
source $PCR_HOME/tcl/fsm/fsm_move_pupil.tcl
source $PCR_HOME/tcl/fsm/fsm_move_stella.tcl
source $PCR_HOME/tcl/fsm/fsm_move_wfsc.tcl
source $PCR_HOME/tcl/fsm/fsm_move_ra_dec.tcl
#
# Static Slope Offset routines
#
source $PCR_HOME/tcl/static/static_globals.tcl
source $PCR_HOME/tcl/static/static_globals_set.tcl
source $PCR_HOME/tcl/static/static_apply.tcl
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
# Open connection all servers and start automatic updating
#   The servers are:
#     1) TCS Server
#     2) PCR Server
#     3) PCR (the pcr and dds processes)
#     4) Topbox Server
#
client_start 1 1 0 1 0 .
#
# Open log file
#
sc_log_open jnk.log
#
# Listen to port for requests
#
SC_Server  "SC_Server" 127.0.0.1 7700
#
# Create the main AO GUI window
#
open_window sc_window $sc_win
#
#
#
#sc_looper $sc_win
