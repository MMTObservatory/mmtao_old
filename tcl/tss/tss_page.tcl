################################################## 
#
# tss_page.tcl
#
####
#
# Create window to deal with tss stuff.
#
# 2008-11-03 skip
#
##################################################

proc tss_page { tss_win page } {

#
    upvar #0 TSS TSS
#
#
# Create the tix NoteBook frame
#
    global tss_page
    set tss_page [ $tss_win subwidget $page ]
#
# Build the page
#
    frame $tss_page.data1 -relief ridge -border 4
#
# 
#
    label $tss_page.data1.l1 -text "total current"
    label $tss_page.data1.v1 -textvar TSS(total_current) -bg grey84 -relief sunken -width 10

    proc tss_set {variable value} {
	set string "set\n$variable\n$value"
        set status [catch { TSS_Cmd send $string} msg ]
    }
#
# Put this panel together
#
    set i 0
    grid config $tss_page.data1.l1 -row $i -column 0
    incr i
    grid config $tss_page.data1.v1 -row $i -column 0
    incr i
#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $tss_page.data1 -side top -anchor w

}
