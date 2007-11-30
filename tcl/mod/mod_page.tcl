################################################## 
#
# mod_page.tcl
#
####
#
# Create window to apply modulated slope offsets
#
# Written  9jan06  DLM  Started with static_page.tcl
#
##################################################

proc mod_page { mod_win page } {

    global PCR_HOME
    source $PCR_HOME/tcl/mod/mod_globals.tcl
#
    upvar #0 Info Info
#
    global reply_data
#
# Create the tix NoteBook frame
#
    global mod_page
    set mod_page [ $mod_win subwidget $page ]
#
# Build the two column page
#
    frame $mod_page.modes -relief ridge -border 4
    frame $mod_page.rate -relief ridge -border 4
#
# Create mode column
#
    radiobutton $mod_page.modes.radio_none -text "None" \
	-variable Mod_Offset_Value -value "None" -anchor w \
	-command {
	    mod_offset_send $mod_page
	}
#
    pack $mod_page.modes.radio_none
#   
    for { set i 0 } { $i < 5 } { incr i } {
#
	radiobutton $mod_page.modes.radio_$i -text "Mode $i" \
	    -variable Mod_Offset_Value -value $i -anchor w \
	    -command {
		mod_offset_send $mod_page
	    }
#
	pack $mod_page.modes.radio_$i

    }
#
# Build rate column
#
    label $mod_page.rate.label -text "Rate" -bg cyan
    pack $mod_page.rate.label -anchor center -fill x

    foreach el $Mod_Rate_List {
#
	radiobutton $mod_page.rate.radio_$el -text "$el" \
	    -variable Mod_Rate_Value -value $el -anchor w \
	    -command {
		mod_rate_send $mod_page
	    }
		
#
	pack $mod_page.rate.radio_$el
#
    }
#
#########################
#
    frame $mod_page.controls -relief ridge -border 4
#
    label $mod_page.controls.label -text "Not Modulating" \
	 -relief raised -border 4
#
    button $mod_page.controls.start -text "Start Modulating" \
	-bg green \
	-command {
	    mod_start_stop $mod_page
	}
#
    pack $mod_page.controls.label \
	    $mod_page.controls.start \
	    -side left -anchor w
#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $mod_page.modes \
	$mod_page.rate \
	$mod_page.controls \
	    -side left -anchor w

}
