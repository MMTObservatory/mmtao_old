################################################## 
#
# bg_page.tcl
#
####
#
# Create window to aquire background, write it to file,
#   and load into the SSC
#
# Written    2001 DLM
#
# Altered  6jan02 DLM Started with Background_Image.tcl
#                     Added call to SN_LoadBackground.c to load
#                       the new aquired, or old, background into
#                       the SSC via a SwiftNet call.
#
# Altered 13apr06 DLM Cleaned up.  Made it into a Tix page
#
# Altered  7dec06 DLM Started with background_page.tcl
#
##################################################

proc bg_page { back_win page } {

    global PCR_HOME
    source $PCR_HOME/tcl/bg/bg_globals.tcl
    source $PCR_HOME/tcl/bitmaps/bitmaps_globals.tcl
    global MMTAO_DATA_DIR

    global reply_data
    global Data_Dir

    set Data_Dir $MMTAO_DATA_DIR
#
# Make Info array available
#
    upvar #0 Info Info
#
# Create the tix NoteBook frame
#
    global bg_page
    set bg_page [ $back_win subwidget $page ]
#
    global dark_display dark_num
    set dark_display 1
    set dark_num 1000

#
# Call C routine to read current time.  The time, in the format hh_mm_ss,
#   may be included, at the request of the user, as a suffix to the
#   dark file name.
#
    set status [catch { Utility_Cmd UT_Time } msg ]
    if { $status } {
	tk_messageBox -message $msg -parent $bg_page -icon error -type ok
    }
    set current_time [lindex $reply_data 1]
#
# Set the local display and utility variables
#
    global New_BG_Base
    global New_BG_File
    set New_BG_Base $Data_Dir
    set New_BG_File "dark_${current_time}"

    global Load_BG_Base
    global Load_BG_File
    set Load_BG_Base $Data_Dir
    set Load_BG_File ""
#
# Determine the current background (if it exists)
#
    set BG_Base [ file dirname $Info(Background_File)]
    set BG_File [ file tail $Info(Background_File)]
#
# Build window
#
    frame $bg_page.full -relief ridge -border 4
    frame $bg_page.dynamic -relief ridge -border 4
#
# Dynamic Background Subtraction
#
    label $bg_page.dynamic.title -text "Dynamic Background" -bg cyan
    button $bg_page.dynamic.on_off \
	-text "\nDynamic Subtraction is\nOFF\n" \
	-bg grey84 -activebackground grey84 \
	-command { 
	    bg_dynamic_on_off $bg_page
	}
#
# Pack the Dynamic Stuff
#    
    pack $bg_page.dynamic.title -fill x \
	-side top
    pack $bg_page.dynamic.on_off
#
# Current Static Background Subtraction
#
    frame $bg_page.full.curr

    label $bg_page.full.curr.title -text "Current Static Background Setting" -bg cyan

    label $bg_page.full.curr.label -text "Loaded Background File"

    label $bg_page.full.curr.value -width 20 -relief sunken \
	    -textvar BG_Current

    button $bg_page.full.curr.display -text "Display Background" \
	-command {
	    if { [string compare Zero_Background $BG_Current ] != 0 } {
		display_wfsc_fits $Info(Background_File).fits 0 $bg_page
	    }
	}

    button $bg_page.full.curr.on_off \
	-text "\nBackground Subtraction is\nOFF\n" \
	-bg grey84 -activebackground grey84 \
	-command { 
	    bg_on_off $bg_page
	}

    set i 0
    set j 0
    grid $bg_page.full.curr.title \
	-row $i -column $j -columnspan 3 -sticky ew
    incr i

    set j 0
    grid $bg_page.full.curr.label \
	-row $i -column $j -sticky e
    incr j
    grid $bg_page.full.curr.value \
	-row $i -column $j -sticky ew
    incr j
    grid $bg_page.full.curr.display \
	-row $i -column $j -sticky e
    incr i

    set j 1
    grid $bg_page.full.curr.on_off \
	-row $i -column $j -sticky ew
#
#
#
    frame $bg_page.full.new
#
    frame $bg_page.full.new.acquire -relief ridge -border 4
#
    frame $bg_page.full.new.acquire.line -relief ridge -height 1m -border 3 -bg black
    label $bg_page.full.new.acquire.label -text "Acquire Background"
#
    label $bg_page.full.new.acquire.num_label -text "Total Number of Frames" \
	    -bg green -anchor w
    entry $bg_page.full.new.acquire.num_entry -width 5 -relief sunken \
	    -textvar dark_num -bg white
#
    label $bg_page.full.new.acquire.name_label -text "Save File Name" \
	    -bg yellow -anchor w
    entry $bg_page.full.new.acquire.name_entry -width 15 -relief sunken \
	    -textvar New_BG_File -bg white
#
# Check to see if the file containing the requested dark frame exists and
#   the appropropriate flag.
#
    set dark_exists [file exists [file join $BG_Base $BG_File ] ]
    if { $dark_exists } {
	$bg_page.full.new.acquire.name_label configure -bg green
	$bg_page.full.new.acquire.name_entry configure -state normal
    }

    button $bg_page.full.new.acquire.aquire -text "Aquire" \
	-command {
	    set error [ bg_acquire $dark_num $dark_display \
				    $New_BG_Base $New_BG_File $bg_page ]
	    if { $error } {
		$bg_page.full.new.acquire.name_label configure -bg yellow
		$bg_page.full.new.acquire.load configure -state disabled -bg grey84
		$bg_page.full.new.acquire.display configure -state disabled
	    } else {
		$bg_page.full.new.acquire.name_label configure -bg green
		$bg_page.full.new.acquire.load configure -state normal -bg green -activebackground lightgreen
		$bg_page.full.new.acquire.display configure -state normal
	    }
	}

    button $bg_page.full.new.acquire.load -text "Load" \
	-state disabled \
	-command {
	    set fileName [file join $New_BG_Base $New_BG_File ]
	    set error [ catch { bg_load $fileName $bg_page } msg ]
	    if { !$error } {
		$bg_page.full.new.read.load configure -state disabled
		set linkName [file join $New_BG_Base dark.default ]
		set status [catch { exec rm -f $linkName } msg ]
		if { [string compare *Zero_Background* ${Load_BG_File} ] != 0 } {
		    set linkName [file join $New_BG_Base dark.default ]
		    set fileName [file join $New_BG_Base $New_BG_File.dat ]
		    set status [catch { exec ln -s $fileName $linkName } msg ]
		    if { $status } {
			tk_messageBox -message "Background Removal NOT set: $msg" \
			    -parent $bg_page -icon error -type ok
		    }
		}
	    }
	    $bg_page.full.new.acquire.name_label configure -bg yellow
	    $bg_page.full.new.acquire.load configure -state disabled -bg grey84
	    $bg_page.full.new.acquire.display configure -state disabled
#
# Call C routine to read current time.  The time, in the format hh_mm_ss,
#   may be included, at the request of the user, as a suffix to the
#   dark file name.
#
	    set status [catch { Utility_Cmd UT_Time } msg ]
	    if { $status } {
		tk_messageBox -message $msg -parent $bg_page -icon error -type ok
	    }
	    set current_time [lindex $reply_data 1]
	    set New_BG_Base $Data_Dir
	    set New_BG_File "dark_${current_time}"
	}

    button $bg_page.full.new.acquire.display -text "Display" \
	-state disabled \
	-command {
	    set fileName [file join $New_BG_Base $New_BG_File ]
	    display_wfsc_fits $fileName.fits 0 $bg_page
	}

# Entry fields

    set i 0
    grid config $bg_page.full.new.acquire.line \
	-row $i -column 0 -columnspan 3 -sticky ew
    grid config $bg_page.full.new.acquire.label \
	-row $i -column 0 -columnspan 3
    incr i
#    
    grid config $bg_page.full.new.acquire.num_entry \
	-row $i -column 0 -sticky e
    grid config $bg_page.full.new.acquire.num_label \
	-row $i -column 1 -columnspan 2 -sticky ew
    incr i

    grid config $bg_page.full.new.acquire.name_entry \
	-row $i -column 0 -sticky e
    grid config $bg_page.full.new.acquire.name_label \
	-row $i -column 1 -columnspan 2 -sticky ew
    incr i

    grid config $bg_page.full.new.acquire.aquire \
	    -row $i -column 0
    grid config $bg_page.full.new.acquire.load \
	    -row $i -column 1
    grid config $bg_page.full.new.acquire.display \
	    -row $i -column 2
    incr i

# Read background file from disk

    frame $bg_page.full.new.read -relief ridge -border 4
#
    frame $bg_page.full.new.read.line -relief ridge -height 1m -border 3 -bg black
    label $bg_page.full.new.read.label -text "Load Background From File"
#
    button $bg_page.full.new.read.dark_zero -text "Zero Background" \
	-command {
	    set Load_BG_Base [file dirname $BG_Zero_File]
	    set Load_BG_File [file tail $BG_Zero_File]
	    $bg_page.full.new.read.load configure -state normal -bg green -activebackground lightgreen
	    $bg_page.full.new.read.display configure -state normal
	}

#
# Get from the WFC the background filename currently loaded into the SSC
#
    entry $bg_page.full.new.read.dark_entry -width 15 -relief sunken \
	    -textvar Load_BG_File
    button $bg_page.full.new.read.dark_browse -text "Browse" \
	-command {
	    set Load_BG_Base $Data_Dir
	    set status [browse_file \
			    "BG File" \
			    { "BG File" { .dat }} \
			    Load_BG_Base \
			    Load_BG_File \
			    $bg_page ]
	    if { $status } {
		set Load_BG_Base $Data_Dir
		set Load_BG_File ""
	        $bg_page.full.new.read.load configure -state disabled -bg grey84
	        $bg_page.full.new.read.display configure -state disabled
	    } else {
		
		set Load_BG_File [string trimright $Load_BG_File ".dat"]
	        $bg_page.full.new.read.load configure -state normal -bg green -activebackground lightgreen
	        $bg_page.full.new.read.display configure -state normal
	    }
	}

    button $bg_page.full.new.read.load -text "Load" \
	-state disabled \
	-command {
	    set fileName [file join $Load_BG_Base $Load_BG_File ]
	    set error [ catch { bg_load $fileName $bg_page } msg ]
	    if { !$error } {
		$bg_page.full.new.read.load configure -state disabled
		set linkName [file join $Data_Dir dark.default ]
		set status [catch { exec rm -f $linkName } msg ]
		if { [string compare Zero_Background ${Load_BG_File} ] != 0 } {
		    set linkName [file join $Data_Dir dark.default ]
		    set fileName [file join $Load_BG_Base $Load_BG_File.dat ]
		    set status [catch { exec ln -s $fileName $linkName } msg ]
		    if { $status } {
			tk_messageBox -message "Background Removal NOT set: $msg" \
			    -parent $bg_page -icon error -type ok
		    }
		}
	    }
	    set Load_BG_Base $Data_Dir
	    set Load_BG_File ""
	    $bg_page.full.new.read.load configure -state disabled -bg lightgrey
	    $bg_page.full.new.read.display configure -state disabled
	}
#
    button $bg_page.full.new.read.display -text "Display" \
	-state disabled \
	-command {
	    if { [string compare Zero_Background $Load_BG_File ] != 0 } {
		set fileName [file join $Load_BG_Base $Load_BG_File ]
		display_wfsc_fits $fileName.fits 0 $bg_page
	    }
	}
# Start button

    set i 0
    grid config $bg_page.full.new.read.line \
	-row $i -column 0 -columnspan 3 -sticky ew
    grid config $bg_page.full.new.read.label \
	-row $i -column 0 -columnspan 3
    incr i

    grid config $bg_page.full.new.read.dark_entry \
		-row $i -column 0 -rowspan 2
    grid config $bg_page.full.new.read.dark_browse \
		-row $i -column 1 -sticky w
    incr i

    grid config $bg_page.full.new.read.dark_zero \
		-row $i -column 1
    incr i

    grid config $bg_page.full.new.read.load \
	    -row $i -column 0 -sticky e
    grid config $bg_page.full.new.read.display \
	    -row $i -column 1 -sticky w
    incr i

    pack $bg_page.full.new.read \
	    $bg_page.full.new.acquire \
	    -side left -anchor n

    pack $bg_page.full.curr \
	    $bg_page.full.new \
	    -side top

    pack $bg_page.full \
	$bg_page.dynamic \
	-side left
}


