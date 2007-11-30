###################################
#
# display_wfsc_fits.tcl
#
###################################
#
# Read the requested .fits file containing WFSC images and display the
#   requested frame
#
# Written  4aug06  DLM
#

proc display_wfsc_fits { fileName frameNumber parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/display/display_globals.tcl

    global reply_data

    set status [file exists $fileName ] 
    if { !$status } {
	tk_messageBox -message "Background file $fileName does not exists" \
	    -parent $parent_win -icon error -type ok
	return -1
    }

    set status [catch { DClient_Cmd WFSC Fits $fileName $frameNumber } msg ]
    set status 0
    if { $status } {
	tk_messageBox -message $reply_data \
	    -parent $parent_win -icon error -type ok
	return -1
    }

    return 0
	
}
