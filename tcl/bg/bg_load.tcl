###################################
#
# bg_load.tcl
#
###################################
#
# Load the requested background file into the PCR
#
# Written  7dec06  Started with backgound_load.tcl
#

proc bg_load { fileName parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/bg/bg_globals.tcl

    upvar #0 Info Info

    global reply_data

    set status [file exists $fileName.dat ]
    if { !$status } {
	tk_messageBox -message "Background file $fileName.dat does not exists" \
	    -parent $parent_win -icon error -type ok
	return -1
    }

    set status [catch { BG_Cmd Send $fileName.dat } msg ]
    if { $status } {
	tk_messageBox -message "Background NOT loaded" \
	    -parent $parent_win -icon error -type ok
	return -1
    } else {
	set BG_File [ file tail $fileName ]
	set BG_Base [ file dirname $fileName ]
	set status [catch { Info_Cmd Set Background_File $fileName } msg ]
	set BG_Taken 0
    }

    return 0
	
}
