###################################
#
# bg_acquire.tcl
#
###################################
#
# Acquire the requested number or WFSC frame, find the
#   average and standard deviation and make avaialable
#   to be used as a background (ie loaded into the SSC's
#   and subtracted form each frame)
#
# Written  7dec06  DLM Started with background_acquire.tcl
#

proc bg_acquire { bg_num bg_display New_BG_Base New_BG_File parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/bg/bg_globals.tcl

    global reply_data

    set fileName [file join $New_BG_Base $New_BG_File]

    set error [catch { BG_Cmd Acquire $bg_num $bg_display \
			    $fileName } msg ]
    if { $error } {
	tk_messageBox -message $reply_data \
	    -parent $parent_win -icon error -type ok
	return -1
    }

    return 0
	
}
