###################################
#
# bg_globals_set.tcl
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
# Flag if background frame has been taken but not loaded
#
set BG_Taken 0
#
# The current background file
#
set BG_Current None
#
# File with all 0's for background subtraction
#
set BG_Zero_File ${PCR_HOME}/tcl/bg/Zero_Background
#
# Flag if background frame has been taken but not loaded
#
set BG_Taken 0
#
# Root window name set to none
#
set bg_page NULL
#
# Info Connected button controls
#
trace variable Info(Background_File) w info_bg_file_trace
proc info_bg_file_trace { name element op } {

    upvar #0 Info Info
    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    source $PCR_HOME/tcl/bg/bg_globals.tcl

    set BG_Current [file tail $Info(Background_File)]
    if { ![expr ![winfo exists $main_win] || ![string compare NULL $bg_page] ] } {
	if { [string compare Zero_Background $BG_Current ] == 0 } {
	    $bg_page.full.curr.label configure -bg grey84
	    $bg_page.full.curr.display configure -state disabled
	    $bg_page.full.curr.on_off configure -state disabled
	    if { $Info(Background_Static) } {
		$bg_page.full.curr.on_off configure -state normal -bg green -activebackground green \
		    -text "Background Subtraction is\nON\n\nPress to Stop"
		set BG_Current "Unknown"
	    } else {
		$bg_page.full.curr.on_off configure -state disabled -bg grey84 -activebackground grey84 \
		    -text "Background Subtraction is\nOFF\n\n"
	    }
	} else {
	    $bg_page.full.curr.label configure -bg green
	    $bg_page.full.curr.display configure -state normal
	    $bg_page.full.curr.on_off configure -state normal
	    if { $Info(Background_Static) } {
		$bg_page.full.curr.on_off configure -state normal -bg green -activebackground green \
		    -text "\nBackground Subtraction is\nON\nPress to Stop"
	    } else {
		$bg_page.full.curr.on_off configure -state normal -bg yellow -activebackground yellow \
		    -text "\nBackground Subtraction is\nOFF\nPress to Start"
	    }
	}	
    }

}
#
trace variable Info(Background_Dynamic) w info_bg_dynamic_trace
proc info_bg_dynamic_trace { name element op } {

    upvar #0 Info Info
    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    source $PCR_HOME/tcl/bg/bg_globals.tcl

    if { ![expr ![winfo exists $main_win] || ![string compare NULL $bg_page] ] } {
	if { $Info(Background_Dynamic) } {
	    $bg_page.dynamic.on_off configure -bg green -activebackground green \
		-text "\nDynamic Subtraction is\nON\n"
	} else {
	    $bg_page.dynamic.on_off configure -bg grey84 -activebackground grey84 \
		-text "\nDynamic Subtraction is\nOFF\n"
	}
    }
}