################################################## 
#
# display_page.tcl
#
####
#
# Create window to run the realtime displays
#
# Written  7jan03 DLM 
#
##################################################

proc display_page { display_win page } {

    global PCR_HOME
    source $PCR_HOME/tcl/display/display_globals.tcl
    source $PCR_HOME/tcl/dclient/dclient_globals.tcl
#
    upvar #0 Info Info
#
    global reply_data
#
# Create the tix NoteBook frame
#
    global display_page
    set display_page [ $display_win subwidget $page ]
#
    frame $display_page.status -relief ridge -border 4
#
#---------------------------------------------------
#
# Data Statistics
#
#---------------------------------------------------
#
    label $display_page.status.receiving -text "Receiving" -bg cyan
    label $display_page.status.rate -text "Rate" -bg cyan

    label $display_page.status.wfsc -text "WFSC" -bg cyan
    label $display_page.status.dds -text "DDS" -bg cyan

    label $display_page.status.wfsc_flow -textvar Info(PCR_WFSC_Receiving_Data) \
	-width 5
    label $display_page.status.dds_flow -textvar Info(PCR_DDS_Receiving_Data) \
	-width 5
#
    label $display_page.status.wfsc_rate -textvar Info(PCR_WFSC_Rate) \
	-width 5
    label $display_page.status.dds_rate -textvar Info(PCR_DDS_Rate) \
	-width 5
#
    set i 0
    set j 1
    grid $display_page.status.receiving \
	-row $i -column $j -sticky ew
    incr j
    grid $display_page.status.rate \
	-row $i -column $j -sticky ew
    incr j

    incr i
    set j 0
    grid $display_page.status.wfsc \
	-row $i -column $j -sticky ew
    incr j
    grid $display_page.status.wfsc_flow \
	-row $i -column $j
    incr j
    grid $display_page.status.wfsc_rate \
	-row $i -column $j

    incr i
    set j 0
    grid $display_page.status.dds \
	-row $i -column $j -sticky ew
    incr j
    grid $display_page.status.dds_flow \
	-row $i -column $j
    incr j
    grid $display_page.status.dds_rate \
	-row $i -column $j
#
############################################
# Create area for the Spots display
############################################
#
    frame $display_page.slopes -relief ridge -border 4
    frame $display_page.slopes.controls
#
#==============
# Spots Immage
#==============
#
# Create frame for pgplot
#
    pgplot $display_page.slopes.image -share true \
	-mincolors 25 -maxcolors 128 -width 260 -height 260 \
	-bg black -fg white
#
# Build Slopes diplay area
#
    pgplot_open Slopes $display_page.slopes.image/xtk $display_page
#
#==============
# Controls
#==============
#
# Update Display
#
    label $display_page.slopes.controls.display_title -text "Display Slopes" -bg cyan \
	-width 17
    label $display_page.slopes.controls.running -textvar Slope_Symbol -width 5
    button $display_page.slopes.controls.single -text "Single" -width 8\
	-command {
	    set Display_RT(Slopes) 0
	    display_slopes Slopes 1 0 $display_page
	}
    checkbutton $display_page.slopes.controls.realtime -var Display_RT(Slopes) \
	-text "Realtime" \
	-command {
	    display_slopes Slopes 1 1 $display_page
	}
#
    label $display_page.slopes.controls.type_title -text "Slopes Type" -bg cyan \
	-width 15
    radiobutton $display_page.slopes.controls.current -var Slopes_Type \
	-value 0 -text "Current"
    radiobutton $display_page.slopes.controls.average -var Slopes_Type \
	-value 1 -text "Average"
#
    label $display_page.slopes.controls.arrow_title -text "Slope Arrows" -bg cyan \
	-width 18
    checkbutton $display_page.slopes.controls.tip_tilt -var Slopes_Arrow \
	-text "Average Tip/Tilt"
#
# Pack the controls
#
    set i 0
    set j 0
    grid $display_page.slopes.controls.display_title \
	-row $j -column $i
    incr j
    grid $display_page.slopes.controls.running \
	-row $j -column $i
    incr j
    grid $display_page.slopes.controls.single \
	-row $j -column $i
    incr j
    grid $display_page.slopes.controls.realtime \
	-row $j -column $i
    incr j
    grid $display_page.slopes.controls.type_title \
	-row $j -column $i
    incr j
    grid $display_page.slopes.controls.current \
	-row $j -column $i -sticky w
    incr j
    grid $display_page.slopes.controls.average \
	-row $j -column $i -sticky w
    incr j
    grid $display_page.slopes.controls.arrow_title \
	-row $j -column $i
    incr j
    grid $display_page.slopes.controls.tip_tilt \
	-row $j -column $i -sticky w
    incr j
#
#==============
# Pack it all
#==============
#
    pack $display_page.slopes.image \
	$display_page.slopes.controls -side left
#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $display_page.status \
	$display_page.slopes \
	-side left -anchor w

}
