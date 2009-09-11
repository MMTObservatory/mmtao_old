################################################## 
#
# alignment_page.tcl
#
####
#
# Create window to turn on Guido's data alignment
#
# Written  13apr06  DLM 
#
##################################################

proc alignment_page { alignment_win page } {

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    source $PCR_HOME/tcl/alignment/alignment_globals.tcl
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl
    source $PCR_HOME/tcl/rotator/rotator_globals.tcl
    source $PCR_HOME/tcl/status/status_globals.tcl
    source $PCR_HOME/tcl/bitmaps/bitmaps_globals.tcl
#
    global reply_data
#
    upvar #0 Info Info
#
# Create the tix NoteBook frame
#
    global alignment_page
    set alignment_page [ $alignment_win subwidget $page ]
#
############################################
#
# Build window
#
############################################
#
    frame $alignment_page.left
    frame $alignment_page.center
    frame $alignment_page.right
    frame $alignment_page.far_right
#
##################################
# Filter Wheel
##################################
#
#==================================
# FWheel frames
#==================================
#
    frame $alignment_page.left.fwheel -relief ridge -border 4
#
    frame $alignment_page.left.fwheel.info
#
#==================================
# FWheel Label
#==================================
#
    label $alignment_page.left.fwheel.info.title \
        -text "Filter\nWheel" -bg lightskyblue
#
# FWheel column labels
#
    label $alignment_page.left.fwheel.info.connected_label \
	-text "Connected" -bg cyan
    label $alignment_page.left.fwheel.info.homed_label \
	-text "Homed" -bg cyan
#
# FWheel connected light
#
    label $alignment_page.left.fwheel.info.strut\
	-text " " -pady 5
    button $alignment_page.left.fwheel.info.connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_connect_device "Fwheel" $alignment_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error connecting to Alignment Stage\n $reply_data" \
		    -parent $alignment_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# FWheel homed light
#
    button $alignment_page.left.fwheel.info.homed \
	-state disabled -bg grey84 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_home "Fwheel" $alignment_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error homing Alignment Stage\n $reply_data" \
		    -parent $alignment_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# FWheel column labels
#
    label $alignment_page.left.fwheel.info.select_label \
	-text "Select" -bg cyan
    label $alignment_page.left.fwheel.info.current_label \
	-text "Current" -bg cyan -anchor w
#
# Requet the Filter Wheel to move to the requested ND
#
    button $alignment_page.left.fwheel.info.go \
	-text "Go" -bg green \
	-command {
	    set status [ topbox_move_abs "Fwheel" $Topbox_ND_Select $alignment_page ]
	}
#
# Pack FWheel Label
#
    set k 0
    set j 0
    grid config $alignment_page.left.fwheel.info.title \
        -row $k -column $j -sticky ewns
    incr j
    grid config $alignment_page.left.fwheel.info.connected_label \
        -row $k -column $j -columnspan 2 -sticky ewns
    incr j
    incr j
    grid config $alignment_page.left.fwheel.info.homed_label \
        -row $k -column $j -sticky ewns

    incr k
    set j 0
    grid config $alignment_page.left.fwheel.info.strut \
        -row $k -column $j
    incr j
    grid config $alignment_page.left.fwheel.info.connected \
        -row $k -column $j -columnspan 2
    incr j
    incr j
    grid config $alignment_page.left.fwheel.info.homed \
        -row $k -column $j
    incr k

    set j 1
    grid config $alignment_page.left.fwheel.info.select_label \
        -row $k -column $j -sticky ew
    incr j
    grid config $alignment_page.left.fwheel.info.current_label \
        -row $k -column $j -sticky ew -columnspan 2
    incr k
#
#
#
    set j 1
    for {set i 0} {$i < 8} {set i [expr $i + 1]} {

	set l $j

	radiobutton $alignment_page.left.fwheel.info.nd_$i -variable Topbox_ND_Select \
	    -value $Topbox_ND_Value($i) -anchor center \
	    -selectcolor green
	label $alignment_page.left.fwheel.info.label_$i \
	    -text $Topbox_ND_Name($i) -anchor w

	grid config $alignment_page.left.fwheel.info.nd_$i \
	    -row $k -column $l -sticky ew
	incr l
	grid config $alignment_page.left.fwheel.info.label_$i \
	    -row $k -column $l -sticky w -columnspan 2
	incr k
    }

    set k 6
    set j 0
    grid config $alignment_page.left.fwheel.info.go \
	-row $k -column $j -sticky ns -rowspan 2
#
# Pack the FWheel parts
#
    pack $alignment_page.left.fwheel.info
    pack $alignment_page.left.fwheel
#
#-----------------------------
# HEX connections
#-----------------------------
#
    frame $alignment_page.right.hex -relief ridge -border 4
    frame $alignment_page.right.hex.move
    frame $alignment_page.right.hex.incr
#
#-----------------------------
#
    label $alignment_page.right.hex.hex_label \
	-text "Focus" -bg cyan
#
    radiobutton $alignment_page.right.hex.wfsc -text "WFSC" \
	-var Hex_Move -value WFSC -anchor e
    radiobutton $alignment_page.right.hex.science -text "Science\nCamera" \
	-var Hex_Move -value Science -anchor e

    button $alignment_page.right.hex.move.y_pos -bitmap @$BitMaps_Dir/up \
	-command {
	    hex_move up $alignment_page
	}
    button $alignment_page.right.hex.move.y_neg -bitmap @$BitMaps_Dir/down \
	-command {
	    hex_move down $alignment_page
	}
    label $alignment_page.right.hex.move.pos_label -height 2 -width 18 \
	-text "Spots Move Inward" -anchor w
    label $alignment_page.right.hex.move.neg_label -height 2 -width 18 \
	-text "Spots Move Outward" -anchor w


    radiobutton $alignment_page.right.hex.incr_10 -text "10 microns" \
	-var Hex_Incr -value 10 -anchor w \
	-command {
	    set Hex_Incr 10
	}

    radiobutton $alignment_page.right.hex.incr_20 -text "20 microns" \
	-var Hex_Incr -value 20 -anchor w \
	-command {
	    set Hex_Incr 20
	}

    radiobutton $alignment_page.right.hex.incr_50 -text "50 microns" \
	-var Hex_Incr -value 50 -anchor w \
	-command {
	    set Hex_Incr 50
	}

    label $alignment_page.right.hex.incr.incr_label \
	-text "Increment in\nHex microns"

    entry $alignment_page.right.hex.incr.incr_entry \
	-textvariable Hex_Incr \
	-width 5 -justify right -bg white
#
# Pack the grid
#
    set i 0
    grid config $alignment_page.right.hex.move.y_pos \
	-row $i -column 0
    grid config $alignment_page.right.hex.move.pos_label \
	-row $i -column 1 -sticky w
    incr i
    grid config $alignment_page.right.hex.move.y_neg \
	-row $i -column 0
    grid config $alignment_page.right.hex.move.neg_label \
	-row $i -column 1 -sticky w

    set i 0
    grid config $alignment_page.right.hex.incr.incr_label \
	-row $i -column 1 -sticky ew
    grid config $alignment_page.right.hex.incr.incr_entry \
	-row $i -column 0
    incr i

    set i 0
    grid config $alignment_page.right.hex.hex_label \
	-row $i -column 0 -sticky nsew -columnspan 3
    incr i
    grid config $alignment_page.right.hex.wfsc  \
	-row $i -column 0 -sticky w
    incr i
    grid config $alignment_page.right.hex.science  \
	-row $i -column 0 -sticky w
    incr i
    grid config $alignment_page.right.hex.move \
	-row $i -column 0 -sticky nsew
    incr i
    grid config $alignment_page.right.hex.incr \
	-row $i -column 0 -sticky ew
    incr i
    grid config $alignment_page.right.hex.incr_10 \
	-row $i -column 0
    incr i
    grid config $alignment_page.right.hex.incr_20 \
	-row $i -column 0
    incr i
    grid config $alignment_page.right.hex.incr_50 \
	-row $i -column 0
#
#---------------------------------------------------
# Pack the Hex stuff
#---------------------------------------------------
#
    pack $alignment_page.right.hex \
	-side top -anchor w
#
##################################
# Reconstructor
##################################
#
    frame $alignment_page.center.recon -relief ridge -border 4
#
    label $alignment_page.center.recon.label \
	-text "Reconstructor" -bg cyan
#
    menubutton $alignment_page.center.recon.list -textvar Info(Recon_Name) \
	-menu $alignment_page.center.recon.list.menu \
	-relief raised -border 2 -disabledforeground black
    set recon_menu [menu $alignment_page.center.recon.list.menu]
    $recon_menu add radiobutton -label 80 -variable recon_size \
	-command {
	    recon_set $recon_size $server_page
	}
    $recon_menu add radiobutton -label 56 -variable recon_size \
	-command {
	    recon_set $recon_size $server_page
	}
    $recon_menu add radiobutton -label 55 -variable recon_size \
	-command {
	    recon_set $recon_size $server_page
	}
    $recon_menu add radiobutton -label 44 -variable recon_size \
	-command {
	    recon_set $recon_size $server_page
	}
    $recon_menu add radiobutton -label 23 -variable recon_size \
	-command {
	    recon_set $recon_size $server_page
	}
#
# Pack the reconstructor stuff
#
    set i 0
    grid config $alignment_page.center.recon.label \
	-row $i -column 0 -sticky ew
    incr i
    grid config $alignment_page.center.recon.list \
	-row $i -column 0
#
##################################
#
# Pack the devices
#
    pack $alignment_page.center.recon -side left
#
##################################
# Rotator
##################################
#
    frame $alignment_page.far_right.rot -relief ridge -border 4
#
    label $alignment_page.far_right.rot.label \
	-text "Rotator" -bg cyan
#
    checkbutton $alignment_page.far_right.rot.on_off -text "Not Tracking\n(Press to Start)" \
	-bg grey84 -var Rotator_Tracking -width 15
#
# Pack the rotator stuff
#
    set i 0
    grid config $alignment_page.far_right.rot.label \
	-row $i -column 0 -sticky ew
    incr i
    grid config $alignment_page.far_right.rot.on_off \
	-row $i -column 0
#
##################################
#
# Pack the devices
#
    pack $alignment_page.far_right.rot -side left
#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $alignment_page.left \
	$alignment_page.center \
	$alignment_page.right \
	$alignment_page.far_right \
	-side left -anchor nw
}
