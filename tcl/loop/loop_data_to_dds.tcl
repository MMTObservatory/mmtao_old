################################################## 
#
# loop_data_to_dds.tcl
#
################################################## 
#
# Routine to send flag to VME to send loop data to the DDS
#
# Written  9apr06  DLM  Started with dds_save_data.tcl
#
##################################################

proc loop_data_to_dds { mask parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/info/info_globals.tcl
#
    upvar #0 Info Info
#
    global reply_data
#
    if { !$Info(Loop_Running) } {

	set status \
	    [tk_messageBox -parent $parent_win -icon question -type yesno \
		 -message "Loop is not running\n\nGo ahead and set DDS flag?" ]
	if { $status == "no" } {
	    return 0
	}
    }

    set status [catch {AOsendWFCcmd SN_SendDDScommand $mask } msg ]
    if { $status } {
	tk_messageBox -message \
	    "Error changing state of data storage\n$reply_data" \
	    -parent $parent_win \
	    -icon error -type ok
	set mask 0
    }
#
# Send value to Info server on VME
#
    set status [catch { Info_Cmd Set Loop_Data_To_DDS $mask } msg ]

    return 0
}
