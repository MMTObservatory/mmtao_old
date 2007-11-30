###################################
#
# tcs_looper.tcl
#
###################################
#
# Self calling prodecure to update TCS information on GUI
#
# Written   25mar04  DLM  Started with oap_looper.tcl
#
# Altered    2feb06  DLM  Change so the loop is infinite, but the call to retieve
#                           TCS information only occurs if we are connected to the
#                           TCS.
#

proc tcs_looper { parent_win } {
      
    global PCR_HOME
    source $PCR_HOME/tcl/tcs/tcs_globals.tcl
    source $PCR_HOME/tcl/client/client_globals.tcl

    if { $TCS_Connected } {
	tcs_get all $parent_win
    }

    after $Client_Update_Rate tcs_looper $parent_win

}
