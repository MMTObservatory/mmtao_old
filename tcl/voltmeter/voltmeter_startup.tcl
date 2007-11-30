#++++++++++++++++++++++++++++++++++++++++++++++++++++
#
# voltmeter_startup.tcl
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
global AO_GUI_HOME
set AO_GUI_HOME $env(AO_GUI_HOME)
#
# global_win_names contain the names of various windows that can be
#   created.  This names are used as a check when the window is created
#   to make sure the correct call has been used.
#
source $AO_GUI_HOME/tcl/window/window_globals.tcl
source $AO_GUI_HOME/tcl/window/window_globals_set.tcl
#
# global_addresses contain the IP addresses and port of required
#   servers.  This includes the reconstructor computer (VME), TCS,
#   Hexapod, topbox, etc
#
source $AO_GUI_HOME/tcl/addresses_globals.tcl
source $AO_GUI_HOME/tcl/addresses_globals_set.tcl
#
# Read windowing procedures
#
source $AO_GUI_HOME/tcl/window/browse_file.tcl
source $AO_GUI_HOME/tcl/window/browse_dir.tcl
source $AO_GUI_HOME/tcl/window/open_window.tcl
source $AO_GUI_HOME/tcl/window/wait_window.tcl
#
# Read bitmap globals
#
source $AO_GUI_HOME/tcl/bitmaps/bitmaps_globals.tcl
source $AO_GUI_HOME/tcl/bitmaps/bitmaps_globals_set.tcl
#
# Read windowing procedures
#
source $AO_GUI_HOME/tcl/debug/debug_globals.tcl
source $AO_GUI_HOME/tcl/debug/debug_globals_set.tcl
#
# FSM routines
#
source $AO_GUI_HOME/tcl/voltmeter/voltmeter_globals.tcl
source $AO_GUI_HOME/tcl/voltmeter/voltmeter_globals_set.tcl
source $AO_GUI_HOME/tcl/voltmeter/voltmeter_close.tcl
source $AO_GUI_HOME/tcl/voltmeter/voltmeter_connect.tcl
source $AO_GUI_HOME/tcl/voltmeter/voltmeter_get.tcl
source $AO_GUI_HOME/tcl/voltmeter/voltmeter_init.tcl
source $AO_GUI_HOME/tcl/voltmeter/voltmeter_looper.tcl
source $AO_GUI_HOME/tcl/voltmeter/voltmeter_measure.tcl
source $AO_GUI_HOME/tcl/voltmeter/voltmeter_monitor.tcl
source $AO_GUI_HOME/tcl/voltmeter/voltmeter_set.tcl
source $AO_GUI_HOME/tcl/voltmeter/voltmeter_window.tcl
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
wm withdraw .
#
# Create the main Voltmeter GUI window
#
open_window voltmeter_window $volt_win
