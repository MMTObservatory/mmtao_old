###################################
#
# display_spots.tcl
#
###################################
#
# Display the requested frame in and IDL window
#
# Written  24feb06  DLM
#
# Altered   9sep06  DLM  Added argument realtime so we can click the "Display *"
#                          button to update the range of the graph while the
#                          realtime display is running, without starting another
#                          dclient_display_single loop.  This was really slowing
#                          down the display.
#
# Altered   0jun-7  DLM  Removed the changing of the refreash rate when error occur.
#                          This was causing the RT display to slow down when it should
#                          not have.
#
# Altered  29jan07  DLM  Changed name from dclient_display_single.tcl
#
proc display_spots { name init_display realtime parent_win } {

    global reply_data

    global PCR_HOME
    source $PCR_HOME/tcl/display/display_globals.tcl
    source $PCR_HOME/tcl/pgplot/pgplot_globals.tcl
#
    upvar #0 Info Info
#
# Retrive and display requested frame
#
    if { $Info(PCR_DDS_Receiving_Data) == 1 } {
	set status [catch { DClient_Cmd Spots Single $init_display } msg ]
    } elseif { $Info(PCR_WFSC_Receiving_Data) == 1 } {
	set status [catch { DClient_Cmd WFSC Single $init_display } msg ]
    } else {
	set status 0
    }
#
    set Spot_Symbol $PGplot_Symbol($New_Spot)
#
# Deal with errors
#
    if { $status == -1 } {
	set Display_RT($name) 0
	tk_messageBox -message \
	    "Error connecting to data server" \
	    -parent $parent_win \
	    -icon error -type ok
    }
#
# Continue updating display if requested
#
    if { $Display_RT($name) && $realtime } {
	after $Display_RT_Rate($name) display_spots $name $init_display $realtime $parent_win
    }

}
