################################################## 
#
# server_page.tcl
#
####
#
# Create window to turn on Guido's data server
#
# Written  13apr06  DLM 
#
##################################################

proc server_page { server_win page } {

    global PCR_HOME
    source $PCR_HOME/tcl/window/window_globals.tcl
    source $PCR_HOME/tcl/server/server_globals.tcl
    source $PCR_HOME/tcl/process/process_globals.tcl
    source $PCR_HOME/tcl/pcr/pcr_globals.tcl
    source $PCR_HOME/tcl/topbox/topbox_globals.tcl
    source $PCR_HOME/tcl/status/status_globals.tcl
    source $PCR_HOME/tcl/bitmaps/bitmaps_globals.tcl
#
    global reply_data
#
    upvar #0 Info Info
#
# Create the tix NoteBook frame
#
    global server_page
    set server_page [ $server_win subwidget $page ]
#
############################################
#
# Build window
#
############################################
#
    frame $server_page.server
    frame $server_page.instruction
#
    frame $server_page.server.process -relief ridge -border 4
    frame $server_page.server.gui -relief ridge -border 4
    frame $server_page.server.pcr -relief ridge -border 4
    frame $server_page.server.power -relief ridge -border 4
#
#-----------------------------
# AO Processes (Servers)
#-----------------------------
#
# Labels
#
    label $server_page.server.process.process_label \
	-text "AO Processes" -bg cyan
    label $server_page.server.process.server_label \
	-text "Process"
    label $server_page.server.process.status_label \
	-text "Status"
#
# Pack the client buttons
#
#   Labels
#
    set i 0
    set j 0
    grid config $server_page.server.process.process_label \
        -row $i -column $j -columnspan 2 -sticky ew
#
    incr i
    set j 0
    grid config $server_page.server.process.server_label \
        -row $i -column $j -sticky ew
    incr j
    grid config $server_page.server.process.status_label \
        -row $i -column $j -sticky ew
    incr i
#
# PCR Server Process
#
    label $server_page.server.process.pcr_server_label \
	-text "Data_Server"
    button $server_page.server.process.pcr_server_start \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    process_start pcr_server $server_page
	}
#
# PCR Process
#
    label $server_page.server.process.pcr_label \
	-text "PCR"
    button $server_page.server.process.pcr_start \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    process_start pcr $server_page
	}
#
# DDS Process
#
    label $server_page.server.process.dds_label \
	-text "DDS"
    button $server_page.server.process.dds_start \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    process_start dds $server_page
	}
#
# Topbox Process
#
    label $server_page.server.process.topbox_label \
	-text "Topbox Server"
    button $server_page.server.process.topbox_start \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    process_start topbox_server $server_page
	}
#
# TSS server Process
#
    label $server_page.server.process.tss_label \
	-text "TSS_Server"
    button $server_page.server.process.tss_start \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    puts "tss: not yet\n"
	    ##process_start tss_server $server_page
	}
#
# Pack PCR_SERVER stuff
#
    set j 0
    grid config $server_page.server.process.pcr_server_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.process.pcr_server_start \
        -row $i -column $j
    incr i

    set j 0
    grid config $server_page.server.process.pcr_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.process.pcr_start \
        -row $i -column $j
    incr i

    set j 0
    grid config $server_page.server.process.dds_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.process.dds_start \
        -row $i -column $j
    incr i

    set j 0
    grid config $server_page.server.process.topbox_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.process.topbox_start \
        -row $i -column $j
    incr i

    set j 0
    grid config $server_page.server.process.tss_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.process.tss_start \
        -row $i -column $j
#
#-----------------------------
# GUI connections
#-----------------------------
#
# Labels
#
    label $server_page.server.gui.gui_label \
	-text "Gui Connections" -bg cyan
    label $server_page.server.gui.server_label \
	-text "Server"
    label $server_page.server.gui.stat_label \
	-text "Status"
#
# Pack the client buttons
#
#   Labels
#
    set i 0
    set j 0
    grid config $server_page.server.gui.gui_label \
        -row $i -column $j -columnspan 2 -sticky ew
#
    incr i
    set j 0
    grid config $server_page.server.gui.server_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.gui.stat_label \
        -row $i -column $j -sticky ew
    incr i
#
# TCS connection
#
    label $server_page.server.gui.tcs_label \
	-text "TCS"
    button $server_page.server.gui.tcs_connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    tcs_connect $server_page
	}
#
# Pack TCS stuff
#
    set j 0
    grid config $server_page.server.gui.tcs_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.gui.tcs_connected \
        -row $i -column $j
    incr i
#
# Status connection
#
    label $server_page.server.gui.status_label \
	-text "Topbox_Status"
    button $server_page.server.gui.status_connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    status_connect $server_page
	}
#
# Pack Status stuff
#
    set j 0
    grid config $server_page.server.gui.status_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.gui.status_connected \
        -row $i -column $j
    incr j
    incr i
#
# Topbox connection
#
    label $server_page.server.gui.topbox_label \
	-text "Topbox_Cmd"
    button $server_page.server.gui.topbox_connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    topbox_connect $server_page
	    if { $status } {
		tk_messageBox -message \
		    "Error connecting to the Topbox\n $reply_data" \
		    -parent $server_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# Pack Topbox stuff
#
    set j 0
    grid config $server_page.server.gui.topbox_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.gui.topbox_connected \
        -row $i -column $j
    incr i
#
# Info connection
#
    label $server_page.server.gui.info_label \
	-text "PCR_Info"
    button $server_page.server.gui.info_connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    info_connect $server_page
	}
#
# Pack Info stuff
#
    set j 0
    grid config $server_page.server.gui.info_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.gui.info_connected \
        -row $i -column $j
    incr j
    incr i
#
# PCR_Server connection
#
    label $server_page.server.gui.pcr_label \
	-text "PCR_Cmd"
    button $server_page.server.gui.pcr_connected \
	-background red -activebackground OrangeRed \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    pcr_server_connect PCR $server_page
	}
#
# Pack PCR_Server stuff
#
    set j 0
    grid config $server_page.server.gui.pcr_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.gui.pcr_connected \
        -row $i -column $j
    incr j
    incr i
#
# Labels
#
    label $server_page.server.pcr.pcr_server_label \
	-text "PCR_Server Connections" -bg cyan
    label $server_page.server.pcr.server_label \
	-text "Server"
    label $server_page.server.pcr.status_label \
	-text "Status"
#
# Pack the client buttons
#
#   Labels
#
    set i 0
    set j 0
    grid config $server_page.server.pcr.pcr_server_label \
        -row $i -column $j -columnspan 2 -sticky ew
#
    incr i
    set j 0
    grid config $server_page.server.pcr.server_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.pcr.status_label \
        -row $i -column $j -sticky ew
    incr i
#
# PCR_Server connections to the PCR
#
    foreach el $Server_List {
#
	label $server_page.server.pcr.label_$el \
	    -text $el
	button $server_page.server.pcr.connected_$el \
	    -background red -activebackground OrangeRed \
	    -padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	    -command "
		pcr_server_connect $el $server_page
	    "
#
# Pack PCR_Server Connections
#
	set j 0
	grid config $server_page.server.pcr.label_$el \
	    -row $i -column $j -sticky e
	incr j
	grid config $server_page.server.pcr.connected_$el \
	    -row $i -column $j
	incr i
    }
#
#-----------------------------
# Power Switches
#-----------------------------
#
# Title
#
    label $server_page.server.power.title \
	-text "Power Switches" -bg cyan
#
# Column labels
#
    label $server_page.server.power.connected_label \
	-text "Connected" -bg red -anchor e
#
# Connected light
#
    button $server_page.server.power.connected \
	-background red -activebackground tomato \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    set status [ topbox_connect_device "Power" $topbox_page ]
	    if { $status } {
		tk_messageBox -message \
		    "Error connecting to Power Switch\n $reply_data" \
		    -parent $topbox_page \
		    -icon error -type ok
		return $status
	    }
	}
#
# Switch labels and indicator lights
#
    frame $server_page.server.power.line -relief ridge -height 1m -border 3 -bg black
    label $server_page.server.power.status_label -text "Status"

    label $server_page.server.power.wfsc_label \
	-text "WFSC"
    button $server_page.server.power.wfsc_start \
	-background red -activebackground tomato \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    topbox_power "WFSC" $server_page
	}

    label $server_page.server.power.peltier_label \
	-text "WFSC_Peltier"
    button $server_page.server.power.peltier_start \
	-background red -activebackground tomato \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    topbox_power "Peltier" $server_page
	}

    label $server_page.server.power.stella_label \
	-text "Stella"
    button $server_page.server.power.stella_start \
	-background red -activebackground tomato \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    topbox_power "Stella" $server_page
	}

    label $server_page.server.power.alignment_label \
	-text "Alignment_Laser"
    button $server_page.server.power.alignment_start \
	-background grey84 -activebackground grey100 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    topbox_power "Alignment_Laser" $server_page
	}

    label $server_page.server.power.f15_label \
	-text "F15_Laser"
    button $server_page.server.power.f15_start \
	-background grey84 -activebackground grey100 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    topbox_power "F15_Laser" $server_page
	}

    label $server_page.server.power.indigo_label \
	-text "Indigo"
    button $server_page.server.power.indigo_start \
	-background grey84 -activebackground grey100 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    topbox_power "Indigo" $server_page
	}

    label $server_page.server.power.spare1_label \
	-text "Spare1"
    button $server_page.server.power.spare1_start \
	-background grey84 -activebackground grey84 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    topbox_power "Spare1" $server_page
	}

    label $server_page.server.power.spare2_label \
	-text "Spare2"
    button $server_page.server.power.spare2_start \
	-background grey84 -activebackground grey100 \
	-padx 0 -pady 0 -height 5 -width 5 -bitmap @$BitMaps_Dir/solid \
	-command { 
	    topbox_power "Spare2" $server_page
	}
#
# Pack the Switch stuff
#
    set i 0
    set j 0
    grid config $server_page.server.power.title \
        -row $i -column $j -columnspan 2 -sticky ew
    incr i

    set j 0
    grid config $server_page.server.power.connected_label \
        -row $i -column $j -sticky ew
    incr j
    grid config $server_page.server.power.connected \
        -row $i -column $j
    incr i

    set j 0
    grid config $server_page.server.power.line \
        -row $i -column $j -columnspan 2 -sticky ew
    grid config $server_page.server.power.status_label \
        -row $i -column $j -columnspan 2 -sticky e
    incr i

    set j 0
    grid config $server_page.server.power.wfsc_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.power.wfsc_start \
        -row $i -column $j
    incr i

    set j 0
    grid config $server_page.server.power.peltier_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.power.peltier_start \
        -row $i -column $j
    incr i

    set j 0
    grid config $server_page.server.power.stella_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.power.stella_start \
        -row $i -column $j
    incr i

    set j 0
    grid config $server_page.server.power.alignment_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.power.alignment_start \
        -row $i -column $j
    incr i

    set j 0
    grid config $server_page.server.power.f15_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.power.f15_start \
        -row $i -column $j
    incr i

    set j 0
    grid config $server_page.server.power.indigo_label \
        -row $i -column $j -sticky e
    incr j
    grid config $server_page.server.power.indigo_start \
        -row $i -column $j
    incr i

#    set j 0
#    grid config $server_page.server.power.spare1_label \
#        -row $i -column $j -sticky e
#    incr j
#    grid config $server_page.server.power.spare1_start \
#        -row $i -column $j
#    incr i

#    set j 0
#    grid config $server_page.server.power.spare2_label \
#        -row $i -column $j -sticky e
#    incr j
#    grid config $server_page.server.power.spare2_start \
#        -row $i -column $j
#    incr i
#
###################
#  Instuctions
###################
#
    label $server_page.instruction.click_label \
	-text "**Click a Red button to connect**"
#
# Pack the instructions
#
    pack config $server_page.instruction.click_label
#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $server_page.server.process \
	$server_page.server.gui \
	$server_page.server.pcr \
	$server_page.server.power \
	-side left -anchor nw
#
    pack $server_page.server \
	$server_page.instruction \
	-side top -anchor n
#
# Force an update of the PCR_Server/PCR/DDS Running buttons
#
    set PCR_Server_Running $PCR_Server_Running
    set PCR_Running $PCR_Running
    set DDS_Running $DDS_Running
#
# Force an update of the PCR_Server connection button
#
    set PCR_Connected $PCR_Connected
#
# Force an update of the Topbox_Server Running button
#
    set Topbox_Server_Running $Topbox_Server_Running
#
# Force an update of the Topbox Server connection button
#
    set Topbox_Connected $Topbox_Connected
#
# Force an update of the Status conneciont to the Topbox Server
#
    set Status_Connected $Status_Connected

}
