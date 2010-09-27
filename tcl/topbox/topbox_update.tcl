###################################
#
# status_update.tcl
#
###################################
#
# This routine sets up the trace on several variables such that
#   when they are accessed, the gui is updated
#
# Variables:
#   Status_Update - set in status_get.tcl after all the Status variables
#                   have been set
#
# Written  17sep07  DLM  Started with gui_update.tcl
#
###################################
#
# Status Variables
#
trace variable Status_Update w status_update_set
proc status_update_set { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl

    upvar #0 Status Status
    upvar #0 Topbox_ND_Value Topbox_ND_Value
#
# If the Main Window exists then update the Camera label
#
    if { [winfo exists $main_win] } {
#
# Check to see which Tix Notebook page is on top and set values on specific pages
#   accordingly.
#
	set Tix_Page [ $main_win.aux raised ]
    }
#
# If the Topbox Page exists then update this page
#
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $topbox_page] ] ] } {
#
#       FSM_Tip_Connected
#
	if { $Status(FSM_Tip_Connected) } {
	    $topbox_page.left.fsm.info.tip_connected configure -bg green -state disabled
	    $topbox_page.left.fsm.info.tip_homed configure -bg red -activebackground tomato -state normal
	} else {
	    $topbox_page.left.fsm.info.tip_connected configure -bg red -state normal
	    $topbox_page.left.fsm.info.tip_homed configure -bg grey84 -state disabled
	}
#
#       FSM_Tip_Connected
#
	if { $Status(FSM_Tilt_Connected) } {
	    $topbox_page.left.fsm.info.tilt_connected configure -bg green -state disabled
	    $topbox_page.left.fsm.info.tilt_homed configure -bg red -activebackground tomato -state normal
	} else {
	    $topbox_page.left.fsm.info.tilt_connected configure -bg red -state normal
	    $topbox_page.left.fsm.info.tilt_homed configure -bg grey84 -state disabled
	}
#
#       FSM_Tip_Homed
#
	if { $Status(FSM_Tip_Home) } {
	    $topbox_page.left.fsm.info.tip_homed configure -bg green -state disabled
	} else {
	    $topbox_page.left.fsm.info.tip_homed configure -bg red -activebackground tomato -state normal
	}
#
#       FSM_Tilt_Homed
#
	if { $Status(FSM_Tilt_Home) } {
	    $topbox_page.left.fsm.info.tilt_homed configure -bg green -state disabled
	} else {
	    $topbox_page.left.fsm.info.tilt_homed configure -bg red -activebackground tomato -state normal
	}
#
#       OAP_Tip_Connected
#
	if { $Status(OAP_Tip_Connected) } {
	    $topbox_page.left.oap.info.tip_connected configure -bg green -state disabled
	    $topbox_page.left.oap.info.tip_homed configure -bg red -activebackground tomato -state normal
	} else {
	    $topbox_page.left.oap.info.tip_connected configure -bg red -state normal
	    $topbox_page.left.oap.info.tip_homed configure -bg grey84 -state disabled
	}
#
#       OAP_Tip_Connected
#
	if { $Status(OAP_Tilt_Connected) } {
	    $topbox_page.left.oap.info.tilt_connected configure -bg green -state disabled
	    $topbox_page.left.oap.info.tilt_homed configure -bg red -activebackground tomato -state normal
	} else {
	    $topbox_page.left.oap.info.tilt_connected configure -bg red -state normal
	    $topbox_page.left.oap.info.tilt_homed configure -bg grey84 -state disabled
	}
#
#       OAP_Tip_Homed
#
	if { $Status(OAP_Tip_Home) } {
	    $topbox_page.left.oap.info.tip_homed configure -bg green -state disabled
	} else {
	    $topbox_page.left.oap.info.tip_homed configure -bg red -activebackground tomato -state normal
	}
#
#       OAP_Tilt_Homed
#
	if { $Status(OAP_Tilt_Home) } {
	    $topbox_page.left.oap.info.tilt_homed configure -bg green -state disabled
	} else {
	    $topbox_page.left.oap.info.tilt_homed configure -bg red -activebackground tomato -state normal
	}
#
#       F15_Connected
#
	if { $Status(F15_Connected) } {
	    $topbox_page.center.f15.info.connected configure -bg green -state disabled
	    $topbox_page.center.f15.info.homed configure -bg red -activebackground tomato -state normal
	} else {
	    $topbox_page.center.f15.info.connected configure -bg red -state normal
	    $topbox_page.center.f15.info.homed configure -bg grey84 -state disabled
	}
#
#       F15_Homed
#
	if { $Status(F15_Home) } {
	    $topbox_page.center.f15.info.homed configure -bg green -state disabled
	} else {
	    $topbox_page.center.f15.info.homed configure -bg red -activebackground tomato -state normal
	}
#
#       F15_Enabled
#
	if { $Status(F15_Enabled) } {
	    $topbox_page.center.f15.info.enable configure -bg green \
		-activebackground tomato -text "Enabled"
	} else {
	    $topbox_page.center.f15.info.enable configure -bg red \
		-activebackground lightgreen -text "Disabled"
	}
#
#       F15_Position
#
	if { [ expr abs( $Status(F15_Position) - $F15_Stow ) ] < 1.0 } {
	    $topbox_page.center.f15.info.stow configure -bg green -activebackground lightgreen
	} else {
	    $topbox_page.center.f15.info.stow configure -bg red -activebackground tomato
	}
	if { [ expr abs( $Status(F15_Position) - $F15_ND ) ] < 1.0 } {
	    $topbox_page.center.f15.info.nd configure -bg green -activebackground lightgreen
	} else {
	    $topbox_page.center.f15.info.nd configure -bg grey84 -activebackground grey100
	}
	if { [ expr abs( $Status(F15_Position) - $F15_Laser ) ] < 1.0 } {
	    $topbox_page.center.f15.info.laser configure -bg green -activebackground lightgreen
	} else {
	    $topbox_page.center.f15.info.laser configure -bg grey84 -activebackground grey100
	}
#
#       Indigo_Connected
#
	if { $Status(Indigo_Connected) } {
	    $topbox_page.center.indigo.info.connected configure -bg green -state disabled
	    $topbox_page.center.indigo.info.homed configure -bg red -activebackground tomato -state normal
	} else {
	    $topbox_page.center.indigo.info.connected configure -bg red -state normal
	    $topbox_page.center.indigo.info.homed configure -bg grey84 -state disabled
	}
#
#       Indigo_Homed
#
	if { $Status(Indigo_Home) } {
	    $topbox_page.center.indigo.info.homed configure -bg green -state disabled
	} else {
	    $topbox_page.center.indigo.info.homed configure -bg red -activebackground tomato -state normal
	}
#
#       Indigo_Enabled
#
	if { $Status(Indigo_Enabled) } {
	    $topbox_page.center.indigo.info.enable configure -bg green \
		-activebackground tomato -text "Enabled"
	} else {
	    $topbox_page.center.indigo.info.enable configure -bg red \
		-activebackground lightgreen -text "Disabled"
	}
#
#       Indigo_Position
#
	if { [ expr abs( $Status(Indigo_Position) - $Indigo_Stow ) ] < 1.0 } {
	    $topbox_page.center.indigo.info.stow configure -bg green -activebackground lightgreen
	} else {
	    $topbox_page.center.indigo.info.stow configure -bg red -activebackground tomato
	}
	if { [ expr abs( $Status(Indigo_Position) - $Indigo_Camera ) ] < 1.0 } {
	    $topbox_page.center.indigo.info.camera configure -bg green -activebackground lightgreen
	} else {
	    $topbox_page.center.indigo.info.camera configure -bg grey84 -activebackground grey100
	}
	if { [ expr abs( $Status(Indigo_Position) - $Indigo_Laser ) ] < 1.0 } {
	    $topbox_page.center.indigo.info.laser configure -bg green -activebackground lightgreen
	} else {
	    $topbox_page.center.indigo.info.laser configure -bg grey84 -activebackground grey100
	}
#
#       Rotation_Connected
#
	if { $Status(Rotation_Connected) } {
	    $topbox_page.right.rotation.info.connected configure -bg green -state disabled
	    $topbox_page.right.rotation.info.homed configure -bg red -activebackground tomato -state normal
	} else {
	    $topbox_page.right.rotation.info.connected configure -bg red -state normal
	    $topbox_page.right.rotation.info.homed configure -bg grey84 -state disabled
	}
#
#       Rotation_Homed
#
	if { $Status(Rotation_Home) } {
	    $topbox_page.right.rotation.info.homed configure -bg green -state disabled
	} else {
	    $topbox_page.right.rotation.info.homed configure -bg red -activebackground tomato -state normal
	}
#
#       Focus_Connected
#
	if { $Status(Focus_Connected) } {
	    $topbox_page.right.focus.info.connected configure -bg green -state disabled
	    $topbox_page.right.focus.info.homed configure -bg red -activebackground tomato -state normal
	} else {
	    $topbox_page.right.focus.info.connected configure -bg red -state normal
	    $topbox_page.right.focus.info.homed configure -bg grey84 -state disabled
	}
#
#       Focus_Homed
#
	if { $Status(Focus_Home) } {
	    $topbox_page.right.focus.info.homed configure -bg green -state disabled
	} else {
	    $topbox_page.right.focus.info.homed configure -bg red -activebackground tomato -state normal
	}

    }
#
# Update the Filter Wheel information on the main page
#
    if { [winfo exists $main_win] } {
#
#       Filter Wheel Current Filter
#
	if { [ expr $Status(Fwheel_Connected) && $Status(Fwheel_Home) ] } {
	    set current [format "%.0f" $Status(Fwheel_Position)]
	    if { $current >= 1 && $current <= 8 } {
		for {set i 0} {$i < 8} {set i [expr $i + 1]} {
		    if { $Topbox_ND_Value($i) == $current } {
			set current_label $i
		    }
		}
		if { $current_label >= 0 && $current_label <= 7 } {
		    $main_win.main.info.fwheel.label configure -bg green -text $Topbox_ND_Name($current_label)
		} else {
		    $main_win.main.info.fwheel.label configure -bg red -text "Unknown"
		}
	    } else {
		$main_win.main.info.fwheel.label configure -bg red -text "Unknown"
	    }
	} else {
	    $main_win.main.info.fwheel.label configure -bg red -text "Unknown"
	}
    }
#
# If the Alignment Page exists then update this page
#
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $alignment_page] ] ] } {
#
#       Filter Wheel Connected
#
	if { $Status(Fwheel_Connected) } {
	    $alignment_page.left.fwheel.info.connected configure -bg green -state disabled
	    $alignment_page.left.fwheel.info.homed configure -bg red -activebackground tomato -state normal
	} else {
	    $alignment_page.left.fwheel.info.connected configure -bg red -state normal
	    $alignment_page.left.fwheel.info.homed configure -bg grey84 -state disabled
	}
#
#       Filter Wheel Homed
#
	if { $Status(Fwheel_Home) } {
	    $alignment_page.left.fwheel.info.homed configure -bg green -state disabled
	    set Status(Fwheel_Enabled) 1
	} else {
	    $alignment_page.left.fwheel.info.homed configure -bg red -activebackground tomato -state normal
	    set Status(Fwheel_Enabled) 0
	}
#
#       Filter Wheel Current Filter
#
	set current [format "%.0f" $Status(Fwheel_Position)]
	if { $current >= 1 && $current <= 8 } {
	    for {set i 0} {$i < 8} {set i [expr $i + 1]} {
		$alignment_page.left.fwheel.info.label_$i configure -bg grey84
		if { $Topbox_ND_Value($i) == $current } {
		    set current_label $i
		}
	    }
	    if { $current_label >= 0 && $current_label <= 7 } {
		$alignment_page.left.fwheel.info.label_$current_label configure -bg green
	    }
	}

    }
#
# If the Server Page exists then update this page
#
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
#
#       Power Connected
#
	if { $Status(Power_Connected) } {
	    $server_page.server.power.connected_label configure -bg grey84
	    $server_page.server.power.connected configure -bg green -state disabled
	} else {
	    $server_page.server.power.connected_label configure -bg red
	    $server_page.server.power.connected configure -bg red -state normal
	}
#
#       WFSC Power
#
	if { $Status(Power_Connected) } {
	    if { $Status(Power_WFSC) } {
		$server_page.server.power.wfsc_start configure -bg green -activebackground lawngreen -state normal
	    } else {
		$server_page.server.power.wfsc_start configure -bg red -activebackground tomato -state normal
	    }
	} else {
	    $server_page.server.power.wfsc_start configure -bg grey84 -state disabled
	}
#
#       WFSC Peltier Power
#
	if { $Status(Power_Connected) } {
	    if { $Status(Power_Peltier) } {
		$server_page.server.power.peltier_start configure -bg green -activebackground lawngreen -state normal
	    } else {
		$server_page.server.power.peltier_start configure -bg red -activebackground tomato -state normal
	    }
	} else {
	    $server_page.server.power.peltier_start configure -bg grey84 -state disabled
	}
#
#       Stella Power
#
	if { $Status(Power_Connected) } {
	    if { $Status(Power_Stella) } {
		$server_page.server.power.stella_start configure -bg green -activebackground lawngreen -state normal
	    } else {
		$server_page.server.power.stella_start configure -bg red -activebackground tomato -state normal
	    }
	} else {
	    $server_page.server.power.stella_start configure -bg grey84 -state disabled
	}
#
#       Alignment_Laser Power
#
	if { $Status(Power_Connected) } {
	    if { $Status(Power_Alignment_Laser) } {
		$server_page.server.power.alignment_start configure -bg green -activebackground lawngreen -state normal
	    } else {
		$server_page.server.power.alignment_start configure -bg grey84 -activebackground grey100 -state normal
	    }
	} else {
	    $server_page.server.power.alignment_start configure -bg grey84 -state disabled
	}
#
#       F15_Laser Power
#
	if { $Status(Power_Connected) } {
	    if { $Status(Power_F15_Laser) } {
		$server_page.server.power.f15_start configure -bg green -activebackground lawngreen -state normal
	    } else {
		$server_page.server.power.f15_start configure -bg grey84 -activebackground grey100 -state normal
	    }
	} else {
	    $server_page.server.power.f15_start configure -bg grey84 -state disabled
	}
#
#       Indigo Power
#
	if { $Status(Power_Connected) } {
	    if { $Status(Power_Indigo) } {
		$server_page.server.power.indigo_start configure -bg green -activebackground lawngreen -state normal
	    } else {
		$server_page.server.power.indigo_start configure -bg grey84 -activebackground grey100 -state normal
	    }
	} else {
	    $server_page.server.power.indigo_start configure -bg grey84 -state disabled
	}
#
#       Spare1 Power
#
#	if { $Status(Power_Connected) } {
#	    if { $Status(Power_Spare1) } {
#		$server_page.server.power.spare1_start configure -bg green -activebackground lawngreen -state normal
#	    } else {
#		$server_page.server.power.spare1_start configure -bg grey84 -activebackground grey100 -state normal
#	    }
#	} else {
#	    $server_page.server.power.spare1_start configure -bg grey84 -state disabled
#	}
#
#       Spare2 Power
#
#	if { $Status(Power_Connected) } {
#	    if { $Status(Power_Spare2) } {
#		$server_page.server.power.spare2_start configure -bg green -activebackground lawngreen -state normal
#	    } else {
#		$server_page.server.power.spare2_start configure -bg grey84 -activebackground grey100 -state normal
#	    }
#	} else {
#	    $server_page.server.power.spare2_start configure -bg grey84 -state disabled
#	}

    }
}
