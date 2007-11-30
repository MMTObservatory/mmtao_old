###################################
#
# hex_looper.tcl
#
###################################
#
# Self calling prodecure to update Hexapod information on GUI
#
# Written    3feb07  DLM  Started with tcs_looper.tcl
#
# Altered    2feb06  DLM  Change so the loop is infinite, but the call to retieve
#                           Hexapod information only occurs if we are connected to the
#                           Hexapod.
#

proc hex_looper { parent_win } {
      
    global PCR_HOME
    source $PCR_HOME/tcl/hex/hex_globals.tcl
    source $PCR_HOME/tcl/client/client_globals.tcl

    if { $Hex_Connected } {
	hex_get all $parent_win
    }

    after $Client_Update_Rate hex_looper $parent_win

}
