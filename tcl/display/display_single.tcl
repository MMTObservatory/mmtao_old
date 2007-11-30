###################################
#
# display_single.tcl
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
# Altered  29jan07  DLM  Changed name from dclient_display_single.tcl
#
proc display_single { name init_display realtime parent_win } {

    global reply_data

    global PCR_HOME
    source $PCR_HOME/tcl/display/display_globals.tcl

#
# Retrive and display requested frame
#
    set status [catch { DClient_Cmd $name Single $init_display } msg ]
#
# Deal with errors
#
    if { $status < 0 } {
#
#     No data to display
#
	if { $status == -1 } {
	    set Display_RT($name) 0
	    tk_messageBox -message \
		"Error connecting to data server" \
		-parent $parent_win \
		-icon error -type ok
	} elseif { $status == -2 } {
	    set rate 1000
	} elseif { $status == -10 } {
	    puts "no NEW data"
	    set rate 1000
	} elseif { $status == -11 } {
	    puts "re-initializing"
	    set rate $Display_RT_Rate($name)
	} elseif { $status == -12 } {
	    set rate $Display_RT_Rate($name)
	    set Display_RT($name) 0
	    tk_messageBox -message \
		$reply_data \
		-parent $parent_win \
		-icon error -type ok
	} else {
	    set Display_RT($name) 0
	    tk_messageBox -message \
		$reply_data \
		-parent $parent_win \
		-icon error -type ok
	}
	set init_display 1
    } elseif { $status == 1 } {
#
#     Display old data but slow down refresh rate
#
	set rate 1000
	set init_display 1
    }  else {
#
#     Full refresh rate with new frame
#
	set rate $Display_RT_Rate($name)
	set init_display 0
    }
#
# Continue updating disply if requested
#
    if { $Display_RT($name) && $realtime } {
	after $rate display_single $name $init_display $realtime $parent_win
    }

}
