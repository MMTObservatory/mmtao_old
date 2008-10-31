################################################## 
#
# science_page.tcl
#
####
#
# Create window to deal with science stuff.
#
# 2008-10-29 skip
#
##################################################

proc science_page { science_win page } {

#
    upvar #0 Science Science
#
#
# Create the tix NoteBook frame
#
    global science_page
    set science_page [ $science_win subwidget $page ]
#
# Build the page
#
    frame $science_page.delay -relief ridge -border 4
#
# 
#
    set offset_closed_delay1 0
    set offset_closed_delay2 0
    set offset_closed_delay3 0
    set offset_closed_delay4 0
    set offset_closed_delay5 0

    label $science_page.delay.l0 -text "Closed offset delays (ms)" -bg cyan
    label $science_page.delay.l1 -text "Gain to 0.0"
    label $science_page.delay.delay1 -textvar Science(offset_closed_delay1) -bg grey84 -relief sunken -width 10
    entry $science_page.delay.e1 -textvariable offset_closed_delay1 -bg white -width 10
    bind $science_page.delay.e1 <Return> {science_set offset_closed_delay1 $offset_closed_delay1}
    label $science_page.delay.l2 -text "Move FSM"
    label $science_page.delay.delay2 -textvar Science(offset_closed_delay2) -bg grey84 -relief sunken -width 10
    entry $science_page.delay.e2 -textvariable offset_closed_delay2 -bg white -width 10
    bind $science_page.delay.e2 <Return> {science_set offset_closed_delay2 $offset_closed_delay2}
    label $science_page.delay.l3 -text "Move TCS"
    label $science_page.delay.delay3 -textvar Science(offset_closed_delay3) -bg grey84 -relief sunken -width 10
    entry $science_page.delay.e3 -textvariable offset_closed_delay3 -bg white -width 10
    bind $science_page.delay.e3 <Return> {science_set offset_closed_delay3 $offset_closed_delay3}
    label $science_page.delay.l4 -text "Gain to 0.5"
    label $science_page.delay.delay4 -textvar Science(offset_closed_delay4) -bg grey84 -relief sunken -width 10
    entry $science_page.delay.e4 -textvariable offset_closed_delay4 -bg white -width 10
    bind $science_page.delay.e4 <Return> {science_set offset_closed_delay4 $offset_closed_delay4}
    label $science_page.delay.l5 -text "Gain to original"
    label $science_page.delay.delay5 -textvar Science(offset_closed_delay5) -bg grey84 -relief sunken -width 10
    entry $science_page.delay.e5 -textvariable offset_closed_delay5 -bg white -width 10
    bind $science_page.delay.e5 <Return> {science_set offset_closed_delay5 $offset_closed_delay5}
    label $science_page.delay.l6 -text "Done sent to instrument"

    proc science_set {variable value} {
	set string "set\n$variable\n$value"
        set status [catch { Science_Cmd send $string} msg ]
    }
#
# Put this panel together
#
    set i 0
    grid config $science_page.delay.l0 -row $i -column 0 -columnspan 2
    incr i
    grid config $science_page.delay.l1 -row $i -column 0 -columnspan 2
    incr i
    grid config $science_page.delay.delay1 -row $i -column 0
    grid config $science_page.delay.e1 -row $i -column 1
    incr i
    grid config $science_page.delay.l2 -row $i -column 0 -columnspan 2
    incr i
    grid config $science_page.delay.delay2 -row $i -column 0
    grid config $science_page.delay.e2 -row $i -column 1
    incr i
    grid config $science_page.delay.l3 -row $i -column 0 -columnspan 2
    incr i
    grid config $science_page.delay.delay3 -row $i -column 0
    grid config $science_page.delay.e3 -row $i -column 1
    incr i
    grid config $science_page.delay.l4 -row $i -column 0 -columnspan 2
    incr i
    grid config $science_page.delay.delay4 -row $i -column 0
    grid config $science_page.delay.e4 -row $i -column 1
    incr i
    grid config $science_page.delay.l5 -row $i -column 0 -columnspan 2
    incr i
    grid config $science_page.delay.delay5 -row $i -column 0
    grid config $science_page.delay.e5 -row $i -column 1
    incr i
    grid config $science_page.delay.l6 -row $i -column 0 -columnspan 2
    incr i
#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $science_page.delay -side top -anchor w

}
