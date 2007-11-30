################################################## 
#
# seeing_page.tcl
#
####
#
# Create window to deal with seeing stuff.
#
# Written  7apr06 DLM Started with system_page.tcl
#
##################################################

proc seeing_page { seeing_win page } {

    global PCR_HOME
#
    upvar #0 Info Info
#
    global reply_data
#
# Create the tix NoteBook frame
#
    global seeing_page
    set seeing_page [ $seeing_win subwidget $page ]
#
# Build the page
#
    frame $seeing_page.test -relief ridge -border 4
#
# 
#
    button $seeing_page.test.set_button -text "Get Seeing Average" \
	-command {
	    seeing_get_average $seeing_page
	}
    label $seeing_page.test.cur_label -text "Current Seeing"
    entry $seeing_page.test.cur_value -textvar Info(Seeing_Value)
    label $seeing_page.test.avg_label -text "Average Seeing"
    entry $seeing_page.test.avg_value -textvar Seeing_Average
    label $seeing_page.test.gain_label -text "Seeing Gain"
    entry $seeing_page.test.gain_value -textvar Seeing_Gain
    checkbutton $seeing_page.test.seeing_auto -text "Average Seeing" -var Seeing_Auto \
	-command {
	    if { $Seeing_Auto } {
		seeing_looper $seeing_page
	    }
	}
    checkbutton $seeing_page.test.gain_auto -text "Automatic Gain" -var Gain_Auto
#
# Put this panel together
#
    set i 0
    grid config $seeing_page.test.set_button \
	-row $i -column 0 -columnspan 2 
    incr i
    grid config $seeing_page.test.cur_label \
	-row $i -column 0
    grid config $seeing_page.test.cur_value \
	-row $i -column 1
    incr i
    grid config $seeing_page.test.avg_label \
	-row $i -column 0
    grid config $seeing_page.test.avg_value \
	-row $i -column 1
    incr i
    grid config $seeing_page.test.gain_label \
	-row $i -column 0
    grid config $seeing_page.test.gain_value \
	-row $i -column 1
    incr i
    grid config $seeing_page.test.seeing_auto \
	-row $i -column 0 -columnspan 2
    incr i
    grid config $seeing_page.test.gain_auto \
	-row $i -column 0 -columnspan 2
    incr i
#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $seeing_page.test \
	    -side top -anchor w

}
