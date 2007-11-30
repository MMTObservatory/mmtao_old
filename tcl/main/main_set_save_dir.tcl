###################################
#
# main_set_save_dir.tcl
#
###################################
#
# Procedure to set and create save directory
#
# Written   24may04    DLM
#
proc main_set_save_dir { parent_win } {

    global PCR_HOME
    global USER_HOME
    global MMTAO_DATA_DIR

    global reply_data
    global Data_Dir
#
# Set the directory to save data.
#
#    set status [catch "AOsendWFCcmd UT_Time " msg ]
#    if { $status } {
#	tk_messageBox -message $msg -parent $parent_win -icon error -type ok
#    }
#
#    set ut_date [lindex $reply_data 0]
#    set ut_date 070126
#    set Data_Dir ${MMTAO_DATA_DIR}/test/$ut_date

    if { ![file exists $MMTAO_DATA_DIR] } {
	exec mkdir -p $MMTAO_DATA_DIR
	exec chmod ugo+w $MMTAO_DATA_DIR
    }
    puts "Data will be saved to $MMTAO_DATA_DIR"

}
