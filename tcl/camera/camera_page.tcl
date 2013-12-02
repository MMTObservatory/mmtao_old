################################################## 
#
# camera_page.tcl
#
####
#
# Create window to deal with camera stuff.
#
# Written  7apr06 DLM Started with system_page.tcl
#
##################################################

proc camera_page { camera_win page } {

    global PCR_HOME
    source $PCR_HOME/tcl/camera/camera_globals.tcl
    source $PCR_HOME/tcl/bitmaps/bitmaps_globals.tcl
#
    upvar #0 Info Info
#
    global reply_data
#
# Create the tix NoteBook frame
#
    global camera_page
    set camera_page [ $camera_win subwidget $page ]
#
# Build the page
#
    frame $camera_page.cam_size
    frame $camera_page.cam_source
    frame $camera_page.cam_rate
    frame $camera_page.cam_run
    frame $camera_page.cam_bias -relief ridge -border 4
    frame $camera_page.cam_servers
#
# Radio buttons to the source of the camera frame
#
    label $camera_page.cam_source.label -text "Camera Source" -bg cyan -width 15
    radiobutton $camera_page.cam_source.test -text "Test" -var Info(WFSC_Source) \
	-value Test -anchor e \
	-command {
	    pcr_request cam_test $camera_page
	    $camera_page.cam_source.test configure -disabledforeground black -bg green
	    $camera_page.cam_source.cam configure -disabledforeground grey -bg grey84
	}
    radiobutton $camera_page.cam_source.cam  -text "Camera" -var Info(WFSC_Source) \
	-value Camera -anchor e  \
	-command {
	    pcr_request cam_image $camera_page
	    $camera_page.cam_source.cam configure -disabledforeground black -bg green
	    $camera_page.cam_source.test configure -disabledforeground grey -bg grey84
	}
#
# Pack the size buttons
#
    set i 0
    grid config $camera_page.cam_source.label \
	-row $i -column 0 -sticky w
    incr i
    grid config $camera_page.cam_source.test \
	-row $i -column 0 -sticky w
    incr i
    grid config $camera_page.cam_source.cam \
	-row $i -column 0 -sticky w    
#
# Radio buttons to the size of the camera frame
#
    label $camera_page.cam_size.label -text "Camera Size" -width 20
    radiobutton $camera_page.cam_size.full -text "Full      (72x72)" -var Info(WFSC_Size) \
	-value Full -anchor e -state disabled \
	-command {
	    pcr_request cam_full $camera_page
	    $camera_page.cam_size.full configure -disabledforeground black -bg green
	    $camera_page.cam_size.bin configure -disabledforeground grey -bg grey84
	}
    radiobutton $camera_page.cam_size.bin  -text "Binned (24x24)" -var Info(WFSC_Size) \
	-value Bin -anchor e -state disabled \
	-command {
	    pcr_request cam_bin $camera_page
	    $camera_page.cam_size.bin configure -disabledforeground black -bg green
	    $camera_page.cam_size.full configure -disabledforeground grey -bg grey84
	}
#
# Pack the size buttons
#
    set i 0
    grid config $camera_page.cam_size.label \
	-row $i -column 0 -sticky ew
    incr i
    grid config $camera_page.cam_size.full \
	-row $i -column 0 -sticky w
    incr i
    grid config $camera_page.cam_size.bin \
	-row $i -column 0 -sticky w    
#
# Set camera rate
#
    label $camera_page.cam_rate.label -text "Camera Frequency" -width 20

    menubutton $camera_page.cam_rate.list -textvar Info(WFSC_Frequency) \
	-menu $camera_page.cam_rate.list.menu -state disabled \
	-relief raised -border 2 -disabledforeground black
    set rate_menu [menu $camera_page.cam_rate.list.menu]
    $rate_menu add radiobutton -label 550 -variable cam_rate \
	-command {
	    camera_rate_set $cam_rate $camera_page
	}
    $rate_menu add radiobutton -label 250 -variable cam_rate \
	-command {
	    camera_rate_set $cam_rate $camera_page
	}
    $rate_menu add radiobutton -label 150 -variable cam_rate \
	-command {
	    camera_rate_set $cam_rate $camera_page
	}
    $rate_menu add radiobutton -label 100 -variable cam_rate \
	-command {
	    camera_rate_set $cam_rate $camera_page
	}
    $rate_menu add radiobutton -label 50 -variable cam_rate \
	-command {
	    camera_rate_set $cam_rate $camera_page
	}
    $rate_menu add radiobutton -label 25 -variable cam_rate \
	-command {
	    camera_rate_set $cam_rate $camera_page
	}
    $rate_menu add radiobutton -label 15 -variable cam_rate \
	-command {
	    camera_rate_set $cam_rate $camera_page
	}
    $rate_menu add radiobutton -label 10 -variable cam_rate \
	-command {
	    camera_rate_set $cam_rate $camera_page
	}
    $rate_menu add radiobutton -label 5 -variable cam_rate \
	-command {
	    camera_rate_set $cam_rate $camera_page
	}
#
# Pack the rate buttons
#
    set i 0
    grid config $camera_page.cam_rate.label \
	-row $i -column 0 -sticky ew
    incr i
    grid config $camera_page.cam_rate.list \
	-row $i -column 0
#
# Start/Stop button
#
    label $camera_page.cam_run.label -text "Camera Status" -width 20
    button $camera_page.cam_run.start_stop -text "Stopped\n  " -bg yellow \
	-state disabled \
	-command {
	    camera_start_stop $camera_page
	}
#
# Pack the Start/Stop buttons
#
    set i 0
    grid config $camera_page.cam_run.label \
	-row $i -column 0 -sticky ew
    incr i
    grid config $camera_page.cam_run.start_stop \
	-row $i -column 0 -sticky ew
#
# Label the Camera Bias Level box
#
    label $camera_page.cam_bias.label -text "Camera Bias Levels"
#
# Create the labels and buttons
#
    for { set k 0 } { $k < 4 } { incr k } {
#
	label $camera_page.cam_bias.label_$k -text "Quad $k" -width 8 -anchor e

	label $camera_page.cam_bias.value_$k -textvar Info(WFSC_Bias_$k) \
	    -bg green -width 4

	scale $camera_page.cam_bias.slider_$k -orient horizontal \
	    -from 0 -to $Camera_Bias_Max \
	    -variable Camera_Bias($k) \
	    -digits 3 -resolution 1 -tickinterval $Camera_Bias_Max \
	    -length 100

	button $camera_page.cam_bias.apply_$k -text "Apply"  -state disabled \
	    -command "
		camera_bias_set $k \$Camera_Bias($k) $camera_page
	    "
    }
#
# Pack the label
#
    set i 0
    grid config $camera_page.cam_bias.label \
	-row $i -column 0 -columnspan 8 -sticky ew
    incr i
#
# Pack in two rows and two columns
#
    set j 0
    grid config $camera_page.cam_bias.label_2 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.value_2 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.slider_2 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.apply_2 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.label_3 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.value_3 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.slider_3 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.apply_3 \
	-row $i -column $j
    incr i

    set j 0
    grid config $camera_page.cam_bias.label_0 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.value_0 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.slider_0 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.apply_0 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.label_1 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.value_1 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.slider_1 \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.apply_1 \
	-row $i -column $j
    incr i
#
# Buttons to set biases
#
    label $camera_page.cam_bias.set_sliders -text "Set Sliders to:" -bg cyan
    button $camera_page.cam_bias.current -text "Current" \
	-command {
	    camera_current_bias $camera_page
	}
    button $camera_page.cam_bias.default -text "Default" \
	-command {
	    camera_default_bias $camera_page
	}
    set j 0
    grid config $camera_page.cam_bias.set_sliders \
	-row $i -column $j -columnspan 2
    incr j
    incr j
    grid config $camera_page.cam_bias.current \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_bias.default \
	-row $i -column $j

# duplicate server/power buttons

    label $camera_page.cam_servers.label -text "Servers/Power" \
        -bg cyan -width 20

#
# PCR Process
#
    set button_size 0.20i
    label $camera_page.cam_servers.pcr_label \
        -text "PCR"
    button $camera_page.cam_servers.pcr_start \
        -background red -activebackground OrangeRed \
        -padx 0 -pady 0 -height $button_size -width $button_size -bitmap @$BitMaps_Dir/solid \
        -command { 
            process_start pcr $camera_page
         }

    label $camera_page.cam_servers.wfsc_label \
        -text "WFSC"
    button $camera_page.cam_servers.wfsc_start \
        -background red -activebackground tomato \
        -padx 0 -pady 0 -height $button_size -width $button_size -bitmap @$BitMaps_Dir/solid \
        -command { 
            topbox_power "WFSC" $camera_page
        }

    set i 0
    grid config $camera_page.cam_servers.label \
	-row $i -column 0 -columnspan 2 -sticky ew
    incr i
    set j 0
    grid config $camera_page.cam_servers.wfsc_label \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_servers.wfsc_start \
	-row $i -column $j
    incr i
    set j 0
    grid config $camera_page.cam_servers.pcr_label \
	-row $i -column $j
    incr j
    grid config $camera_page.cam_servers.pcr_start \
	-row $i -column $j

#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $camera_page.cam_bias \
	-side bottom -anchor w
    pack $camera_page.cam_size \
	-side left -anchor nw
    pack $camera_page.cam_rate \
	-side left -anchor n
    pack $camera_page.cam_run \
	-side left -anchor nw
    pack $camera_page.cam_servers \
	-side left -anchor nw

    camera_current_bias $camera_page

    if { $Info(WFSC_Source) == "Test" } {
	$camera_page.cam_source.test configure -disabledforeground black -bg green
	$camera_page.cam_source.cam configure -disabledforeground grey -bg grey84
    } elseif { $Info(WFSC_Source) == "Camera"} {
	$camera_page.cam_source.cam configure -disabledforeground black -bg green
	$camera_page.cam_source.test configure -disabledforeground grey -bg grey84
    }

    if { $Info(WFSC_Size) == "Full" } {
	$camera_page.cam_size.full configure -disabledforeground black -bg green
	$camera_page.cam_size.bin configure -disabledforeground grey -bg grey84
    } elseif { $Info(WFSC_Size) == "Bin" } {
	$camera_page.cam_size.bin configure -disabledforeground black -bg green
	$camera_page.cam_size.full configure -disabledforeground grey -bg grey84
    }


}
