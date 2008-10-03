###################################
#
# global_trace.tcl
#
###################################
#
# The upper most level TCL code must also source this file
#   The trace command is used to force the update of gui
#   variables on all gui panels.
#
# All TCL source codes should source global_variables.tcl
#   in order for the automatic update to take place.
#

set SC_Accept 0
set SC_Socket ""
set SC_Parameters 0
set SC_Arguements 0
set SC_Interp ""
set SC_Name ""
set SC_Connected 0

set SC_Home_Set 0
set SC_Tip_Home 0.0
set SC_Tilt_Home 0.0
set SC_RA_Home 0.0
set SC_DEC_Home 0.0

set SC_Tip_Off 0
set SC_Tilt_Off 0
set SC_RA_Off 0
set SC_DEC_Off 0

set SC_Max_Incr 0.2
set SC_Min_Incr 0.01
set SC_Wait 2000
set SC_Max_Move 40.0

set SC_Move_Wait 2000
set SC_Max_Closed 0.4

set SC_Log_Sock 0

set SC_Prompt 0

set debug_SC 0

set SC_Update 0

set SC_Ra_Offset 0.0
set SC_Dec_Offset 0.0

set nominal_gain 0.6

set SC_Info_List [list \
		      Loop_Running \
		      Loop_Gain \
		      WFSC_Frequency \
		      WFSC_Rotation \
		      WFSC_Focus \
		      Static_Astig_0 \
		      Static_Astig_45 \
		      Static_Coma_x \
		      Static_Coma_y \
		      Static_Trefoil_0 \
		      Static_Trefoil_30 \
		      Static_Spherical \
		      Seeing_Value \
		      Background_Dynamic \
		      Background_Static \
		      Background_File \
		     ]

set SC_Delay_1 200
set SC_Delay_2 10
set SC_Delay_3 2000
set SC_Delay_4 500
set SC_Delay_5 500

set SC_Focus 0

trace variable SC_Connected w sc_connected
proc sc_connected { name element op } {
    upvar $name x
    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl
    if { [winfo exists .sc_win] } {
	if { $x } {
	    .sc_win.request.connect configure -bg green -text "Connected"
	} else {
	    .sc_win.request.connect configure -bg grey84 -text "Not Connected"
	    for {set num 0} {$num < 4} {set num [expr $num + 1]} {
		set SC_Request($num) --
	    }
	}
    }
}

trace variable SC_Accept w sc_accept
proc sc_accept { name element op } {
    upvar $name x
    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl
    if { [winfo exists .sc_win] } {
	if { $x } {
	    .sc_win.accept.button configure -bg green -activebackground lightgreen -text "Accepting Requests"
	} else {
	    .sc_win.accept.button configure -bg yellow -activebackground lightyellow -text "Not Accepting Requests"
	}
    }
}

trace variable SC_RA_Home w sc_ra_home_set
proc sc_ra_home_set { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    upvar #0 TCS TCS

    set SC_RA_Off [expr $TCS(ao_off_ra) - $value]
}

trace variable SC_DEC_Home w sc_dec_home_set
proc sc_dec_home_set { name element op } {
    upvar ${name} value

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    upvar #0 TCS TCS

    set SC_DEC_Off [expr $TCS(ao_off_dec) - $value]
}

trace variable TCS(ao_off_ra) w sc_tcs_ao_off_ra_set
proc sc_tcs_ao_off_ra_set { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl
#
    upvar #0 TCS TCS
#
    set SC_RA_Off [expr $TCS(ao_off_ra) - $SC_RA_Home]
}

trace variable TCS(ao_off_dec) w sc_tcs_ao_off_dec_set
proc sc_tcs_ao_off_dec_set { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl
#
    upvar #0 TCS TCS
#
    set SC_DEC_Off [expr $TCS(ao_off_dec)- $SC_DEC_Home]
}

trace variable Status(FSM_Tip_Position) w status_fsm_tip_position_set
proc status_fsm_tip_position_set { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl
#
    upvar #0 Status Status
#
    set SC_Tip_Off [expr $Status(FSM_Tip_Position) - $SC_Tip_Home]
}

trace variable Status(FSM_Tilt_Position) w status_fsm_tilt_position_set
proc status_fsm_tilt_position_set { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl
#
    upvar #0 Status Status
#
    set SC_Tilt_Off [expr $Status(FSM_Tilt_Position) - $SC_Tilt_Home]
}

trace variable Status(Rotation_Position) w status_rotation_position_set
proc status_rotation_position_set { name element op } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl
#
    upvar #0 Status Status
    upvar #0 Info Info
#
    set Info(WFSC_Rotation) $Status(Rotation_Position)
}
