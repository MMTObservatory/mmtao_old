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
#
#-----------------------------
#
    label $alignment_page.right.hex.hex_label \
	-text "Focus at Science Camera" -bg cyan
    button $alignment_page.right.hex.all -text "All"  \
	-command {
	    hex_get all $alignment_page
	}
    button $alignment_page.right.hex.motion -text "In Motion?"  \
	-command {
	    hex_get motion_flag $alignment_page
	}
    button $alignment_page.right.hex.up -text "+10"  \
	-command {
	    hex_focus 10 $alignment_page
	}
    button $alignment_page.right.hex.down -text "-10"  \
	-command {
	    hex_focus -10 $alignment_page
	}
#
# Pack the focus buttons
#
    set i 0
    grid config $alignment_page.right.hex.hex_label \
        -row $i -column 0 -sticky ew
    incr i
    grid config $alignment_page.right.hex.all \
        -row $i -column 0 -sticky e
    incr i
    grid config $alignment_page.right.hex.motion \
        -row $i -column 0 -sticky e
    incr i
    grid config $alignment_page.right.hex.up \
        -row $i -column 0 -sticky e
    incr i
    grid config $alignment_page.right.hex.down \
        -row $i -column 0 -sticky e
    incr i
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
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $alignment_page.left \
	$alignment_page.center \
	$alignment_page.right \
	-side left -anchor nw
}
