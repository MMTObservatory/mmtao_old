#=================================================================
#
# main_window.tcl
#
#=================================================================
#
# Procedure to create the Main AO GUI window
#
# Created  9feb06   DLM  Started with the old main_window.tcl
#
#=================================================================

proc main_window { win_name } {

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    source $PCR_HOME/tcl/main/main_globals.tcl
#
   if { $win_name != $main_win } {
	puts "main_window routine should be called with the parameter \$main_win"
	return -1
    }

    toplevel $main_win
    wm title $main_win "MMT AO Main Gui"
    wm geometry $main_win $main_position
    wm resizable $main_win 0 0
#
# Make global variables available
#
    source $PCR_HOME/tcl/bitmaps/bitmaps_globals.tcl
    source $PCR_HOME/tcl/info/info_globals.tcl
    source $PCR_HOME/tcl/fsm/fsm_globals.tcl
    source $PCR_HOME/tcl/pgplot/pgplot_globals.tcl
#
    upvar #0 Status Status
    upvar #0 Info Info
#
#################################################################
#
#    Build Window
#    ------------
#
#  The window will have three main areas:
#    1) A menu bar at the top mainly for server routines and engineering window
#    2) A non-changing top sections which contains all buttons, display, plots
#       etc needed for closed loop operation.  This will occupy about half
#       the window.
#    3) A changable section which will have several ear tags for windows with
#       information, loop characterization and engineering
#
#################################################################
#
# Create frames for the three sections
#
    frame $main_win.menubar -relief raised -border 2
    frame $main_win.main -relief raised -border 2
#
#-----------------------------------------------------------------
#
# 1) Menu Bar
#
#-----------------------------------------------------------------
#
# Create menu labels
#
    menubutton $main_win.menubar.file -text "File"     \
	-menu $main_win.menubar.file.menu
    menubutton $main_win.menubar.help -text "Help"     \
	-menu $main_win.menubar.help.menu

    set fmenu $main_win.menubar.file.menu
    set hmenu $main_win.menubar.help.menu
#
# Create menu list
#
    menu $fmenu
    menu $hmenu
#
# Connect actions to each menu label
#
    $fmenu add command -label "Quit" \
	-command {
	    client_close $main_win
	    exit
	}

    $hmenu add command -label "You wish!" -command { }
#
# Pack all menus on the left with the exception of the "Help" menu
#
    pack $main_win.menubar.file -side left
    pack $main_win.menubar.help -side right
#
#-----------------------------------------------------------------
#
# 2) Main AO GUI controls
#
#-----------------------------------------------------------------
#
#
############################################
# Main AO controls
############################################
#
    frame $main_win.main.controls -relief ridge -border 4
#
    label $main_win.main.controls.loop_title -text "Loop Controls" -bg cyan    
#
# Open/Close loop
#
    button $main_win.main.controls.change_button \
        -state disabled -height 5 -bg yellow \
	-text "LOOP IS\nOPEN" \
	-command {
	    loop_open_close $main_win
	}
#
# Loop gain
#
    label $main_win.main.controls.gain_label -text "Loop Gain" -bg cyan 

    label $main_win.main.controls.gain_entry -textvariable Info(Loop_Gain) \
	-width 4 -anchor e -relief sunken -bg white -fg black

    button $main_win.main.controls.gain_up -bitmap @$BitMaps_Dir/up -state disabled \
	-command {
	    loop_gain_up_down up $main_win
	}
    button $main_win.main.controls.gain_down -bitmap @$BitMaps_Dir/down -state disabled \
	-command {
	    loop_gain_up_down down $main_win
	}
#
#
    button $main_win.main.controls.pid_off -text "pid off" -command { loop_set_pid_off $main_win }
    button $main_win.main.controls.pid_on  -text "pid on"  -command { loop_set_pid_on  $main_win }
#
    label $main_win.main.controls.pgain_label -text "Loop PGain" -bg cyan 

    entry $main_win.main.controls.pgain_entry -textvariable Main_P_Gain \
	-width 4 -justify right -relief sunken -bg white -fg black
    bind $main_win.main.controls.pgain_entry <Return> {loop_pgain_set $Main_P_Gain $main_win}

    button $main_win.main.controls.pgain_up -bitmap @$BitMaps_Dir/up \
	-state normal \
	-command {
	    loop_pgain_up_down up $main_win
	}
    button $main_win.main.controls.pgain_down -bitmap @$BitMaps_Dir/down \
	 -state normal \
	-command {
	    loop_pgain_up_down down $main_win
	}
#
    label $main_win.main.controls.igain_label -text "Loop IGain" -bg cyan 

    entry $main_win.main.controls.igain_entry -textvariable Main_I_Gain \
	-width 4 -justify right -relief sunken -bg white -fg black
    bind $main_win.main.controls.igain_entry <Return> {loop_igain_set $Main_I_Gain $main_win}

    button $main_win.main.controls.igain_up -bitmap @$BitMaps_Dir/up \
	 -state normal \
	-command {
	    loop_igain_up_down up $main_win
	}
    button $main_win.main.controls.igain_down -bitmap @$BitMaps_Dir/down \
	 -state normal \
	-command {
	    loop_igain_up_down down $main_win
	}
#
    label $main_win.main.controls.dgain_label -text "Loop DGain" -bg cyan 

    entry $main_win.main.controls.dgain_entry -textvariable Main_D_Gain \
	-width 4 -justify right -relief sunken -bg white -fg black
    bind $main_win.main.controls.dgain_entry <Return> {loop_dgain_set $Main_D_Gain $main_win}

    button $main_win.main.controls.dgain_up -bitmap @$BitMaps_Dir/up \
	 -state normal \
	-command {
	    loop_dgain_up_down up $main_win
	}
    button $main_win.main.controls.dgain_down -bitmap @$BitMaps_Dir/down \
	 -state normal \
	-command {
	    loop_dgain_up_down down $main_win
	}
#
#
    button $main_win.main.controls.tap_off -text "tap off" -command { loop_set_tap_gains_off $main_win }
    button $main_win.main.controls.tap_on  -text "tap on"  -command { loop_set_tap_gains_on  $main_win }
#
    label $main_win.main.controls.Delta_gain_label -text "Delta_gain" -bg cyan 

    entry $main_win.main.controls.Delta_gain_entry -textvariable Main_Delta_gain \
	-width 4 -justify right -relief sunken -bg white -fg black
    bind $main_win.main.controls.Delta_gain_entry <Return> {loop_Delta_gain_set $Main_Delta_gain $main_win}

    button $main_win.main.controls.Delta_gain_up -bitmap @$BitMaps_Dir/up \
	 -state normal \
	-command {
	    loop_Delta_gain_up_down up $main_win
	}
    button $main_win.main.controls.Delta_gain_down -bitmap @$BitMaps_Dir/down \
	 -state normal \
	-command {
	    loop_Delta_gain_up_down down $main_win
	}
#
    label $main_win.main.controls.Previous_gain_label -text "Previous_gain" -bg cyan 

    entry $main_win.main.controls.Previous_gain_entry -textvariable Main_Previous_gain \
	-width 4 -justify right -relief sunken -bg white -fg black
    bind $main_win.main.controls.Previous_gain_entry <Return> {loop_Previous_gain_set $Main_Previous_gain $main_win}

    button $main_win.main.controls.Previous_gain_up -bitmap @$BitMaps_Dir/up \
	-state normal \
	-command {
	    loop_Previous_gain_up_down up $main_win
	}
    button $main_win.main.controls.Previous_gain_down -bitmap @$BitMaps_Dir/down \
	 -state normal \
	-command {
	    loop_Previous_gain_up_down down $main_win
	}
#
# Camera Status
#
    label $main_win.main.controls.cam_status \
	-text "Camera is\nSTOPPED" -bg yellow -height 4 -width 15
#
# Put this panel together
#
    set i 0
    grid config $main_win.main.controls.loop_title \
	-row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid config $main_win.main.controls.change_button \
	-row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid config $main_win.main.controls.gain_label \
	-row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid config $main_win.main.controls.gain_entry \
	-row $i -rowspan 2 -column 0 -sticky e
    grid config $main_win.main.controls.gain_up \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.gain_down \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.pid_off \
	-row $i -column 0 -sticky w
    grid config $main_win.main.controls.pid_on \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.pgain_label \
	-row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid config $main_win.main.controls.pgain_entry \
	-row $i -rowspan 2 -column 0 -sticky e
    grid config $main_win.main.controls.pgain_up \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.pgain_down \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.igain_label \
	-row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid config $main_win.main.controls.igain_entry \
	-row $i -rowspan 2 -column 0 -sticky e
    grid config $main_win.main.controls.igain_up \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.igain_down \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.dgain_label \
	-row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid config $main_win.main.controls.dgain_entry \
	-row $i -rowspan 2 -column 0 -sticky e
    grid config $main_win.main.controls.dgain_up \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.dgain_down \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.tap_off \
	-row $i -column 0 -sticky w
    grid config $main_win.main.controls.tap_on \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.Delta_gain_label \
	-row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid config $main_win.main.controls.Delta_gain_entry \
	-row $i -rowspan 2 -column 0 -sticky e
    grid config $main_win.main.controls.Delta_gain_up \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.Delta_gain_down \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.Previous_gain_label \
	-row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid config $main_win.main.controls.Previous_gain_entry \
	-row $i -rowspan 2 -column 0 -sticky e
    grid config $main_win.main.controls.Previous_gain_up \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.pgain_down \
	-row $i -column 1 -sticky w
    incr i
    grid config $main_win.main.controls.cam_status \
	-row $i -column 0 -columnspan 2 -sticky ew
    incr i
#
############################################
# Position and Filter Wheel Information
############################################
#
#-------------------------------------------
# Position Information
#-------------------------------------------
#
    frame $main_win.main.info
#
    frame $main_win.main.info.pos -relief ridge -border 4
    frame $main_win.main.info.pos.fsm
    frame $main_win.main.info.pos.tcs
    frame $main_win.main.info.pos.rot
#
    label $main_win.main.info.pos.pos_title -text "Positioning" -bg cyan
#
#    FSM
#
    frame $main_win.main.info.pos.fsm.line -relief ridge -height 1m -border 3 -bg black
    label $main_win.main.info.pos.fsm.label -text "FSM"
#
    label $main_win.main.info.pos.fsm.tip_label -text "Tip: "
    label $main_win.main.info.pos.fsm.tip_value -width 6 -anchor e -relief sunken \
	-textvariable Status(FSM_Tip_Position) -bg white -fg black
#
    label $main_win.main.info.pos.fsm.tilt_label -text "Tilt: "
    label $main_win.main.info.pos.fsm.tilt_value -width 6 -anchor e -relief sunken \
	-textvariable Status(FSM_Tilt_Position) -bg white -fg black
#
#    Telescope
#
    frame $main_win.main.info.pos.tcs.line -relief ridge -height 1m -border 3 -bg black
    label $main_win.main.info.pos.tcs.label -text "TCS Offsets"
#
    label $main_win.main.info.pos.tcs.ra_label -text "RA: "
    label $main_win.main.info.pos.tcs.ra_value -width 6 -anchor e -relief sunken \
	-textvariable TCS(ao_off_ra) -bg white -fg black
#
    label $main_win.main.info.pos.tcs.dec_label -text "Dec: "
    label $main_win.main.info.pos.tcs.dec_value -width 6 -anchor e -relief sunken \
	-textvariable TCS(ao_off_dec) -bg white -fg black
#
#    Rotator
#
    frame $main_win.main.info.pos.rot.line -relief ridge -height 1m -border 3 -bg black
    label $main_win.main.info.pos.rot.label -text "Rotator"
#
    label $main_win.main.info.pos.rot.wfsc_label -text "WFSC: "
    label $main_win.main.info.pos.rot.wfsc_value -width 6 -anchor e -relief sunken \
	-textvariable Status(Rotation_Position) -bg white -fg black
#
    label $main_win.main.info.pos.rot.tcs_label -text "Rotator: "
    label $main_win.main.info.pos.rot.tcs_value -width 6 -anchor e -relief sunken \
	-textvariable TCS(rot) -bg white -fg black
    label $main_win.main.info.pos.rot.tracking -text "Not Tracking" -bg green
#
#-----------------------------------------------------------------
#
# Put the position panel together
#
#-----------------------------------------------------------------
#
#    FSM
#
    set i 0
    grid config $main_win.main.info.pos.fsm.line \
	-row $i -column 0 -columnspan 2 -sticky ew
    grid config $main_win.main.info.pos.fsm.label \
	-row $i -column 0 -columnspan 2 -sticky w
    incr i
    grid config $main_win.main.info.pos.fsm.tip_label \
	-row $i -column 0 -sticky ew
    grid config $main_win.main.info.pos.fsm.tip_value \
	-row $i -column 1 -sticky ew
    incr i
    grid config $main_win.main.info.pos.fsm.tilt_label \
	-row $i -column 0 -sticky ew
    grid config $main_win.main.info.pos.fsm.tilt_value \
	-row $i -column 1 -sticky ew
    incr i
#
#    Telescope offsets
#
    set i 0
    grid config $main_win.main.info.pos.tcs.line \
	-row $i -column 0 -columnspan 2 -sticky ew
    grid config $main_win.main.info.pos.tcs.label \
	-row $i -column 0 -columnspan 2 -sticky w
    incr i
    grid config $main_win.main.info.pos.tcs.ra_label \
	-row $i -column 0 -sticky ew
    grid config $main_win.main.info.pos.tcs.ra_value \
	-row $i -column 1 -sticky ew
    incr i
    grid config $main_win.main.info.pos.tcs.dec_label \
	-row $i -column 0 -sticky ew
    grid config $main_win.main.info.pos.tcs.dec_value \
	-row $i -column 1 -sticky ew
    incr i
#
#    Rotator
#
    set i 0
    grid config $main_win.main.info.pos.rot.line \
	-row $i -column 0 -columnspan 2 -sticky ew
    grid config $main_win.main.info.pos.rot.label \
	-row $i -column 0 -columnspan 2 -sticky w
    incr i
    grid config $main_win.main.info.pos.rot.wfsc_label \
	-row $i -column 0 -sticky ew
    grid config $main_win.main.info.pos.rot.wfsc_value \
	-row $i -column 1 -sticky ew
    incr i
    grid config $main_win.main.info.pos.rot.tcs_label \
	-row $i -column 0 -sticky ew
    grid config $main_win.main.info.pos.rot.tcs_value \
	-row $i -column 1 -sticky ew
    incr i
    grid config $main_win.main.info.pos.rot.tracking \
	-row $i -column 0 -columnspan 2 -sticky ew
    incr i
#
#    Entire panel
#
    pack $main_win.main.info.pos.pos_title \
	$main_win.main.info.pos.fsm \
	$main_win.main.info.pos.tcs \
	$main_win.main.info.pos.rot -fill x -side top
#
#-------------------------------------------
# Filter Wheel Information
#-------------------------------------------
#
    frame $main_win.main.info.fwheel -relief ridge -border 4
#
    label $main_win.main.info.fwheel.title -text "Filter Wheel" -bg cyan
    label $main_win.main.info.fwheel.label -text "Unknown"
#
    pack $main_win.main.info.fwheel.title \
	$main_win.main.info.fwheel.label \
	-fill x -side top
#
#-------------------------------------------
# Pack the information stuff
#-------------------------------------------
#
    pack $main_win.main.info.pos \
	$main_win.main.info.fwheel \
	-fill x -side top
#
############################################
# Build FSM move arrows
############################################
#
    frame $main_win.main.fsmview -relief ridge -border 4
    frame $main_win.main.fsmview.move
    frame $main_win.main.fsmview.incr

    label $main_win.main.fsmview.title0 -text "FSM Motion" -pady 4 -bg cyan

    radiobutton $main_win.main.fsmview.wfsc -text "WFSC" -var FSM_Camera -value WFSC -anchor e
    radiobutton $main_win.main.fsmview.stella -text "Stella" -var FSM_Camera -value Stella -anchor e
    radiobutton $main_win.main.fsmview.pupil -text "Pupil" -var FSM_Camera -value Pupil -anchor e

    button $main_win.main.fsmview.move.y_pos -bitmap @$BitMaps_Dir/up \
	-command {
	    fsm_move_camera up $main_win
	}
    button $main_win.main.fsmview.move.y_neg -bitmap @$BitMaps_Dir/down \
	-command {
	    fsm_move_camera down $main_win
	}
    button $main_win.main.fsmview.move.x_pos -bitmap @$BitMaps_Dir/right \
	-command {
	    fsm_move_camera right $main_win
	}
    button $main_win.main.fsmview.move.x_neg -bitmap @$BitMaps_Dir/left \
	-command {
	    fsm_move_camera left $main_win
	}

    radiobutton $main_win.main.fsmview.incr_03 -text "0.3 Incr" -var FSM_Incr -value 0.3 -anchor w \
	-command {
	    set FSM_Incr 0.3
	}

    radiobutton $main_win.main.fsmview.incr_1 -text "1.0 Incr" -var FSM_Incr -value 1.0 -anchor w \
	-command {
	    set FSM_Incr 1.0
	}

    radiobutton $main_win.main.fsmview.incr_5 -text "5.0 incr" -var FSM_Incr -value 5.0 -anchor w \
	-command {
	    set FSM_Incr 5.0
	}

    label $main_win.main.fsmview.incr.incr_label \
	-text "Incr (\")"

    entry $main_win.main.fsmview.incr.incr_entry          \
	-textvariable FSM_Incr              \
	-width 5 -justify right -bg white

    # fsmview
    set i 0
    grid config $main_win.main.fsmview.move.y_pos        \
	-row $i -column 1 -sticky nsew
    incr i
    grid config $main_win.main.fsmview.move.x_neg        \
	-row $i -column 0 -sticky nsew
    grid config $main_win.main.fsmview.move.x_pos        \
	-row $i -column 2 -sticky nsew
    incr i
    grid config $main_win.main.fsmview.move.y_neg        \
	-row $i -column 1 -sticky nsew

    set i 0
    grid config $main_win.main.fsmview.incr.incr_label        \
	-row $i -column 0 -sticky ew
    grid config $main_win.main.fsmview.incr.incr_entry        \
	-row $i -column 1
    incr i

    set i 0
    grid config $main_win.main.fsmview.title0            \
	-row $i -column 0 -sticky nsew -columnspan 3
    incr i
    grid config $main_win.main.fsmview.wfsc  \
	-row $i -column 0 -sticky w
    incr i
    grid config $main_win.main.fsmview.stella  \
	-row $i -column 0 -sticky w
    incr i
    grid config $main_win.main.fsmview.pupil  \
	-row $i -column 0 -sticky w
    incr i
    grid config $main_win.main.fsmview.move              \
	-row $i -column 0 -sticky nsew
    incr i
    grid config $main_win.main.fsmview.incr       \
	-row $i -column 0 -sticky ew
    incr i
    grid config $main_win.main.fsmview.incr_03        \
	-row $i -column 0
    incr i
    grid config $main_win.main.fsmview.incr_1        \
	-row $i -column 0
    incr i
    grid config $main_win.main.fsmview.incr_5        \
	-row $i -column 0
#
############################################
# Create area for the Spots display
############################################
#
    frame $main_win.main.spots -relief ridge -border 4
    frame $main_win.main.spots.controls
#
#==============
# Spots Immage
#==============
#
# Create frame for pgplot
#
    pgplot $main_win.main.spots.image -share true \
	-mincolors 25 -maxcolors 128 -width 400 -height 360 \
	-bg black -fg white
#
# Build Spots diplay area
#
    pgplot_open Spots $main_win.main.spots.image/xtk $main_win
#
#==============
# Controls
#==============
#
# Update Display
#
    label $main_win.main.spots.controls.display_title -text "Display Spots" -bg cyan \
	-width 17
    label $main_win.main.spots.controls.running -textvar Spot_Symbol -width 5
    button $main_win.main.spots.controls.single -text "Single" -width 8\
	-command {
	    set Display_RT(Spots) 0
	    display_spots Spots 1 0 $main_win
	}
    checkbutton $main_win.main.spots.controls.realtime -var Display_RT(Spots) \
	-text "Realtime" \
	-command {
	    display_spots Spots 1 1 $main_win
	}
#
    label $main_win.main.spots.controls.type_title -text "Spots Type" -bg cyan \
	-width 15
    radiobutton $main_win.main.spots.controls.current -var Spots_Type \
	-value 0 -text "Current"
    radiobutton $main_win.main.spots.controls.average -var Spots_Type \
	-value 1 -text "Average"
    radiobutton $main_win.main.spots.controls.pupil -var Spots_Type \
	-value 2 -text "Pupil"
#
    label $main_win.main.spots.controls.arrow_title -text "Slope Arrows" -bg cyan \
	-width 18
    radiobutton $main_win.main.spots.controls.none -var Spots_Slopes \
	-value 0 -text "None"
    radiobutton $main_win.main.spots.controls.slopes -var Spots_Slopes \
	-value 1 -text "All Slopes"
    radiobutton $main_win.main.spots.controls.focus_slopes -var Spots_Slopes \
	-value 2 -text "Focus Slopes"
    checkbutton $main_win.main.spots.controls.tip_tilt -var Spots_Arrow \
	-text "Average Tip/Tilt"
#
# Pack the controls
#
    set i 0
    set j 0
    grid $main_win.main.spots.controls.display_title \
	-row $j -column $i
    incr j
    grid $main_win.main.spots.controls.running \
	-row $j -column $i
    incr j
    grid $main_win.main.spots.controls.single \
	-row $j -column $i
    incr j
    grid $main_win.main.spots.controls.realtime \
	-row $j -column $i
    incr i

    set j 0
    grid $main_win.main.spots.controls.type_title \
	-row $j -column $i
    incr j
    grid $main_win.main.spots.controls.current \
	-row $j -column $i -sticky w
    incr j
    grid $main_win.main.spots.controls.average \
	-row $j -column $i -sticky w
    incr j
    grid $main_win.main.spots.controls.pupil \
	-row $j -column $i -sticky w
    incr i

    set j 0
    grid $main_win.main.spots.controls.arrow_title \
	-row $j -column $i
    incr j
    grid $main_win.main.spots.controls.none \
	-row $j -column $i -sticky w
    incr j
    grid $main_win.main.spots.controls.slopes \
	-row $j -column $i -sticky w
    incr j
#    grid $main_win.main.spots.controls.focus_slopes \
#	-row $j -column $i -sticky w
#    incr j
    grid $main_win.main.spots.controls.tip_tilt \
	-row $j -column $i -sticky w
    incr j
#
#==============
# Pack it all
#==============
#
    pack $main_win.main.spots.image \
	$main_win.main.spots.controls -side top
#
#
############################################
# Build Seeing and Intensity plot areas
############################################
#
#    frame $main_win.main.spots -relief ridge -border 4
#
# Seeing plot
#
#    frame $main_win.main.spots.seeing -relief raised -border 2 -width 230 -height 150
#
#   Create frame for pgplot
#
#    pgplot $main_win.main.spots.seeing.plot -maxcolors 4 -width 180 -height 120
#
#   Build seeing graph using pgplot
#
#    PGplot_Cmd Seeing Open $main_win.main.spots.seeing.plot/xtk
#
#   Create frame for pgplot
#
#    bind $main_win.main.spots.seeing.plot <Configure> {PGplot_Cmd Seeing TestDraw 0.0 1.0}
#
# Intensity plot
#
#    frame $main_win.main.spots.intensity -relief raised -border 2 -width 230 -height 150
#
#   Create frame for pgplot
#
#    pgplot $main_win.main.spots.intensity.plot -maxcolors 4 -width 180 -height 120
#
#   Open a pgplot device in the given frame
#
#    PGplot_Cmd Intensity Open $main_win.main.spots.intensity.plot/xtk
#
#   Build seeing graph using pgplot
#
#    bind $main_win.main.spots.intensity.plot <Configure> {PGplot_Cmd Intensity TestDraw 0.0 1.0}
#
# Puts this spots together
#
#    pack $main_win.main.spots.seeing.plot -side top -expand true -fill both
#    pack $main_win.main.spots.intensity.plot -side top -expand true -fill both
#    pack $main_win.main.spots.seeing $main_win.main.spots.intensity -side top
#
    pack $main_win.main.spots $main_win.main.info -side right
    pack $main_win.main.spots $main_win.main.fsmview -side right
    pack $main_win.main.controls -anchor w -expand true -fill both
#
#-----------------------------------------------------------------
#
# 2) Folder tab area
#
#-----------------------------------------------------------------
#
    tixNoteBook $main_win.aux -ipadx 5 -ipady 5 -height 330 -width 800
#
    $main_win.aux add server -createcmd { server_page $main_win.aux server } \
	-label "Servers\n& Power" -under 0
    $main_win.aux add topbox -createcmd { topbox_page $main_win.aux topbox } \
	-label "Topbox\nStatus" -under 0
    $main_win.aux add camera -createcmd { camera_page $main_win.aux camera } \
	-label "Camera\nSettings" -under 0
    $main_win.aux add alignment -createcmd { alignment_page $main_win.aux alignment } \
	-label "Alignment\n& Setup" -under 0
    $main_win.aux add display -createcmd { display_page $main_win.aux display } \
	-label "Slope\nDisplay" -under 0
    $main_win.aux add bg -createcmd { bg_page $main_win.aux bg } \
	-label "Camera\nBackground" -under 0
    $main_win.aux add static -createcmd { static_page $main_win.aux static } \
	-label "Static\nOffsets" -under 0
    $main_win.aux add mod -createcmd { mod_page $main_win.aux mod } \
	-label "Slope & Mod\nOffsets" -under 0
    $main_win.aux add seeing -createcmd { seeing_page $main_win.aux seeing } \
	-label "Seeing" -under 0
    $main_win.aux add save -createcmd { save_page $main_win.aux save } \
	-label "Save\nData" -under 0
    $main_win.aux add science -createcmd { science_page $main_win.aux science } \
	-label "Science" -under 0
    $main_win.aux add tss -createcmd { tss_page $main_win.aux tss } \
	-label "TSS" -under 0
#
#-----------------------------------------------------------------
#
# Pack the three section
#
#-----------------------------------------------------------------
#
    pack $main_win.menubar  -side top -fill x
    pack $main_win.main -side top -fill both
    pack $main_win.aux -side top -expand yes

}
