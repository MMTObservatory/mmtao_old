###################################
#
# server_globals_set.tcl
#
###################################
#
# The *_globals.tcl file is sourced by the upper most level of TCL
#   source code (ie. AO_GUI_startup.tcl) as well as the all lower
#   level of tcl code that need these variables.
#
# The *_globals_set.tcl file is sourced by only the upper most level
#   TCL code (usually *_startup.tcl) so values are set to the global
#   variables only once.  In *_globals_set.tcl file, the trace command
#   can be used to force the update of gui variables, colors, buttons,
#   etc on all gui panels.
#
###################################
#
# Connections between PCR_Server and the PCR
#
set Server_List [ list \
		      PCR_CMD \
		      PCR_WFSC \
		      PCR_DDS \
		      PCR_ERROR ]
#
# PCR_Server Running button controls
#
trace variable PCR_Server_Running w pcr_server_running_trace
proc pcr_server_running_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.process.pcr_server_start configure -bg green -activebackground lightgreen
	} else {
	    $server_page.server.process.pcr_server_start configure -bg red -activebackground OrangeRed
	}
    }
}
#
# PCR Running button controls
#
trace variable PCR_Running w pcr_running_trace
proc pcr_running_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl

    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.process.pcr_start configure -bg green -activebackground lightgreen
	    if { [string compare .null $camera_page] } {
	        $camera_page.cam_servers.pcr_start configure -bg green -activebackground lightgreen
            }
	} else {
	    $server_page.server.process.pcr_start configure -bg red -activebackground OrangeRed
	    if { [string compare .null $camera_page] } {
	        $camera_page.cam_servers.pcr_start configure -bg red -activebackground OrangeRed
            }
	}
    }
}
#
# DDS Running button controls
#
trace variable DDS_Running w dds_running_trace
proc dds_running_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.process.dds_start configure -bg green -activebackground lightgreen
	} else {
	    $server_page.server.process.dds_start configure -bg red -activebackground OrangeRed
	}
    }
}
#
# Topbox_Server Running button controls
#
trace variable Topbox_Server_Running w topbox_server_running_trace
proc topbox_server_running_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.process.topbox_start configure -bg green -activebackground lightgreen
	} else {
	    $server_page.server.process.topbox_start configure -bg red -activebackground OrangeRed
	}
    }
}
#
# Science Server Running button controls
#
trace variable Science_Server_Running w science_server_running_trace
proc science_server_running_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.process.science_start configure -bg green -activebackground lightgreen
	} else {
	    $server_page.server.process.science_start configure -bg red -activebackground OrangeRed
	}
    }
}
#
# TSS Server Running button controls
#
trace variable TSS_Server_Running w tss_server_running_trace
proc tss_server_running_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.process.tss_start configure -bg green -activebackground lightgreen
	} else {
	    $server_page.server.process.tss_start configure -bg red -activebackground OrangeRed
	}
    }
}
#
# TCS Connected button controls
#
trace variable TCS_Connected w tcs_connected_trace
proc tcs_connected_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.gui.tcs_connected configure -bg green -state disabled
	} else {
	    $server_page.server.gui.tcs_connected configure -bg red -state normal
	}
    }
}
#
# Status Connected button controls
#
trace variable Status_Connected w status_connected_trace
proc status_connected_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.gui.status_connected configure -bg green -state disabled
	} else {
	    $server_page.server.gui.status_connected configure -bg red -state normal
	}
    }
}
#
# Topbox Connected button controls
#
trace variable Topbox_Connected w topbox_connected_trace
proc topbox_connected_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.gui.topbox_connected configure -bg green -state disabled
	} else {
	    $server_page.server.gui.topbox_connected configure -bg red -state normal
	}
    }
}
#
# Science Connected button controls
#
trace variable Science_Connected w science_connected_trace
proc science_connected_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.gui.science_connected configure -bg green -state disabled
	} else {
	    $server_page.server.gui.science_connected configure -bg red -state normal
	}
    }
}
#
# TSS Connected button controls
#
trace variable TSS_Connected w tss_connected_trace
proc tss_connected_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.gui.tss_connected configure -bg green -state disabled
	} else {
	    $server_page.server.gui.tss_connected configure -bg red -state normal
	}
    }
}
#
# Info Connected button controls
#
trace variable Info_Connected w info_connected_trace
proc info_connected_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.gui.info_connected configure -bg green -state disabled
	} else {
	    $server_page.server.gui.info_connected configure -bg red -state normal
	}
    }
}
#
# PCR Connected button controls
#
trace variable PCR_Connected w pcr_connected_trace
proc pcr_connected_trace { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $value } {
	    $server_page.server.gui.pcr_connected configure -bg green -state disabled
	} else {
	    $server_page.server.gui.pcr_connected configure -bg red -state normal
	}
    }
}
#
# Connected between the PCR_Server and the PCR
#
trace variable Info(PCR_CMD_Connected) w pcr_cmd_connected_trace
proc pcr_cmd_connected_trace { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    source $PCR_HOME/tcl/process/process_globals.tcl

    upvar #0 Info Info

    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $Info(PCR_CMD_Connected) } {
	    $server_page.server.pcr.connected_PCR_CMD configure -bg green -state disabled
#	    set PCR_Running 1
	} else {
	    $server_page.server.pcr.connected_PCR_CMD configure -bg red -state normal
#	    set PCR_Running 0
	}
    }
}
#
trace variable Info(PCR_WFSC_Connected) w pcr_wfsc_connected_trace
proc pcr_wfsc_connected_trace { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    source $PCR_HOME/tcl/process/process_globals.tcl

    upvar #0 Info Info

    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $Info(PCR_WFSC_Connected) } {
	    $server_page.server.pcr.connected_PCR_WFSC configure -bg green -state disabled
	    set PCR_Running 1
	} else {
	    $server_page.server.pcr.connected_PCR_WFSC configure -bg red -state normal
	    set PCR_Running 0
	}
    }
}
#
trace variable Info(PCR_DDS_Connected) w pcr_dds_connected_trace
proc pcr_dds_connected_trace { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    source $PCR_HOME/tcl/process/process_globals.tcl

    upvar #0 Info Info

    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $Info(PCR_DDS_Connected) } {
	    $server_page.server.pcr.connected_PCR_DDS configure -bg green -state disabled
	    set DDS_Running 1
	} else {
	    $server_page.server.pcr.connected_PCR_DDS configure -bg red -state normal
	    set DDS_Running 0
	}
    }
}
#
trace variable Info(PCR_ERROR_Connected) w pcr_error_connected_trace
proc pcr_error_connected_trace { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl

    upvar #0 Info Info

    if { [ expr ![ expr ![winfo exists $main_win] || ![string compare .null $server_page] ] ] } {
	if { $Info(PCR_ERROR_Connected) } {
	    $server_page.server.pcr.connected_PCR_ERROR configure -bg green -state disabled
	} else {
	    $server_page.server.pcr.connected_PCR_ERROR configure -bg red -state normal
	}
    }
}
