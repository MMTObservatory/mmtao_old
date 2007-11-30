################################################## 
#
# camera_biasfile_close.tcl
#
####
#
# Close an open camera bias file
#
# Written  24jun07  DLM  Started with sc_log_open.tcl
#
##################################################

proc camera_biasfile_close { fd } {

    global PCR_HOME
    source $PCR_HOME/tcl/camera/camera_globals.tcl

    if { $debug_Camera } {
	puts "  camera_biasfile_close: Closing camera bias file"
    }

    flush $fd
    close $fd

}
