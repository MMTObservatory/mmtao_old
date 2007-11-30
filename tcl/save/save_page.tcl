################################################## 
#
# save_page.tcl
#
####
#
# Create window to run the realtime displays
#
# Written  7jan03 DLM 
#
##################################################

proc save_page { save_win page } {

    global PCR_HOME
    source $PCR_HOME/tcl/save/save_globals.tcl
    source $PCR_HOME/tcl/dclient/dclient_globals.tcl
#
    upvar #0 Info Info
#
    global MMTAO_DATA_DIR
    global Data_Dir
    set Data_Dir $MMTAO_DATA_DIR

    global reply_data
#
# Create the tix NoteBook frame
#
    global save_page
    set save_page [ $save_win subwidget $page ]
#
# Call C routine to read current time.  The time, in the format hh_mm_ss,
#   may be included, at the request of the user, as a suffix to the
#   dark file name.
#
    set status [catch { Utility_Cmd UT_Time } msg ]
    if { $status } {
	tk_messageBox -message $msg -parent $save_page -icon error -type ok
    }
    set current_time [lindex $reply_data 1]
#
    frame $save_page.wfsc -relief ridge -border 4
    frame $save_page.dds -relief ridge -border 4
    frame $save_page.pcr -relief ridge -border 4
#
############################################
#
# Make header labels for saving WFSC date
#
############################################
#
    label $save_page.wfsc.label -text "Save WFSC Data" -bg cyan -width 18
#
    set i 0
    set j 0
    grid $save_page.wfsc.label \
	-row $i -column $j -columnspan 2 -sticky ew
    incr i
#
############################################
#
# Make save button for WFSC date
#
############################################
#
    label $save_page.wfsc.frame_label -text "Frames" -bg green
#
    entry $save_page.wfsc.frame_entry -textvariable DClient_WFSC_Frames \
	-width 5 -justify right -bg white
#
    label $save_page.wfsc.name_label -text "Save File Name" \
	    -bg yellow -anchor w
    entry $save_page.wfsc.name_entry -width 15 -relief sunken \
	    -textvar WFSC_File -bg white
#
    set j 0
    grid $save_page.wfsc.frame_label \
	-row $i -column $j -sticky e
    incr j
    grid $save_page.wfsc.frame_entry \
	-row $i -column $j -sticky w
    incr i
#
    set j 0
    grid $save_page.wfsc.name_label \
	-row $i -column $j -sticky e
    incr j
    grid $save_page.wfsc.name_entry \
	-row $i -column $j -sticky w
    incr i
#
    button $save_page.wfsc.save -text "Save" -bg grey84 \
	-command {
	    dclient_wfsc_save $DClient_WFSC_Frames $Data_Dir $WFSC_Time $save_page
	    set status [catch { Utility_Cmd UT_Time } msg ]
	    if { $status } {
		tk_messageBox -message $msg -parent $save_page -icon error -type ok
	    }
	    set current_time [lindex $reply_data 1]
	    set WFSC_Time ${current_time}
	    set WFSC_File "wfsc_${current_time}.fits"
	}
    set j 0
    grid $save_page.wfsc.save \
	-row $i -column $j -columnspan 2
    incr i
#
############################################
#
# Make header labels for saving DDS data
#
############################################
#
    label $save_page.dds.label -text "Save Loop Data" -bg cyan -width 23
#
    set i 0
    set j 0
    grid $save_page.dds.label \
	-row $i -column $j -columnspan 2 -sticky ew
    incr i
#
############################################
#
# Make save button for DDS data
#
############################################
#
    set j 0
    foreach el $DClient_DM_Save_List {
	checkbutton $save_page.dds.check_$el -text $el -var DClient_DM_Save($el)
	if { $j == 2 } {
	    set j 0
	    incr i
	}
	grid $save_page.dds.check_$el \
	    -row $i -column $j -sticky w
	incr j
    }
    incr i

    button $save_page.dds.check_all -textvar DClient_All \
	-width 10 \
	-command {
	    if { [string compare $DClient_All "Check All"] == 0 } {
		foreach el $DClient_DM_Save_List {
		    set DClient_DM_Save($el) 1
		}
		set DClient_All "Uncheck All"
	    } elseif { [string compare $DClient_All "Uncheck All"] == 0 } {
		foreach el $DClient_DM_Save_List {
		    set DClient_DM_Save($el) 0
		}
		set DClient_All "Check All"
	    } else {
		puts "Unknown lable $DClient_All"
	    }
	}
		
    set j 0
    grid $save_page.dds.check_all \
	-row $i -column $j -columnspan 2
    incr i

    label $save_page.dds.frame_label -text "Frames" -bg green

    entry $save_page.dds.frame_entry -textvariable DClient_DDS_Frames \
	-width 5 -justify right -bg white

#
    label $save_page.dds.name_label -text "Save File Name" \
	    -bg yellow -anchor w
    entry $save_page.dds.name_entry -width 15 -relief sunken \
	    -textvar DDS_File -bg white
#
    set j 0
    grid $save_page.dds.frame_label \
	-row $i -column $j -sticky e
    incr j
    grid $save_page.dds.frame_entry \
	-row $i -column $j -sticky w
    incr i
    set j 0
    grid $save_page.dds.name_label \
	-row $i -column $j -sticky e
    incr j
    grid $save_page.dds.name_entry \
	-row $i -column $j -sticky e
    incr i
#
    button $save_page.dds.save -text "Save" -bg grey84 \
	-command {
	    dclient_dm_save $DClient_DDS_Frames $Data_Dir $DDS_Time $save_page
	    set status [catch { Utility_Cmd UT_Time } msg ]
	    if { $status } {
		tk_messageBox -message $msg -parent $save_page -icon error -type ok
	    }
	    set current_time [lindex $reply_data 1]
	    set DDS_Time ${current_time}
	    set DDS_File "dds_${current_time}.fits"
	}
    set j 0
    grid $save_page.dds.save \
	-row $i -column $j -columnspan 2
    incr i
#
# Save loop data to file (done by the PCR)
#
    checkbutton $save_page.pcr.save -text "Save PCR Data" \
	-var Save_PCR_Data \
	-command {
	    pcr_save_data $Save_PCR_Data $save_page
	}
    grid $save_page.pcr.save \
	-row 0 -column 0
#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $save_page.wfsc \
	$save_page.dds \
	$save_page.pcr \
	-side left -anchor w

}
