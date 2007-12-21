#=================================================================
# rotator_window.tcl
#=================================================================
#
# Test window
#
# Created   8jan04   DLM  Started with alignment_window.
#
#=================================================================

proc rotator_window { rot_win } {

#
#-----------------------------------------------------------------
# Load Global Settings
#-----------------------------------------------------------------
#
    global AO_GUI_HOME
    source $AO_GUI_HOME/tcl/rotator/rotator_globals.tcl
    source $AO_GUI_HOME/tcl/tcs/tcs_globals.tcl
    source $AO_GUI_HOME/tcl/motor/motor_globals.tcl

    global reply_data
#
    toplevel $rot_win
    wm title $rot_win "Rotator Information"
    wm geometry $rot_win +500+500
#
    upvar #0 Motor Motor
#
#=================================================================
# Build Window
#=================================================================
#
#==================================
# Menu Bar
#==================================
#
    frame $rot_win.menubar -relief raised -bd 2
#
    menubutton $rot_win.menubar.file -text "File" \
	-menu $rot_win.menubar.file.menu
#
    pack $rot_win.menubar.file -side left
#
# Create menus
#
    menu $rot_win.menubar.file.menu
#
# Connect actions to each menu label
#
    $rot_win.menubar.file.menu add command -label "Quit" \
	-command { 
	    exit
	}

    pack $rot_win.menubar -side top -fill x
#
#################################################################
    # --------------------- Rotator --------------------- #
#################################################################

    frame $rot_win.rot              -border 0

    label $rot_win.rot.title \
        -text "Rotator" -pady 4 -bg lightskyblue

    set i 0
    grid $rot_win.rot.title \
	-row $i -column 0 -columnspan 2 \
	-sticky ew
    incr i
#
# Status lights
#
    checkbutton $rot_win.rot.connected \
	-variable TCS_Connected -state disabled -disabledforeground green
    label $rot_win.rot.connected_label -text "Connected"
    checkbutton $rot_win.rot.error \
	-variable TCS_Error -state disabled -disabledforeground red
    label $rot_win.rot.error_label -text "Error"
#
    grid $rot_win.rot.connected \
	-row $i -column 0 -sticky e
    grid $rot_win.rot.connected_label \
	-row $i -column 1 -sticky w
    incr i
    grid $rot_win.rot.error \
	-row $i -column 0 -sticky e
    grid $rot_win.rot.error_label \
	-row $i -column 1 -sticky w
    incr i
#
# Display Rotator information
#
    label $rot_win.rot.name_rot -relief sunken \
	-text "Rotator Angle" -width 20 -relief raised -bg lightgreen
    label $rot_win.rot.label_rot -relief sunken \
	-textvariable TCS(rot) -width 22

    label $rot_win.rot.name_pa -relief sunken \
	-text "Parallactic Angle" -width 20 -relief raised -bg lightgreen
    label $rot_win.rot.label_pa -relief sunken \
	-textvariable TCS(pa) -width 22
    
    label $rot_win.rot.name_rotpa -relief sunken \
	-text "Position Angle" -width 20 -relief raised -bg lightgreen
    label $rot_win.rot.label_rotpa -relief sunken \
	-textvariable TCS(rotpa) -width 22
    
    label $rot_win.rot.name_wfsc -relief sunken \
	-text "WFSC Angle" -width 20 -relief raised -bg lightgreen
    label $rot_win.rot.label_wfsc -relief sunken \
	-textvariable Motor(Rotation_Position) -width 22
    
    label $rot_win.rot.name_0 -relief sunken \
	-text "Astig 0" -width 20 -relief raised -bg lightgreen
    label $rot_win.rot.label_0 -relief sunken \
	-textvariable new_astig_0_value -width 22
    
    label $rot_win.rot.name_45 -relief sunken \
	-text "Astig 45" -width 20 -relief raised -bg lightgreen
    label $rot_win.rot.label_45 -relief sunken \
	-textvariable new_astig_45_value -width 22
    
    grid $rot_win.rot.name_rot \
	-row $i -column 0 -sticky ew
    grid $rot_win.rot.label_rot \
	-row $i -column 1
    incr i
    
    grid $rot_win.rot.name_wfsc \
	-row $i -column 0 -sticky ew
    grid $rot_win.rot.label_wfsc \
	-row $i -column 1
    incr i
    incr i
    
    grid $rot_win.rot.name_pa \
	-row $i -column 0 -sticky ew
    grid $rot_win.rot.label_pa \
	-row $i -column 1
    incr i
    
    grid $rot_win.rot.name_rotpa \
	-row $i -column 0 -sticky ew
    grid $rot_win.rot.label_rotpa \
	-row $i -column 1
    incr i

    grid $rot_win.rot.name_0 \
	-row $i -column 0 -sticky ew
    grid $rot_win.rot.label_0 \
	-row $i -column 1
    incr i
    grid $rot_win.rot.name_45 \
	-row $i -column 0 -sticky ew
    grid $rot_win.rot.label_45 \
	-row $i -column 1
    incr i
#
# Tracking controls
#    
    checkbutton $rot_win.rot.tracking -text "Tracking"\
	-variable Rotator_Tracking
#
    grid $rot_win.rot.tracking \
	-row $i -column 0 -sticky e
    incr i
#
#-----------------------------------------------------------------
#
# Pack this section
#
    pack $rot_win.rot
#
#=================================================================
#
# Start updating Rotator values
# 
    set Rotator_Update 1
    rotator_looper $rot_win
}
