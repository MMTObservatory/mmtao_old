#=================================================================
# voltmeter_window.tcl
#=================================================================
#
# TclTK procedure to create the second level window for
# control and information about the fsm (tip and tilt).
#
# Created   8jan04   DLM  Started with alignment_window.
#
# Altered   4may06   DLM  Upgraded to new gui structure
#
# New Commands for Resistance
#
# SYST:ERR?
# CONF:RES (@101)
# ROUT:MON:STAT ON
# ROUT:MON:DATA?
# ROUT:MON:STAT OFF
#
#=================================================================

proc voltmeter_window { win_name } {

    global AO_GUI_HOME
    source $AO_GUI_HOME/tcl/window/window_globals.tcl

#
# Check that the correct window name was sent
#
    if { $win_name != $volt_win } {
	puts "voltmeter_window routine should be called with the parameter \$volt_win"
	return -1
    }

    toplevel $volt_win
    wm title $volt_win "DM Voltmeter"
    wm geometry $volt_win $volt_position
#
# Initialize global variables
#
    source $AO_GUI_HOME/tcl/voltmeter/voltmeter_globals.tcl
#
#=================================================================
# Build Window
#=================================================================
#
#==================================
# Menu Bar
#==================================
#
    frame $volt_win.menubar -relief raised -bd 2
#
    menubutton $volt_win.menubar.connect -text "Connect" \
	-menu $volt_win.menubar.connect.menu
    menubutton $volt_win.menubar.update -text "Update" \
	-menu $volt_win.menubar.update.menu
#
    pack $volt_win.menubar.connect \
	$volt_win.menubar.update -side left
#
# Create connect menu
#
    menu $volt_win.menubar.connect.menu
    $volt_win.menubar.connect.menu add command -label "Voltmeter" \
	-command {
	    voltmeter_init $volt_win
	}
#
# Create update menu
#
    menu $volt_win.menubar.update.menu
    $volt_win.menubar.update.menu add command -label "Continously" \
	-command {
	    set VoltMeter_Update 1
	    voltmeter_looper $volt_win
	}
    $volt_win.menubar.update.menu add command -label "Once" \
	-command {
	    voltmeter_measure $volt_win
	}
#
################
# VOLTMETER
################
#
    frame $volt_win.voltmeter              -border 0
    frame $volt_win.voltmeter.sub          -border 4

    label $volt_win.voltmeter.title \
        -text "Resistance" -pady 4 -bg lightskyblue

    button $volt_win.voltmeter.sub.init_button -text "Connect" \
	-command {
	    voltmeter_connect $volt_win
        }
    button $volt_win.voltmeter.sub.set_button -text "Init" \
	-command {
	    voltmeter_init $volt_win
        }
#	    voltmeter_set "CONF:VOLT:DC 10,0.001,(@101:111)" $volt_win
    button $volt_win.voltmeter.sub.start_button -text "Start" \
	-command {
	    voltmeter_set "CONF:RES (@101:111)" $volt_win
	    voltmeter_set "TRIG:SOURCE TIMER" $volt_win
	    voltmeter_set "TRIG:TIMER 1.0" $volt_win
	    voltmeter_set "TRIG:COUNT INFINITY" $volt_win
	    voltmeter_set "INIT" $volt_win
        }
    button $volt_win.voltmeter.sub.stop_button -text "Stop" \
	-command {
	    voltmeter_set "ABOR" $volt_win
        }
    button $volt_win.voltmeter.sub.get_button -text "Get" \
	-command {
	    voltmeter_measure $volt_win
        }
    button $volt_win.voltmeter.sub.close_button -text "Close" \
        -command {
	    voltmeter_set "ABOR" $volt_win
	    voltmeter_close $volt_win
        }

    pack $volt_win.voltmeter.sub.init_button \
	$volt_win.voltmeter.sub.set_button \
	$volt_win.voltmeter.sub.start_button \
	$volt_win.voltmeter.sub.get_button \
	$volt_win.voltmeter.sub.stop_button \
	$volt_win.voltmeter.sub.close_button -side top

#
# VoltMeter readings
#
    frame $volt_win.voltmeter.readings -border 4

    set i 0
    foreach el $Resistance_Channels {
	
	button $volt_win.voltmeter.readings.value_$el -relief raised \
	    -text "Channel $el" -width 10 -bg yellow \
	    -command "
                voltmeter_monitor $el $volt_win
	    "

	label $volt_win.voltmeter.readings.label_$el -relief sunken \
	    -textvariable Resistance($el) -width 10

	grid $volt_win.voltmeter.readings.value_$el \
	    -row $i -column 0
	grid $volt_win.voltmeter.readings.label_$el \
	    -row $i -column 1
	incr i

    }

#
# Pack the VoltMeter parts
#
    pack $volt_win.voltmeter.title

    pack $volt_win.voltmeter.sub \
	$volt_win.voltmeter.readings -side left

    frame $volt_win.hline \
        -borderwidth 5 -relief ridge -height 3m -bg black
#
#-----------------------------------------------------------------
# Closing
#-----------------------------------------------------------------
#
    button $volt_win.close -text "Quit" \
      -command {
	  exit
      }

    set i 0
    grid config $volt_win.menubar -row $i -column 0 -sticky ew
    incr i
    grid config $volt_win.voltmeter -row $i -column 0 -sticky ew
    incr i
    grid config $volt_win.hline -row $i -column 0 -sticky nsew -columnspan 1
    incr i
    grid config $volt_win.close -row $i -column 0 -sticky nsew -columnspan 1
#
#=================================================================
#
# Start updating VoltMeter values
# 
    set VoltMeter_Update 0
#    voltmeter_looper $volt_win
}
