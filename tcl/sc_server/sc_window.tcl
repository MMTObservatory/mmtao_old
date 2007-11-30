#========================================================
# Science Camera Server
#========================================================
#
# TclTK procedure to create a window for the Science Camera server,
#   and to start this server.
#
# Created 29apr03  DLM
#
#========================================================

proc sc_window { win_name } {

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl

#
# Check that the correct window name was sent
#
    if { $win_name != $sc_win } {
	puts "sc_window routine should be called with the parameter \$sc_win"
	return -1
    }

    toplevel $sc_win
    wm title $sc_win "Science Camera Server"
    wm geometry $sc_win $sc_position
#
# Initialize global variables
#
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl
#
    upvar #0 TCS TCS
    upvar #0 Info Info
#
#################################################################
#
# Create Menu Bar
#
#################################################################
#
# Create frames the menu bar
#
    frame $sc_win.menubar -relief raised -border 2
    frame $sc_win.main -relief raised -border 2
#
# Create menu labels
#
    menubutton $sc_win.menubar.file -text "File"     \
	-menu $sc_win.menubar.file.menu

    set fmenu $sc_win.menubar.file.menu
#
# Create menu list
#
    menu $fmenu
#
# Connect actions to each menu label
#
    $fmenu add command -label "Quit" \
	-command {
	    client_close $sc_win
	    exit
	}

#
# Pack all menus on the left with the exception of the "Help" menu
#
    pack $sc_win.menubar.file -side left
#
#################################################################
#
# Build Window
#
#################################################################
#
#-----------------------------------------------------------------
# Request
#-----------------------------------------------------------------
#
    frame $sc_win.request -relief ridge -border 4
#
    set i 0
    label $sc_win.request.title -text " Science Camera Server" -bg lightgoldenrod
    pack $sc_win.request.title
    incr i

    label $sc_win.request.connect -text "Not Connected"
    pack $sc_win.request.connect
    incr i
#
    for {set num 0} {$num < 4} {set num [expr $num + 1]} {
	set SC_Request($num) --
	label $sc_win.request.request_$num -textvar SC_Request($num)
	pack $sc_win.request.request_$num
    }

    button $sc_win.request.clear -text "Clear Request" \
	-command {
	    set SC_Arguements 0
	    for {set i 0} {$i < $SC_Parameters} { incr i } {
		set SC_Request($i) --
	    }
	    set SC_Parameters 0
	}
    pack $sc_win.request.clear
#
#-----------------------------------------------------------------
# Accept requests
#-----------------------------------------------------------------
#
    frame $sc_win.accept -relief ridge -border 4
#
    checkbutton $sc_win.accept.button -bg yellow -activebackground lightyellow \
	-var SC_Accept -text "Not Accepting Requests" -width 23
#
    pack $sc_win.accept.button -fill x
#
#-----------------------------------------------------------------
# Offset in RA/DEC
#-----------------------------------------------------------------
#
    frame $sc_win.move -relief ridge -border 4
#
    label $sc_win.move.label -text "Offset Object" -bg lightgoldenrod

    label $sc_win.move.ra_label \
        -text "Ra"
    entry $sc_win.move.ra_entry \
        -textvariable SC_Ra_Offset \
        -width 7 -justify right -bg white

    label $sc_win.move.dec_label \
        -text "Dec"
    entry $sc_win.move.dec_entry \
        -textvariable SC_Dec_Offset \
        -width 7 -justify right -bg white

    button $sc_win.move.negate -text "Negate" \
	-command {
	    set SC_Ra_Offset [ expr -1.0 * $SC_Ra_Offset ]
	    set SC_Dec_Offset [ expr -1.0 * $SC_Dec_Offset ]
	}
    button $sc_win.move.clear -text "Clear" \
	-command {
	    set SC_Ra_Offset 0.0
	    set SC_Dec_Offset 0.0
	}
    button $sc_win.move.move -text "Move" \
	-command {
	    if { $Info(Loop_Running) } {
		set status [ sc_offset_closed 0 $SC_Ra_Offset $SC_Dec_Offset $sc_win ]
	    } else {
		set status [ sc_offset_open 0 $SC_Ra_Offset $SC_Dec_Offset $sc_win ]
	    }
	    if { $status } {
		SC_Log "sc_window: Error making manual offset"
	    }
        }
#
    set i 0
    grid config $sc_win.move.label \
        -row $i -column 0 -columnspan 4 -sticky ew
    incr i
    grid config $sc_win.move.negate \
        -row $i -column 0
    grid config $sc_win.move.ra_label \
        -row $i -column 1
    grid config $sc_win.move.ra_entry \
        -row $i -column 2 -sticky w
    grid config $sc_win.move.move \
        -row $i -rowspan 2 -column 3
    incr i
    grid config $sc_win.move.clear \
        -row $i -column 0
    grid config $sc_win.move.dec_label \
        -row $i -column 1
    grid config $sc_win.move.dec_entry \
        -row $i -column 2 -sticky w
    incr i
#
#-----------------------------------------------------------------
# Information
#-----------------------------------------------------------------
#
    frame $sc_win.info -relief ridge -border 4
#
    label $sc_win.info.running -text "Loop Open" \
	-bg yellow -activebackground lightyellow
    label $sc_win.info.gain_label -text "Loop Gain"
    label $sc_win.info.gain_entry -textvar Info(Loop_Gain) -relief ridge -border 2 \
	-width 4
#
    set i 0
    grid config $sc_win.info.running \
        -row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid config $sc_win.info.gain_label \
        -row $i -column 0
    grid config $sc_win.info.gain_entry \
        -row $i -column 1 -sticky w
    incr i
#
    frame $sc_win.off -relief ridge -border 4
#
    label $sc_win.off.title -text "Telescope" -bg lightgoldenrod
    label $sc_win.off.pa_label -text "PA"
    label $sc_win.off.pa_entry -textvar TCS(pa) -relief ridge -border 2  \
	-width 6
    label $sc_win.off.ra_label -text "RA Offset"
    label $sc_win.off.ra_entry -textvar SC_RA_Off -relief ridge -border 2  \
	-width 6
    label $sc_win.off.dec_label -text "Dec Offset"
    label $sc_win.off.dec_entry -textvar SC_DEC_Off -relief ridge -border 2  \
	-width 6
    label $sc_win.off.ra_home -text "RA Home"
    label $sc_win.off.ra_home_value -textvar SC_RA_Home -relief ridge -border 2  \
	-width 6
    label $sc_win.off.dec_home -text "Dec Home"
    label $sc_win.off.dec_home_value -textvar SC_DEC_Home -relief ridge -border 2  \
	-width 6
#
    grid config $sc_win.off.title \
        -row $i -column 0 -columnspan 2
    incr i
    grid config $sc_win.off.pa_label \
        -row $i -column 0
    grid config $sc_win.off.pa_entry \
        -row $i -column 1
    incr i
    grid config $sc_win.off.ra_label \
        -row $i -column 0
    grid config $sc_win.off.ra_entry \
        -row $i -column 1
    incr i
    grid config $sc_win.off.dec_label \
        -row $i -column 0
    grid config $sc_win.off.dec_entry \
        -row $i -column 1
    incr i
    grid config $sc_win.off.ra_home \
        -row $i -column 0
    grid config $sc_win.off.ra_home_value \
        -row $i -column 1
    incr i
    grid config $sc_win.off.dec_home \
        -row $i -column 0
    grid config $sc_win.off.dec_home_value \
        -row $i -column 1
    incr i

#
    frame $sc_win.fsm -relief ridge -border 4
#
    label $sc_win.fsm.title -text "Field Steering Mirror" -bg lightgoldenrod
    label $sc_win.fsm.tip_label -text "Tip Offset"
    label $sc_win.fsm.tip_entry -textvar SC_Tip_Off -relief ridge -border 2  \
	-width 6
    label $sc_win.fsm.tilt_label -text "Tilt Offset"
    label $sc_win.fsm.tilt_entry -textvar SC_Tilt_Off -relief ridge -border 2  \
	-width 6
    label $sc_win.fsm.tip_home -text "Tip Home"
    label $sc_win.fsm.tip_home_value -textvar SC_Tip_Home -relief ridge -border 2  \
	-width 6
    label $sc_win.fsm.tilt_home -text "Tilt_Home"
    label $sc_win.fsm.tilt_home_value -textvar SC_Tilt_Home -relief ridge -border 2  \
	-width 6

#
    grid config $sc_win.fsm.title \
        -row $i -column 0 -columnspan 2
    incr i
    grid config $sc_win.fsm.tip_label \
        -row $i -column 0
    grid config $sc_win.fsm.tip_entry \
        -row $i -column 1
    incr i
    grid config $sc_win.fsm.tilt_label \
        -row $i -column 0
    grid config $sc_win.fsm.tilt_entry \
        -row $i -column 1
    incr i
    grid config $sc_win.fsm.tip_home \
        -row $i -column 0
    grid config $sc_win.fsm.tip_home_value \
        -row $i -column 1
    incr i
    grid config $sc_win.fsm.tilt_home \
        -row $i -column 0
    grid config $sc_win.fsm.tilt_home_value \
        -row $i -column 1
    incr i
#
#-----------------------------------------------------------------
# Put the window together
#-----------------------------------------------------------------
#
    button $sc_win.set_home -text "Set Home" \
	-command {
	    sc_set_home 0 $sc_win
	}

    button $sc_win.go_home -text "Go Home" \
	-command {
	    sc_go_home 0 $sc_win
	}
#
#-----------------------------------------------------------------
# Static Offsets
#-----------------------------------------------------------------
#
    frame $sc_win.static -relief ridge -border 4
#
    label $sc_win.static.title -text "Static Aberrations" -bg lightgoldenrod
    label $sc_win.static.as_0_label -text "Astigmatism 0"
    label $sc_win.static.as_0_entry -textvar Info(Static_Astig_0) -relief ridge -border 2  \
	-width 6
    label $sc_win.static.as_45_label -text "Astigmatism 45"
    label $sc_win.static.as_45_entry -textvar Info(Static_Astig_45) -relief ridge -border 2  \
	-width 6
#
    grid config $sc_win.static.title \
        -row $i -column 0 -columnspan 2
    incr i
    grid config $sc_win.static.as_0_label \
        -row $i -column 0
    grid config $sc_win.static.as_0_entry \
        -row $i -column 1
    incr i
    grid config $sc_win.static.as_45_label \
        -row $i -column 0
    grid config $sc_win.static.as_45_entry \
        -row $i -column 1
    incr i
#
#
#-----------------------------------------------------------------
# Focus
#-----------------------------------------------------------------
#
    frame $sc_win.focus -relief ridge -border 4
#
    label $sc_win.focus.title -text "Focus" -bg lightgoldenrod
    label $sc_win.focus.wfsc_label -text "Focus"
    label $sc_win.focus.wfsc_entry -textvar SC_Focus -relief ridge -border 2  \
	-width 6
#
    grid config $sc_win.focus.title \
        -row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid config $sc_win.focus.wfsc_label \
        -row $i -column 0 -sticky ew
    grid config $sc_win.focus.wfsc_entry \
        -row $i -column 1 -sticky ew
    incr i
#
#-----------------------------------------------------------------
# Testing of delays
#-----------------------------------------------------------------
#
    frame $sc_win.delay -relief ridge -border 4
#
    label $sc_win.delay.label -text "Set Delays" -bg lightgoldenrod

    label $sc_win.delay.gain_down -text "Gain to 0.0"
    entry $sc_win.delay.delay_1 \
        -textvariable SC_Delay_1 \
        -width 7 -justify right -bg white
    label $sc_win.delay.ms -text "miliseconds"

    label $sc_win.delay.fsm -text "Move FSM"
    entry $sc_win.delay.delay_2 \
        -textvariable SC_Delay_2 \
        -width 7 -justify right -bg white

    label $sc_win.delay.tcs -text "Move TCS"
    entry $sc_win.delay.delay_3 \
        -textvariable SC_Delay_3 \
        -width 7 -justify right -bg white

    label $sc_win.delay.gain_05 -text "Gain to 0.05"
    entry $sc_win.delay.delay_4 \
        -textvariable SC_Delay_4 \
        -width 7 -justify right -bg white

    label $sc_win.delay.gain_up -text "Gain to original"
    entry $sc_win.delay.delay_5 \
        -textvariable SC_Delay_5 \
        -width 7 -justify right -bg white

    label $sc_win.delay.done -text "Done sent to Clio"

    set i 0
    grid config $sc_win.delay.gain_down \
        -row $i -column 0 -sticky ew
    incr i
    grid config $sc_win.delay.delay_1 \
        -row $i -column 0 -sticky ew
    grid config $sc_win.delay.ms \
        -row $i -column 1 -sticky ew
    incr i
    grid config $sc_win.delay.fsm \
        -row $i -column 0 -sticky ew
    incr i
    grid config $sc_win.delay.delay_2 \
        -row $i -column 0 -sticky ew
    incr i
    grid config $sc_win.delay.tcs \
        -row $i -column 0 -sticky ew
    incr i
    grid config $sc_win.delay.delay_3 \
        -row $i -column 0 -sticky ew
    incr i
    grid config $sc_win.delay.gain_05 \
        -row $i -column 0 -sticky ew
    incr i
    grid config $sc_win.delay.delay_4 \
        -row $i -column 0 -sticky ew
    incr i
    grid config $sc_win.delay.gain_up \
        -row $i -column 0 -sticky ew
    incr i
    grid config $sc_win.delay.delay_5 \
        -row $i -column 0 -sticky ew
    incr i
    grid config $sc_win.delay.done \
        -row $i -column 0 -sticky ew
    incr i

#-----------------------------------------------------------------
#
# Pack the window
#
#-----------------------------------------------------------------
#
    set i 0
    grid $sc_win.menubar -row $i -column 0 -columnspan 3 -sticky ew
    incr i
    grid $sc_win.request -row $i -column 0 -columnspan 2 -sticky ew
    grid $sc_win.delay -row $i -column 2 -rowspan 3 -sticky ew
    incr i
    grid $sc_win.accept -row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid $sc_win.move -row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid $sc_win.info -row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid $sc_win.off -row $i -column 0 -sticky ew
    grid $sc_win.fsm -row $i -column 1 -sticky ew
    incr i
    grid $sc_win.set_home -row $i -column 0 -sticky ew
    grid $sc_win.go_home -row $i -column 1 -sticky ew
    incr i
    grid $sc_win.static -row $i -column 0 -columnspan 2 -sticky ew
    incr i
    grid $sc_win.focus -row $i -column 0 -columnspan 2 -sticky ew
#    pack $sc_win.menubar -side top -fill x
#    pack $sc_win.request $sc_win.accept $sc_win.move $sc_win.info \
#	 -side top -fill both -expand 1
#   pack $sc_win.off -side left 
#    pack $sc_win.fsm -side top -fill both -expand 1
#    pack $sc_win.set_home $sc_win.go_home -side left -anchor e
#    pack $sc_win.right -side right -anchor e
#
# Default setup values
#
    set SC_Accept 1
}
