###################################
#
# bg_zero.tcl
#
###################################
#
# Set the background to 0 in the SSC's.  This is done
#   by loading background of 0 from file.
#
# Written  7dec06  DLM Started with background_zero.tcl
#

proc bg_zero { fileName parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/bg/bg_globals.tcl

    global reply_data

#    set status [catch { AOsendWFCcmd SN_LoadBackground $fileName.dat } msg ]
    set status 0
    if { $status } {
	tk_messageBox -message "Background NOT loaded: $msg" \
	    -parent parent_win -icon error -type ok
	return -1
    } else {
	set BG_File [ file tail $fileName ]
	set BG_Base [ file dirname $fileName ]
	set status [catch {AOsendWFCcmd SInfo_PutFileName background_file $fileName } msg]
	set BG_Taken 0
    }

    return 0
	
}
