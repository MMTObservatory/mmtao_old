###################################
#
# pcr_save_data.tcl
#
###################################
#
# Request the PCR DM Server to save data
#
# Written  4sep06  Started with pcr_dm_stop.tcl
#
# Altered 31jan07  DLM Changed to work with AO GUI
#

proc pcr_save_data { value parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/pcr/pcr_globals.tcl

    global reply_data

    if { $value } {

	set status [catch { PCR_Cmd DM_Cmd Save_Data_On } msg ]

    } else {

	set status [catch { PCR_Cmd DM_Cmd Save_Data_Off } msg ]

    }	

    return 0
}
