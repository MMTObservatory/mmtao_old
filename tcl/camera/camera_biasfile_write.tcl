################################################## 
#
# camera_biasfile_write.tcl
#
####
#
# Write the requested camera bias values to file
#
# Written  24jun07  DLM  Started with sc_log_open.tcl
#
##################################################

proc camera_biasfile_write { filename bias_0 bias_1 bias_2 bias_3 parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/camera/camera_globals.tcl

    set there [file exists $filename $parent_win]
    puts "there = $there"

    if { $there } {
	set status \
	    [tk_messageBox -parent $parent_win -icon question -type yesno \
		 -message "$filename exists\nDo you want to overwrite?" ]
	if { $status == "np" } {
	    return 0
	} 
    }	

    set fd [ open $filename w ]
    puts "fd = $fd"

    puts $fd $bias_0
    puts $fd $bias_1
    puts $fd $bias_2
    puts $fd $bias_3

    flush $fd
    close $fd

    if { $debug_Camera } {
	puts "  camera_biasfile_write: Camera bias file written"
    }

    return 0
}
