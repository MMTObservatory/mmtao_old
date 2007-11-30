################################################## 
#
# camera_biasfile_open.tcl
#
####
#
# Open the requested camera bias file with the requested access
#   parameters
#
# Written  24jun07  DLM  Started with sc_log_open.tcl
#
##################################################

proc camera_biasfile_open { filename access } {

    global PCR_HOME
    source $PCR_HOME/tcl/camera/camera_globals.tcl

    set fd [ open $filename $access ]
    puts "fd = $fd"

    if { $debug_SC } {
	puts "  camera_biasfile_open: Opened camera bais file $filename with acess $access"
    }

    return fd

}
