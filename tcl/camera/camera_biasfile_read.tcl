################################################## 
#
# camera_biasfile_read.tcl
#
####
#
# Read the requested camera bias file
#
# Written  24jun07  DLM  Started with sc_log_open.tcl
#
##################################################

proc camera_biasfile_read { filename bias_0 bias_1 bias_2 bias_3 parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/camera/camera_globals.tcl

    set there [file exists $filename $parent_win]
    puts "there = $there"

    if { !$there } {
	set status \
	    [tk_messageBox -parent $parent_win -icon error -type ok \
		 -message "$filename does not exists" ]
	return -1
    }	

    set fd [ open $filename r ]
    puts "fd = $fd"

    gets $fd bias_0
    puts $bias_0
    gets $fd bias_1
    puts $bias_1
    gets $fd bias_2
    puts $bias_2
    gets $fd bias_3
    puts $bias_3

    close $fd

    if { $debug_Camera } {
	puts "  camera_biasfile_read: Camera bias file read"
    }

    return 0
}
