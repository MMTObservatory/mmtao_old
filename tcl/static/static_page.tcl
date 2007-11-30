################################################## 
#
# static_page.tcl
#
####
#
# Create window to deal with static stuff.
#
# Written  7apr06 DLM Started with system_page.tcl
#
##################################################

proc static_page { static_win page } {

    global PCR_HOME
    source $PCR_HOME/tcl/static/static_globals.tcl
#
    upvar #0 Info Info
#
    global reply_data
#
# Create the tix NoteBook frame
#
    global static_page
    set static_page [ $static_win subwidget $page ]
#
# Build the page
#
    frame $static_page.modes -relief ridge -border 4
#
# Create two columns with labels
#
    set i 0
    set j 0
#
    foreach el $Static_List {
#
	if { $i == 0 } {
#
#      Label the sliders
#
	    label $static_page.modes.type_$j -text "Mode" -bg lightblue
	    label $static_page.modes.value_$j -text "Value" -bg lightblue
	    label $static_page.modes.units_$j -text "nanometers RMS" -bg lightblue
#
	    grid config $static_page.modes.type_$j \
		-row $i -column $j -sticky ew
	    grid config $static_page.modes.value_$j \
		-row $i -column [ expr $j+1 ] -sticky ew
	    grid config $static_page.modes.units_$j \
		-row $i -column [ expr $j+2 ] -sticky ew
	    incr i
	}
	    
	checkbutton $static_page.modes.check_$el -text [ string range $el 7 end ] \
	    -variable Static_Set($el) -onvalue 1 -offvalue 0 -anchor w -width 7

	label $static_page.modes.value_$el -textvar Info($el) -width 3 -bg green

	scale $static_page.modes.slider_$el -orient horizontal \
	    -from -$Static_Max($el) -to $Static_Max($el) \
	    -variable Static_Value($el) \
	    -digits 3 -resolution 5 -tickinterval $Static_Max($el) \
	    -state disabled -length [ expr $Static_Max($el)/1 ] \
	    -command { static_rms }

	if { $Info($el) } {
	    set Static_Set($el) 1
	    set Static_Value($el) $Info($el)
	} else {
	    set Static_Set($el) 0
	    set Static_Value($el) 0
	}
#
	grid config $static_page.modes.check_$el \
	    -row $i -column $j
	grid config $static_page.modes.value_$el \
	    -row $i -column [ expr $j+1 ]
	grid config $static_page.modes.slider_$el \
	    -row $i -column [ expr $j+2 ]
	incr i

	if { $i == 3 } {
	    set i 0
	    set j 3
	}
    }
#
#########################
#
    button $static_page.modes.current -text "Current" \
	-command {
	    foreach el $Static_List {
		if { $Info($el) } {
		    set Static_Set($el) 1
		    set Static_Value($el) $Info($el)
		} else {
		    set Static_Set($el) 0
		    set Static_Value($el) 0
		}
	    }
	}

    button $static_page.modes.display -text "Display" \
	-command {
	    static_display $Base $File $static_page
	}

    button $static_page.modes.apply -text "Apply" \
	-command {
	    static_apply $Base $File $static_page
	}

    frame $static_page.modes.show_rms

    entry $static_page.modes.show_rms.entry -textvar Static_RMS \
	-width 5
    label $static_page.modes.show_rms.label -text "Total RMS (nanometers)"

    pack $static_page.modes.show_rms.label \
	$static_page.modes.show_rms.entry \
	-side left -fill x 

    grid config $static_page.modes.current \
	-row 4 -column 0
#    grid config $static_page.modes.display \
#	-row 4 -column 1
    grid config $static_page.modes.apply \
	-row 4 -column 1
    grid config $static_page.modes.show_rms \
	-row 4 -column 2 -columnspan 2
    incr i

#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $static_page.modes \
	    -side top -anchor w

}
