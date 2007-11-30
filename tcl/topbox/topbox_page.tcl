################################################## 
#
# topbox_page.tcl
#
####
#
# Create window to display the status and interact with the Topbox
#
# Written  17sep07  DLM Started with seeing_page.tcl
#
##################################################

proc topbox_page { topbox_win page } {

    global PCR_HOME
#
    source $PCR_HOME/tcl/window/window_globals.tcl
    source $PCR_HOME/tcl/bitmaps/bitmaps_globals.tcl
#
    upvar #0 Info Info
    upvar #0 Status Status
    upvar #0 Topbox_ND_Name Topbox_ND_Name
    upvar #0 Topbox_ND_Value Topbox_ND_Value
#
    global reply_data
#
# Create the tix NoteBook frame
#
    global topbox_page
    set topbox_page [ $topbox_win subwidget $page ]
#
# Build the page
#
    frame $topbox_page.left
    frame $topbox_page.center
    frame $topbox_page.right
#
##################################
# Field Steering Mirror (FSM)
##################################
#
#==================================
# FSM frames
#==================================
#
    frame $topbox_page.left.fsm -relief ridge -border 4
#
    frame $topbox_page.left.fsm.info
    frame $topbox_page.left.fsm.info.arrows -border 4
#
#==================================
# FSM Label
#==================================
#
    label $topbox_page.left.fsm.info.title \
        -text "Field Steering\nMirror" -bg lightskyblue
#
# FSM move arrows
#
    button $topbox_page.left.fsm.info.arrows.y_pos \
        -bitmap @$BitMaps_Dir/up\
	-command {
	    topbox_move_rel "FSM_Tip" $Topbox_FSM_Incr $topbox_page
	}
    button $topbox_page.left.fsm.info.arrows.y_neg \
        -bitmap @$BitMaps_Dir/down \
	-command {
	    topbox_move_rel "FSM_Tip" [expr -1 * $Topbox_FSM_Incr ] $topbox_page
	}
    button $topbox_page.left.fsm.info.arrows.x_pos \
        -bitmap @$BitMaps_Dir/right \
	-command {
	    topbox_move_rel "FSM_Tilt" $Topbox_FSM_Incr  $topbox_page
	}
    button $topbox_page.left.fsm.info.arrows.x_neg \
        -bitmap @$BitMaps_Dir/left \
	-command {
	    topbox_move_rel "FSM_Tilt" [expr -1 * $Topbox_FSM_Incr ]  $topbox_page
	}

    grid config $topbox_page.left.fsm.info.arrows.y_pos \
        -row 0 -column 1
    grid config $topbox_page.left.fsm.info.arrows.x_neg \
        -row 1 -column 0
    grid config $topbox_page.left.fsm.info.arrows.x_pos \
        -row 1 -column 2
    grid config $topbox_page.left.fsm.info.arrows.y_neg \
        -row 2 -column 1
#
# FSM motion increment for arrows
#
    label $topbox_page.left.fsm.info.incr_label \
        -text "Increment\nin arcsec"
    entry $topbox_page.left.fsm.info.incr_entry \
        -textvariable Topbox_FSM_Incr \
        -width 7 -justify right -bg white
#
# FSM column labels
#
    label $topbox_page.left.fsm.info.position_label \
	-text "Absolute\nPosition" -bg cyan
    label $topbox_page.left.fsm.info.connected_label \
	-text "Connected" -bg cyan
    label $topbox_page.left.fsm.info.homed_label \
	-text "Homed" -bg cyan
    label $topbox_page.left.fsm.info.moving_label \
	-text "Moving"
    label $topbox_page.left.fsm.info.negative_label \
	-text "Negative\nLimit"
    label $topbox_page.left.fsm.info.positive_label \
	-text "Positive\nLimit"
    label $topbox_page.left.fsm.info.error_label \
	-text "Error"
#
# FSM axis title
#
    label $topbox_page.left.fsm.info.tip_label  \
        -text "Tip:"
    label $topbox_page.left.fsm.info.tilt_label \
        -text "Tilt:"
#
# FSM current position display
#
    label  $topbox_page.left.fsm.info.tip_value  \
        -textvariable Status(FSM_Tip_Position) \
        -width 9 -justify right -bg grey84 -relief sunken
    label  $topbox_page.left.fsm.info.tilt_value \
        -textvariable Status(FSM_Tilt_Position) \
        -width 9 -justify right -bg grey84 -relief sunken
#
# FSM connected light
#
    button $topbox_page.left.fsm.info.tip_connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_connect_device "Tip" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error connecting to FSM:Tip\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
    button $topbox_page.left.fsm.info.tilt_connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_connect_device "Tilt" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error connecting to FSM:Tilt\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# FSM homed light
#
    button $topbox_page.left.fsm.info.tip_homed \
	-state disabled -bg grey84 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_home "FSM_Tip" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error homing FSM:Tip\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
    button $topbox_page.left.fsm.info.tilt_homed \
	-state disabled -bg grey84 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_home "FSM_Tilt" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error homing FSM:Tilt\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# FSM moving flag
#
    checkbutton $topbox_page.left.fsm.info.tip_moving \
	-variable Status(FSM_Tip_Moving) -state disabled -disabledforeground green
    checkbutton $topbox_page.left.fsm.info.tilt_moving \
	-variable Status(FSM_Tilt_Moving) -state disabled -disabledforeground green
#
# FSM negative limit flag
#
    checkbutton $topbox_page.left.fsm.info.tip_negative \
	-variable Status(FSM_Tip_Negative) -state disabled -disabledforeground red
    checkbutton $topbox_page.left.fsm.info.tilt_negative \
	-variable Status(FSM_Tilt_Negative) -state disabled -disabledforeground red
#
# FSM positive limit flag
#
    checkbutton $topbox_page.left.fsm.info.tip_positive \
	-variable Status(FSM_Tip_Positive) -state disabled -disabledforeground red
    checkbutton $topbox_page.left.fsm.info.tilt_positive \
	-variable Status(FSM_Tilt_Positive) -state disabled -disabledforeground red
#
# FSM error flag
#
    checkbutton $topbox_page.left.fsm.info.tip_error \
	-variable Status(FSM_Tip_Error) -state disabled -disabledforeground red
    checkbutton $topbox_page.left.fsm.info.tilt_error \
	-variable Status(FSM_Tilt_Error) -state disabled -disabledforeground red
#
# Pack FSM Label
#
    set i 0
    set j 0
    grid config $topbox_page.left.fsm.info.title \
        -row $i -column $j -columnspan 2 -sticky ewns
#
# Pack FSM arrows into main panel
#
    set i 1
    grid config $topbox_page.left.fsm.info.arrows \
        -row $i -rowspan 3 -column $j -sticky ew
#
# Pack FSM column labels
#
    set i 0
    set j 2
    grid config $topbox_page.left.fsm.info.position_label \
        -row $i -column $j -sticky ewns
    incr j
    grid config $topbox_page.left.fsm.info.connected_label \
        -row $i -column $j -sticky ewns
    incr j
    grid config $topbox_page.left.fsm.info.homed_label \
        -row $i -column $j -sticky ewns
#    incr j
#    grid config $topbox_page.left.fsm.info.moving_label \
#        -row $i -column $j -sticky ew
#   incr j
#    grid config $topbox_page.left.fsm.info.negative_label \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.left.fsm.info.positive_label \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.left.fsm.info.error_label \
#        -row $i -column $j -sticky ew
#
# Pack FSM Tip
#
    incr i
    set j 1
    grid config $topbox_page.left.fsm.info.tip_label \
        -row $i -column $j -sticky e
    incr j
    grid config $topbox_page.left.fsm.info.tip_value \
        -row $i -column $j -sticky w
    incr j
    grid config $topbox_page.left.fsm.info.tip_connected \
        -row $i -column $j
    incr j
    grid config $topbox_page.left.fsm.info.tip_homed \
        -row $i -column $j
#    incr j
#    grid config $topbox_page.left.fsm.info.tip_moving \
#        -row $i -column $j
#    incr j
#    grid config $topbox_page.left.fsm.info.tip_negative \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.left.fsm.info.tip_positive \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.left.fsm.info.tip_error \
#        -row $i -column $j -sticky ew
#
# Pack FSM Tilt
#
    incr i
    set j 1
    grid config $topbox_page.left.fsm.info.tilt_label \
        -row $i -column $j -sticky e
    incr j
    grid config $topbox_page.left.fsm.info.tilt_value \
        -row $i -column $j -sticky w
    incr j
    grid config $topbox_page.left.fsm.info.tilt_connected \
        -row $i -column $j
    incr j
    grid config $topbox_page.left.fsm.info.tilt_homed \
        -row $i -column $j
#    incr j
#    grid config $topbox_page.left.fsm.info.tilt_moving \
#        -row $i -column $j
#    incr j
#    grid config $topbox_page.left.fsm.info.tilt_negative \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.left.fsm.info.tilt_positive \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.left.fsm.info.tilt_error \
#        -row $i -column $j -sticky ew
#
# Pack FSM arrow increments
#
    incr i
    set j 2
    grid config $topbox_page.left.fsm.info.incr_entry \
        -row $i -column $j -sticky e
    incr j
    grid config $topbox_page.left.fsm.info.incr_label \
        -row $i -column $j -columnspan 2 -sticky w
#
# Pack the FSM parts
#
    pack $topbox_page.left.fsm.info -side top
#
##################################
# Off Axis Parabola (OAP)
##################################
#
#==================================
# OAP frames
#==================================
#
    frame $topbox_page.left.oap -relief ridge -border 4
#
    frame $topbox_page.left.oap.info
    frame $topbox_page.left.oap.info.arrows -border 4
#
#==================================
# OAP Label
#==================================
#
    label $topbox_page.left.oap.info.title \
        -text "Off-Axis\nParabola" -bg lightskyblue
#
# OAP move arrows
#
    button $topbox_page.left.oap.info.arrows.y_pos \
        -bitmap @$BitMaps_Dir/up \
	-command {
	    topbox_move_rel "OAP_Tip" $Topbox_OAP_Incr $topbox_page
	}
    button $topbox_page.left.oap.info.arrows.y_neg \
        -bitmap @$BitMaps_Dir/down \
	-command {
	    topbox_move_rel "OAP_Tip" [expr -1 * $Topbox_OAP_Incr ] $topbox_page
	}
    button $topbox_page.left.oap.info.arrows.x_pos \
        -bitmap @$BitMaps_Dir/right \
	-command {
	    topbox_move_rel "OAP_Tilt" $Topbox_OAP_Incr $topbox_page
	}
    button $topbox_page.left.oap.info.arrows.x_neg \
        -bitmap @$BitMaps_Dir/left \
	-command {
	    topbox_move_rel "OAP_Tilt" [expr -1 * $Topbox_OAP_Incr ]  $topbox_page
	}

    grid config $topbox_page.left.oap.info.arrows.y_pos \
        -row 0 -column 1
    grid config $topbox_page.left.oap.info.arrows.x_neg \
        -row 1 -column 0
    grid config $topbox_page.left.oap.info.arrows.x_pos \
        -row 1 -column 2
    grid config $topbox_page.left.oap.info.arrows.y_neg \
        -row 2 -column 1
#
# OAP motion increment for arrows
#
    label $topbox_page.left.oap.info.incr_label \
        -text "Increment\nin arcsec"
    entry $topbox_page.left.oap.info.incr_entry \
        -textvariable Topbox_OAP_Incr \
        -width 7 -justify right -bg white
#
# OAP column labels
#
    label $topbox_page.left.oap.info.position_label \
	-text "Absolute\nPosition" -bg cyan
    label $topbox_page.left.oap.info.connected_label \
	-text "Connected" -bg cyan
    label $topbox_page.left.oap.info.homed_label \
	-text "Homed" -bg cyan
    label $topbox_page.left.oap.info.moving_label \
	-text "Moving"
    label $topbox_page.left.oap.info.negative_label \
	-text "Negative\nLimit"
    label $topbox_page.left.oap.info.positive_label \
	-text "Positive\nLimit"
    label $topbox_page.left.oap.info.error_label \
	-text "Error"
#
# OAP axis title
#
    label $topbox_page.left.oap.info.tip_label  \
        -text "Tip:"
    label $topbox_page.left.oap.info.tilt_label \
        -text "Tilt:"
#
# OAP current position display
#
    label  $topbox_page.left.oap.info.tip_value  \
        -textvariable Status(OAP_Tip_Position) \
        -width 9 -justify right -bg grey84 -relief sunken
    label  $topbox_page.left.oap.info.tilt_value \
        -textvariable Status(OAP_Tilt_Position) \
        -width 9 -justify right -bg grey84 -relief sunken
#
# OAP connected light
#
    button $topbox_page.left.oap.info.tip_connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_connect_device "Tip" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error connecting to OAP:Tip\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
    button $topbox_page.left.oap.info.tilt_connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_connect_device "Tilt" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error connecting to OAP:Tilt\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# OAP homed light
#
    button $topbox_page.left.oap.info.tip_homed \
	-state disabled -bg grey84 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_home "OAP_Tip" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error homing OAP:Tip\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
    button $topbox_page.left.oap.info.tilt_homed \
	-state disabled -bg grey84 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_home "OAP_Tilt" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error homing OAP:Tilt\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# OAP moving flag
#
    checkbutton $topbox_page.left.oap.info.tip_moving \
	-variable Status(OAP_Tip_Moving) -state disabled -disabledforeground green
    checkbutton $topbox_page.left.oap.info.tilt_moving \
	-variable Status(OAP_Tilt_Moving) -state disabled -disabledforeground green
#
# OAP negative limit flag
#
    checkbutton $topbox_page.left.oap.info.tip_negative \
	-variable Status(OAP_Tip_Negative) -state disabled -disabledforeground red
    checkbutton $topbox_page.left.oap.info.tilt_negative \
	-variable Status(OAP_Tilt_Negative) -state disabled -disabledforeground red
#
# OAP positive limit flag
#
    checkbutton $topbox_page.left.oap.info.tip_positive \
	-variable Status(OAP_Tip_Positive) -state disabled -disabledforeground red
    checkbutton $topbox_page.left.oap.info.tilt_positive \
	-variable Status(OAP_Tilt_Positive) -state disabled -disabledforeground red
#
# OAP error flag
#
    checkbutton $topbox_page.left.oap.info.tip_error \
	-variable Status(OAP_Tip_Error) -state disabled -disabledforeground red
    checkbutton $topbox_page.left.oap.info.tilt_error \
	-variable Status(OAP_Tilt_Error) -state disabled -disabledforeground red
#
# Pack OAP Label
#
    set i 0
    set j 0
    grid config $topbox_page.left.oap.info.title \
        -row $i -column $j -columnspan 2 -sticky ewns
#
# Pack OAP arrows into main panel
#
    set i 1
    grid config $topbox_page.left.oap.info.arrows \
        -row $i -rowspan 3 -column $j -sticky ew
#
# Pack OAP column labels
#
    set i 0
    set j 2
    grid config $topbox_page.left.oap.info.position_label \
        -row $i -column $j -sticky ewns
    incr j
    grid config $topbox_page.left.oap.info.connected_label \
        -row $i -column $j -sticky ewns
    incr j
    grid config $topbox_page.left.oap.info.homed_label \
        -row $i -column $j -sticky ewns
#    incr j
#    grid config $topbox_page.left.oap.info.moving_label \
#        -row $i -column $j -sticky ew
#   incr j
#    grid config $topbox_page.left.oap.info.negative_label \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.left.oap.info.positive_label \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.left.oap.info.error_label \
#        -row $i -column $j -sticky ew
#
# Pack OAP Tip
#
    incr i
    set j 1
    grid config $topbox_page.left.oap.info.tip_label \
        -row $i -column $j -sticky e
    incr j
    grid config $topbox_page.left.oap.info.tip_value \
        -row $i -column $j -sticky w
    incr j
    grid config $topbox_page.left.oap.info.tip_connected \
        -row $i -column $j
    incr j
    grid config $topbox_page.left.oap.info.tip_homed \
        -row $i -column $j
#    incr j
#    grid config $topbox_page.left.oap.info.tip_moving \
#        -row $i -column $j
#    incr j
#    grid config $topbox_page.left.oap.info.tip_negative \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.left.oap.info.tip_positive \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.left.oap.info.tip_error \
#        -row $i -column $j -sticky ew
#
# Pack OAP Tilt
#
    incr i
    set j 1
    grid config $topbox_page.left.oap.info.tilt_label \
        -row $i -column $j -sticky e
    incr j
    grid config $topbox_page.left.oap.info.tilt_value \
        -row $i -column $j -sticky w
    incr j
    grid config $topbox_page.left.oap.info.tilt_connected \
        -row $i -column $j
    incr j
    grid config $topbox_page.left.oap.info.tilt_homed \
        -row $i -column $j
#    incr j
#    grid config $topbox_page.left.oap.info.tilt_moving \
#        -row $i -column $j
#    incr j
#    grid config $topbox_page.left.oap.info.tilt_negative \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.left.oap.info.tilt_positive \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.left.oap.info.tilt_error \
#        -row $i -column $j -sticky ew
#
# Pack OAP arrow increments
#
    incr i
    set j 2
    grid config $topbox_page.left.oap.info.incr_entry \
        -row $i -column $j -sticky e
    incr j
    grid config $topbox_page.left.oap.info.incr_label \
        -row $i -column $j -columnspan 2 -sticky w
#
# Pack the OAP parts
#
    pack $topbox_page.left.oap.info -side top
#
##################################
# F15 Stage
##################################
#
#==================================
# F15 frames
#==================================
#
    frame $topbox_page.center.f15 -relief ridge -border 4
#
    frame $topbox_page.center.f15.info
    frame $topbox_page.center.f15.info.arrows -border 4
#
#==================================
# F15 Label
#==================================
#
    label $topbox_page.center.f15.info.title \
        -text "F15\nStage" -bg lightskyblue -width 9
#
# F15 move arrows
#
    button $topbox_page.center.f15.info.arrows.x_pos \
        -bitmap @$BitMaps_Dir/right \
	-command {
	    topbox_move_rel "F15" $Topbox_F15_Incr  $topbox_page
	}
    button $topbox_page.center.f15.info.arrows.x_neg \
        -bitmap @$BitMaps_Dir/left \
	-command {
	    topbox_move_rel "F15" [expr -1 * $Topbox_F15_Incr ]  $topbox_page
	}
    label $topbox_page.center.f15.info.arrows.space \
	-text " "

    grid config $topbox_page.center.f15.info.arrows.x_neg \
        -row 0 -column 0
    grid config $topbox_page.center.f15.info.arrows.space \
        -row 0 -column 1
    grid config $topbox_page.center.f15.info.arrows.x_pos \
        -row 0 -column 2
#
# F15 motion increment for arrows
#
    label $topbox_page.center.f15.info.incr_label \
        -text "Increment\nin mm"
    entry $topbox_page.center.f15.info.incr_entry \
        -textvariable Topbox_F15_Incr \
        -width 7 -justify right -bg white
#
# F15 column labels
#
    label $topbox_page.center.f15.info.position_label \
	-text "Absolute\nPosition" -bg cyan
    label $topbox_page.center.f15.info.connected_label \
	-text "Connected" -bg cyan
    label $topbox_page.center.f15.info.homed_label \
	-text "Homed" -bg cyan
    label $topbox_page.center.f15.info.moving_label \
	-text "Moving"
    label $topbox_page.center.f15.info.negative_label \
	-text "Negative\nLimit"
    label $topbox_page.center.f15.info.positive_label \
	-text "Positive\nLimit"
    label $topbox_page.center.f15.info.error_label \
	-text "Error"
#
# F15 current position display
#
    label  $topbox_page.center.f15.info.value  \
        -textvariable Status(F15_Position) \
        -width 9 -justify right -bg grey84 -relief sunken
#
# F15 connected light
#
    button $topbox_page.center.f15.info.connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_connect_device "F15" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error connecting to F15 Stage\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# F15 homed light
#
    button $topbox_page.center.f15.info.homed \
	-state disabled -bg grey84 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_home "F15" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error homing F15 Stage\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# F15 moving flag
#
    checkbutton $topbox_page.center.f15.info.moving \
	-variable Status(F15_Moving) -state disabled -disabledforeground green
#
# F15 negative limit flag
#
    checkbutton $topbox_page.center.f15.info.negative \
	-variable Status(F15_Negative) -state disabled -disabledforeground red
#
# F15 positive limit flag
#
    checkbutton $topbox_page.center.f15.info.positive \
	-variable Status(F15_Positive) -state disabled -disabledforeground red
#
# F15 error flag
#
    checkbutton $topbox_page.center.f15.info.error \
	-variable Status(F15_Error) -state disabled -disabledforeground red
#
# F15 position buttons
#
    button $topbox_page.center.f15.info.enable -text "Enable" -width 6 \
	-command {
	    topbox_enable "F15" $topbox_page
	}
    button $topbox_page.center.f15.info.stow -text "Stow" \
	-command {
	    topbox_move_abs "F15" $F15_Stow  $topbox_page
	}
    button $topbox_page.center.f15.info.nd -text "ND" \
	-command {
	    topbox_move_abs "F15" $F15_ND  $topbox_page
	}
    button $topbox_page.center.f15.info.laser -text "Laser" \
	-command {
	    topbox_move_abs "F15" $F15_Laser  $topbox_page
	}
#
# Pack F15 Label
#
    set i 0
    set j 0
    grid config $topbox_page.center.f15.info.title \
        -row $i -column $j -sticky ewns
#
# Pack F15 arrows into main panel
#
    set i 1
    grid config $topbox_page.center.f15.info.arrows \
        -row $i -column $j -sticky ew
#
# Pack F15 column labels
#
    set i 0
    set j 1
    grid config $topbox_page.center.f15.info.position_label \
        -row $i -column $j -sticky ewns
    incr j
    grid config $topbox_page.center.f15.info.connected_label \
        -row $i -column $j -sticky ewns
    incr j
    grid config $topbox_page.center.f15.info.homed_label \
        -row $i -column $j -sticky ewns
#    incr j
#    grid config $topbox_page.center.f15.info.moving_label \
#        -row $i -column $j -sticky ew
#   incr j
#    grid config $topbox_page.center.f15.info.negative_label \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.center.f15.info.positive_label \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.center.f15.info.error_label \
#        -row $i -column $j -sticky ew
#
# Pack F15 lights
#
    incr i
    set j 1
    grid config $topbox_page.center.f15.info.value \
        -row $i -column $j -sticky w
    incr j
    grid config $topbox_page.center.f15.info.connected \
        -row $i -column $j
    incr j
    grid config $topbox_page.center.f15.info.homed \
        -row $i -column $j
#    incr j
#    grid config $topbox_page.center.f15.info.moving \
#        -row $i -column $j
#    incr j
#    grid config $topbox_page.center.f15.info.negative \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.center.f15.info.positive \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.center.f15.info.error \
#        -row $i -column $j -sticky ew
#
# Pack F15 arrow increments
#
    incr i
    set j 0
    grid config $topbox_page.center.f15.info.incr_entry \
        -row $i -column $j -sticky e
    incr j
    grid config $topbox_page.center.f15.info.incr_label \
        -row $i -rowspan 2 -column $j -sticky n
#
# Pack F15 position buttons
#
    incr j
    grid config $topbox_page.center.f15.info.enable \
	-row $i -column $j -sticky ew
    incr j
    grid config $topbox_page.center.f15.info.stow \
       -row $i -column $j -sticky ew
#
    incr i
    set j 2
    grid config $topbox_page.center.f15.info.nd \
	-row $i -column $j -sticky ew
    incr j
    grid config $topbox_page.center.f15.info.laser \
       -row $i -column $j -sticky ew
#
# Pack the F15 parts
#
    pack $topbox_page.center.f15.info -side top
#
##################################
# Alignment Stage
##################################
#
# **Note: All background command are Indigo for historical reasons
#
#==================================
# Indigo frames
#==================================
#
    frame $topbox_page.center.indigo -relief ridge -border 4
#
    frame $topbox_page.center.indigo.info
    frame $topbox_page.center.indigo.info.arrows -border 4
#
#==================================
# Indigo Label
#==================================
#
    label $topbox_page.center.indigo.info.title \
        -text "Alignment\nStage" -bg lightskyblue -width 9
#
# Indigo move arrows
#
    button $topbox_page.center.indigo.info.arrows.x_pos \
        -bitmap @$BitMaps_Dir/right \
	-command {
	    topbox_move_rel "Indigo" $Topbox_Indigo_Incr  $topbox_page
	}
    button $topbox_page.center.indigo.info.arrows.x_neg \
        -bitmap @$BitMaps_Dir/left \
	-command {
	    topbox_move_rel "Indigo" [expr -1 * $Topbox_Indigo_Incr ]  $topbox_page
	}
    label $topbox_page.center.indigo.info.arrows.space \
	-text " "

    grid config $topbox_page.center.indigo.info.arrows.x_neg \
        -row 0 -column 0
    grid config $topbox_page.center.indigo.info.arrows.space \
        -row 0 -column 1
    grid config $topbox_page.center.indigo.info.arrows.x_pos \
        -row 0 -column 2
#
# Indigo motion increment for arrows
#
    label $topbox_page.center.indigo.info.incr_label \
        -text "Increment\nin mm"
    entry $topbox_page.center.indigo.info.incr_entry \
        -textvariable Topbox_Indigo_Incr \
        -width 7 -justify right -bg white
#
# Indigo column labels
#
    label $topbox_page.center.indigo.info.position_label \
	-text "Absolute\nPosition" -bg cyan
    label $topbox_page.center.indigo.info.connected_label \
	-text "Connected" -bg cyan
    label $topbox_page.center.indigo.info.homed_label \
	-text "Homed" -bg cyan
    label $topbox_page.center.indigo.info.moving_label \
	-text "Moving"
    label $topbox_page.center.indigo.info.negative_label \
	-text "Negative\nLimit"
    label $topbox_page.center.indigo.info.positive_label \
	-text "Positive\nLimit"
    label $topbox_page.center.indigo.info.error_label \
	-text "Error"
#
# Indigo current position display
#
    label  $topbox_page.center.indigo.info.value  \
        -textvariable Status(Indigo_Position) \
        -width 9 -justify right -bg grey84 -relief sunken
#
# Indigo connected light
#
    button $topbox_page.center.indigo.info.connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_connect_device "Indigo" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error connecting to Alignment Stage\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# Indigo homed light
#
    button $topbox_page.center.indigo.info.homed \
	-state disabled -bg grey84 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_home "Indigo" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error homing Alignment Stage\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# Indigo moving flag
#
    checkbutton $topbox_page.center.indigo.info.moving \
	-variable Status(Indigo_Moving) -state disabled -disabledforeground green
#
# Indigo negative limit flag
#
    checkbutton $topbox_page.center.indigo.info.negative \
	-variable Status(Indigo_Negative) -state disabled -disabledforeground red
#
# Indigo positive limit flag
#
    checkbutton $topbox_page.center.indigo.info.positive \
	-variable Status(Indigo_Positive) -state disabled -disabledforeground red
#
# Indigo error flag
#
    checkbutton $topbox_page.center.indigo.info.error \
	-variable Status(Indigo_Error) -state disabled -disabledforeground red
#
# Indigo position buttons
#
    button $topbox_page.center.indigo.info.enable -text "Enable" -width 6 \
	-command {
	    topbox_enable "Indigo" $topbox_page
	}
    button $topbox_page.center.indigo.info.stow -text "Stow" \
	-command {
	    topbox_move_abs "Indigo" $Indigo_Stow  $topbox_page
	}
    button $topbox_page.center.indigo.info.camera -text "Indigo" \
	-command {
	    topbox_move_abs "Indigo" $Indigo_Camera  $topbox_page
	}
    button $topbox_page.center.indigo.info.laser -text "Laser" \
	-command {
	    topbox_move_abs "Indigo" $Indigo_Laser  $topbox_page
	}
#
# Pack Indigo Label
#
    set i 0
    set j 0
    grid config $topbox_page.center.indigo.info.title \
        -row $i -column $j -sticky ewns
#
# Pack Indigo arrows into main panel
#
    set i 1
    grid config $topbox_page.center.indigo.info.arrows \
        -row $i -column $j -sticky ew
#
# Pack Indigo column labels
#
    set i 0
    set j 1
    grid config $topbox_page.center.indigo.info.position_label \
        -row $i -column $j -sticky ewns
    incr j
    grid config $topbox_page.center.indigo.info.connected_label \
        -row $i -column $j -sticky ewns
    incr j
    grid config $topbox_page.center.indigo.info.homed_label \
        -row $i -column $j -sticky ewns
#    incr j
#    grid config $topbox_page.center.indigo.info.moving_label \
#        -row $i -column $j -sticky ew
#   incr j
#    grid config $topbox_page.center.indigo.info.negative_label \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.center.indigo.info.positive_label \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.center.indigo.info.error_label \
#        -row $i -column $j -sticky ew
#
# Pack Indigo lights
#
    incr i
    set j 1
    grid config $topbox_page.center.indigo.info.value \
        -row $i -column $j -sticky w
    incr j
    grid config $topbox_page.center.indigo.info.connected \
        -row $i -column $j
    incr j
    grid config $topbox_page.center.indigo.info.homed \
        -row $i -column $j
#    incr j
#    grid config $topbox_page.center.indigo.info.moving \
#        -row $i -column $j
#    incr j
#    grid config $topbox_page.center.indigo.info.negative \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.center.indigo.info.positive \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.center.indigo.info.error \
#        -row $i -column $j -sticky ew
#
# Pack Indigo arrow increments
#
    incr i
    set j 0
    grid config $topbox_page.center.indigo.info.incr_entry \
        -row $i -column $j -sticky e
    incr j
    grid config $topbox_page.center.indigo.info.incr_label \
        -row $i -rowspan 2 -column $j -sticky n
#
# Pack Indigo position buttons
#
    incr j
    grid config $topbox_page.center.indigo.info.enable \
	-row $i -column $j -sticky ew
    incr j
    grid config $topbox_page.center.indigo.info.stow \
       -row $i -column $j -sticky ew
#
    incr i
    set j 2
    grid config $topbox_page.center.indigo.info.camera \
	-row $i -column $j -sticky ew
    incr j
    grid config $topbox_page.center.indigo.info.laser \
       -row $i -column $j -sticky ew
#
# Pack the Indigo parts
#
    pack $topbox_page.center.indigo.info -side top
#
##################################
# WFSC Rotation
##################################
#
#==================================
# rotation frames
#==================================
#
    frame $topbox_page.right.rotation -relief ridge -border 4
#
    frame $topbox_page.right.rotation.info
    frame $topbox_page.right.rotation.info.arrows -border 4
#
#==================================
# Rotation Label
#==================================
#
    label $topbox_page.right.rotation.info.title \
        -text "WFSC\nRotation" -bg lightskyblue -width 9
#
# Rotation move arrows
#
    button $topbox_page.right.rotation.info.arrows.x_pos \
        -bitmap @$BitMaps_Dir/right \
	-command {
	    topbox_move_rel "Rotation" $Topbox_Rotation_Incr  $topbox_page
	}
    button $topbox_page.right.rotation.info.arrows.x_neg \
        -bitmap @$BitMaps_Dir/left \
	-command {
	    topbox_move_rel "Rotation" [expr -1 * $Topbox_Rotation_Incr ]  $topbox_page
	}
    label $topbox_page.right.rotation.info.arrows.space \
	-text " "

    grid config $topbox_page.right.rotation.info.arrows.x_neg \
        -row 0 -column 0
    grid config $topbox_page.right.rotation.info.arrows.space \
        -row 0 -column 1
    grid config $topbox_page.right.rotation.info.arrows.x_pos \
        -row 0 -column 2
#
# Rotation motion increment for arrows
#
    label $topbox_page.right.rotation.info.incr_label \
        -text "Increment\nin degrees" -width 11
    entry $topbox_page.right.rotation.info.incr_entry \
        -textvariable Topbox_Rotation_Incr \
        -width 7 -justify right -bg white
#
# Rotation column labels
#
    label $topbox_page.right.rotation.info.position_label \
	-text "Absolute\nPosition" -bg cyan
    label $topbox_page.right.rotation.info.connected_label \
	-text "Connected" -bg cyan
    label $topbox_page.right.rotation.info.homed_label \
	-text "Homed" -bg cyan
    label $topbox_page.right.rotation.info.moving_label \
	-text "Moving"
    label $topbox_page.right.rotation.info.enabled_label \
	-text "Enabled"
    label $topbox_page.right.rotation.info.negative_label \
	-text "Negative\nLimit"
    label $topbox_page.right.rotation.info.positive_label \
	-text "Positive\nLimit"
#
# Rotation current position display
#
    label  $topbox_page.right.rotation.info.value  \
        -textvariable Status(Rotation_Position) \
        -width 9 -justify right -bg grey84 -relief sunken
#
# Rotation connected light
#
    button $topbox_page.right.rotation.info.connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_connect_device "Rotation" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error connecting to Alignment Stage\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# Rotation homed light
#
    button $topbox_page.right.rotation.info.homed \
	-state disabled -bg grey84 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_home "Rotation" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error homing Alignment Stage\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# Rotation moving flag
#
    checkbutton $topbox_page.right.rotation.info.moving \
	-variable Status(Rotation_Moving) -state disabled -disabledforeground green
#
# Rotation enabled flag
#
    checkbutton $topbox_page.right.rotation.info.enabled \
	-variable Status(Rotation_Moving) -state disabled -disabledforeground green
#
# Rotation negative limit flag
#
    checkbutton $topbox_page.right.rotation.info.negative \
	-variable Status(Rotation_Negative) -state disabled -disabledforeground red
#
# Rotation positive limit flag
#
    checkbutton $topbox_page.right.rotation.info.positive \
	-variable Status(Rotation_Positive) -state disabled -disabledforeground red
#
# Pack Rotation Label
#
    set i 0
    set j 0
    grid config $topbox_page.right.rotation.info.title \
        -row $i -column $j -sticky ewns
    incr i
#
# Position field
#
    grid config $topbox_page.right.rotation.info.value \
        -row $i -column $j -sticky w
    incr i
#
# Pack Rotation arrows into main panel
#
    grid config $topbox_page.right.rotation.info.arrows \
        -row $i -column $j -sticky ew
    incr i
#
# Pack Rotation column labels
#
    set i 0
    set j 1
    grid config $topbox_page.right.rotation.info.connected_label \
        -row $i -column $j -sticky ewns
    incr j
    grid config $topbox_page.right.rotation.info.homed_label \
        -row $i -column $j -sticky ewns
    incr j
#    grid config $topbox_page.right.rotation.info.moving_label \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.right.rotation.info.enabled_label \
#        -row $i -column $j -sticky ew
#   incr j
#    grid config $topbox_page.right.rotation.info.negative_label \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.right.rotation.info.positive_label \
#        -row $i -column $j -sticky ew
#    incr j
#
# Pack Rotation lights
#
    incr i
    set j 1
    grid config $topbox_page.right.rotation.info.connected \
        -row $i -column $j
    incr j
    grid config $topbox_page.right.rotation.info.homed \
        -row $i -column $j
    incr j
#    grid config $topbox_page.right.rotation.info.moving \
#        -row $i -column $j
#    incr j
#    grid config $topbox_page.right.rotation.info.enabled \
#        -row $i -column $j
#    incr j
#    grid config $topbox_page.right.rotation.info.negative \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.right.rotation.info.positive \
#        -row $i -column $j -sticky ew
#
# Pack Rotation arrow increments
#
    set i 2
    set j 1
    grid config $topbox_page.right.rotation.info.incr_entry \
        -row $i -column $j -sticky e
    incr j
    grid config $topbox_page.right.rotation.info.incr_label \
        -row $i -rowspan 2 -column $j -sticky n
#
# Pack the Rotation parts
#
    pack $topbox_page.right.rotation.info -side top
#
#
##################################
# WFSC Focus
##################################
#
#==================================
# Focus frames
#==================================
#
    frame $topbox_page.right.focus -relief ridge -border 4
#
    frame $topbox_page.right.focus.info
    frame $topbox_page.right.focus.info.arrows -border 4
#
#==================================
# Focus Label
#==================================
#
    label $topbox_page.right.focus.info.title \
        -text "WFSC\nFocus" -bg lightskyblue -width 9
#
# Focus move arrows
#
    button $topbox_page.right.focus.info.arrows.x_pos \
        -bitmap @$BitMaps_Dir/right \
	-command {
	    topbox_move_rel "Focus" $Topbox_Focus_Incr  $topbox_page
	}
    button $topbox_page.right.focus.info.arrows.x_neg \
        -bitmap @$BitMaps_Dir/left \
	-command {
	    topbox_move_rel "Focus" [expr -1 * $Topbox_Focus_Incr ]  $topbox_page
	}
    label $topbox_page.right.focus.info.arrows.space \
	-text " "

    grid config $topbox_page.right.focus.info.arrows.x_neg \
        -row 0 -column 0
    grid config $topbox_page.right.focus.info.arrows.space \
        -row 0 -column 1
    grid config $topbox_page.right.focus.info.arrows.x_pos \
        -row 0 -column 2
#
# Focus motion increment for arrows
#
    label $topbox_page.right.focus.info.incr_label \
        -text "Increment in\nHex microns" -width 11
    entry $topbox_page.right.focus.info.incr_entry \
        -textvariable Topbox_Focus_Incr \
        -width 7 -justify right -bg white
#
# Focus column labels
#
    label $topbox_page.right.focus.info.position_label \
	-text "Absolute\nPosition" -bg cyan
    label $topbox_page.right.focus.info.connected_label \
	-text "Connected" -bg cyan
    label $topbox_page.right.focus.info.homed_label \
	-text "Homed" -bg cyan
    label $topbox_page.right.focus.info.moving_label \
	-text "Moving"
    label $topbox_page.right.focus.info.enabled_label \
	-text "Enabled"
    label $topbox_page.right.focus.info.negative_label \
	-text "Negative\nLimit"
    label $topbox_page.right.focus.info.positive_label \
	-text "Positive\nLimit"
#
# Focus current position display
#
    label  $topbox_page.right.focus.info.value  \
        -textvariable Status(Focus_Position) \
        -width 9 -justify right -bg grey84 -relief sunken
#
# Focus connected light
#
    button $topbox_page.right.focus.info.connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_connect_device "Focus" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error connecting to Alignment Stage\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# Focus homed light
#
    button $topbox_page.right.focus.info.homed \
	-state disabled -bg grey84 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_home "Focus" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error homing Alignment Stage\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# Focus moving flag
#
    checkbutton $topbox_page.right.focus.info.moving \
	-variable Status(Focus_Moving) -state disabled -disabledforeground green
#
# Focus enabled flag
#
    checkbutton $topbox_page.right.focus.info.enabled \
	-variable Status(Focus_Moving) -state disabled -disabledforeground green
#
# Focus negative limit flag
#
    checkbutton $topbox_page.right.focus.info.negative \
	-variable Status(Focus_Negative) -state disabled -disabledforeground red
#
# Focus positive limit flag
#
    checkbutton $topbox_page.right.focus.info.positive \
	-variable Status(Focus_Positive) -state disabled -disabledforeground red
#
# Pack Focus Label
#
    set i 0
    set j 0
    grid config $topbox_page.right.focus.info.title \
        -row $i -column $j -sticky ewns
    incr i
#
# Position field
#
    grid config $topbox_page.right.focus.info.value \
        -row $i -column $j -sticky w
    incr i
#
# Pack Focus arrows into main panel
#
    grid config $topbox_page.right.focus.info.arrows \
        -row $i -column $j -sticky ew
    incr i
#
# Pack Focus column labels
#
    set i 0
    set j 1
    grid config $topbox_page.right.focus.info.connected_label \
        -row $i -column $j -sticky ewns
    incr j
    grid config $topbox_page.right.focus.info.homed_label \
        -row $i -column $j -sticky ewns
    incr j
#    grid config $topbox_page.right.focus.info.moving_label \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.right.focus.info.enabled_label \
#        -row $i -column $j -sticky ew
#   incr j
#    grid config $topbox_page.right.focus.info.negative_label \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.right.focus.info.positive_label \
#        -row $i -column $j -sticky ew
#    incr j
#
# Pack Focus lights
#
    incr i
    set j 1
    grid config $topbox_page.right.focus.info.connected \
        -row $i -column $j
    incr j
    grid config $topbox_page.right.focus.info.homed \
        -row $i -column $j
    incr j
#    grid config $topbox_page.right.focus.info.moving \
#        -row $i -column $j
#    incr j
#    grid config $topbox_page.right.focus.info.enabled \
#        -row $i -column $j
#    incr j
#    grid config $topbox_page.right.focus.info.negative \
#        -row $i -column $j -sticky ew
#    incr j
#    grid config $topbox_page.right.focus.info.positive \
#        -row $i -column $j -sticky ew
#
# Pack Focus arrow increments
#
    set i 2
    set j 1
    grid config $topbox_page.right.focus.info.incr_entry \
        -row $i -column $j -sticky e
    incr j
    grid config $topbox_page.right.focus.info.incr_label \
        -row $i -rowspan 2 -column $j -sticky n
#
# Pack the Focus parts
#
    pack $topbox_page.right.focus.info -side top
#
##################################
#
# Pack the devices
#
    pack $topbox_page.left.fsm -side top -fill x
    pack $topbox_page.left.oap -fill x
    pack $topbox_page.center.f15 -side top -fill x
    pack $topbox_page.center.indigo -fill x
    pack $topbox_page.right.rotation -side top -fill x
    pack $topbox_page.right.focus -fill x
#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $topbox_page.left \
	 $topbox_page.center \
	 $topbox_page.right \
	    -side left -anchor nw

}
