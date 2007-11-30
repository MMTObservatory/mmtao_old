###################################
#
# gui_update.tcl
#
###################################
#
# This routine sets up the trace on several variables such that
#   when they are accessed, the gui is updated
#
# Variables:
#   Info_Update - set in info_get.tcl after all the Info variables
#                 have been set
#
###################################
#
# Info Variables
#
trace variable Info_Update w info_update_set
proc info_update_set { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    source $PCR_HOME/tcl/camera/camera_globals.tcl
    source $PCR_HOME/tcl/pcr/pcr_globals.tcl

    upvar #0 Info Info
#
# Check the status of the Camera and set label and buttons accordingly.
#   This check is a trace on Info(WFSC_Running) because it comes after
#   Info(Initialized) in InfoList so the most recent information is used
#   to setup the gui
#
# If the Main Window exists then update the Camera label
#
    if { [winfo exists $main_win] } {
        if { $Info(WFSC_Initialized) } {
	    if { $Info(WFSC_Running) } {
		#
		# Initialized and Running
		#
		$main_win.main.controls.cam_status configure \
		    -text "Camera is\nRUNNING\nat $Info(WFSC_Frequency) Hz" -bg green
		if { $Info(Loop_Running) } {
		    #
		    # Loop is Running
		    #
		    $main_win.main.controls.change_button configure \
			-state normal -text "LOOP IS\nCLOSED\n\nPress to Open" -bg red
		    $main_win.main.controls.gain_up configure -state normal
		    $main_win.main.controls.gain_down configure -state normal
		} else {
		    #
		    # Loop Ready but NON Running
		    #
		    $main_win.main.controls.change_button configure \
			-state normal -text "LOOP IS\nOPEN\n\nPress to Close" -bg yellow
		    $main_win.main.controls.gain_up configure -state disabled
		    $main_win.main.controls.gain_down configure -state disabled
		}
	    } else {
		#
		# Initialized but NOT Running
		#
		$main_win.main.controls.cam_status configure -text "Camera is\nSTOPPED" \
		    -bg yellow
		$main_win.main.controls.change_button configure \
		    -state disabled -text "LOOP IS\nOPEN" -bg yellow
		$main_win.main.controls.gain_up configure -state disabled
		$main_win.main.controls.gain_down configure -state disabled
	    }
	} else {
	    #
	    # Not Initialized
	    #
	    $main_win.main.controls.cam_status configure \
		-text "Camera is\nNOT Initialized" -bg yellow
	    $main_win.main.controls.change_button configure \
		-state disabled -text "Close AO Loop" -bg grey84
	}
        set PCR_Save_Data $Info(Loop_Save_Data)
#
# Check to see which Tix Notebook page is on top and set values on specific pages
#   accordingly.
#
#   Added 15jun07  DLM
#
	set Tix_Page [ $main_win.aux raised ]
    }
#
# If the Camera Page exists then update the camera control buttons
#
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $camera_page] ] ] } {
        if { $Info(WFSC_Initialized) } {
	    #
	    # Initialized
	    #
	    $camera_page.cam_size.label configure -bg cyan
	    $camera_page.cam_rate.label configure -bg cyan
	    $camera_page.cam_run.label configure -bg cyan
	    $camera_page.cam_bias.label configure -bg cyan
	    $camera_page.cam_rate.list configure -bg green
	    $camera_page.cam_bias.apply_0 configure -state normal
	    $camera_page.cam_bias.apply_1 configure -state normal
	    $camera_page.cam_bias.apply_2 configure -state normal
	    $camera_page.cam_bias.apply_3 configure -state normal
	    if { $Info(WFSC_Running) } {
		#
		# Initialized and Running
		#
		$camera_page.cam_run.start_stop configure -state normal \
		    -text "Running\nPress to Stop" -bg green
		$camera_page.cam_rate.list configure -state disabled \
		    -bg green
		$camera_page.cam_size.full configure -state disabled
		$camera_page.cam_size.bin configure -state disabled
		$camera_page.cam_source.test configure -state disabled
		$camera_page.cam_source.cam configure -state disabled
		$camera_page.cam_rate.list configure -state disabled
	    } else {
		#
		# Initialized but NOT Running
		#
		$camera_page.cam_run.start_stop configure -state normal \
		    -text "Stopped\nPress to Start" -bg yellow
		$camera_page.cam_size.full configure -state normal
		$camera_page.cam_size.bin configure -state normal
		$camera_page.cam_source.test configure -state normal
		$camera_page.cam_source.cam configure -state normal
		$camera_page.cam_rate.list configure -state normal
	    }
	} else {
	    #
	    # Not Initialized
	    #
	    $camera_page.cam_size.label configure -bg grey
	    $camera_page.cam_rate.label configure -bg grey
	    $camera_page.cam_run.label configure -bg grey
	    $camera_page.cam_size.full configure -state disabled
	    $camera_page.cam_size.bin configure -state disabled
	    $camera_page.cam_rate.list configure -state disabled -bg grey
	    $camera_page.cam_run.start_stop configure -state disabled
	    $camera_page.cam_bias.apply_0 configure -state disabled
	    $camera_page.cam_bias.apply_1 configure -state disabled
	    $camera_page.cam_bias.apply_2 configure -state disabled
	    $camera_page.cam_bias.apply_3 configure -state disabled
	}
    }
}
