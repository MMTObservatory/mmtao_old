################################################## 
#
# gain_page.tcl
#
####
#
# Create window to set the seeing dependent gain.
#
# Written  7apr06 DLM Started with system_page.tcl
#
##################################################

proc gain_page { gain_win page } {

    global PCR_HOME
    source $PCR_HOME/tcl/system/system_globals.tcl
#    source $PCR_HOME/tcl/info/info_globals.tcl
#
    upvar #0 Info Info
#
    global reply_data
#
# Create the tix NoteBook frame
#
    global win
    set win [ $gain_win subwidget $page ]
#
# Build the page
#
    frame $win.test -relief ridge -border 4
#
# 
#
    button $win.test.set_button -text "Set gain (Test)" \
	-command {
	    gain_set $win
	}
    entry $win.test.avg -textvar Seeing_Average
#
# Put this panel together
#
    set i 0
    grid config $win.test.set_button \
	-row $i -column 0
    incr i
    grid config $win.test.avg \
	-row $i -column 0
    incr i
#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $win.test \
	    -side top -anchor w

}
