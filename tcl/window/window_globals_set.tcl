#########################################################
#
# global_win_names_set.tcl
#
#########################################################
#
# This file sets values to global variables and is sourced
#   only once by the startup script.  Any tcl/tk script which
#   needs to use these variables will source global_win_names.tcl
#
# Written 21dec04  DLM
#
#########################################################
#
# Do we have a dual screen? (astigmatism only...so far)
#
if { [string first astigmatism $env(HOST)] > 0 } {
    set DUAL_SCREEN 1
} else {
    set DUAL_SCREEN 0
}
#
# Windows and pages that do not depend on single or dual screen
#
set cam_page .null
set server_page .null
set topbox_page .null
set aber_page .null
set camera_page .null
set static_page .null
set mod_page .null
set save_page .null
set alignment_page .null
#
# Set names and positions of windows
#
if { $DUAL_SCREEN } {

    set main_win .main_win
    set main_position "800x820+0+0"

    set lgs_win .lgs_win
    set lgs_position "-0-0"

    set power_win .power_win
    set power_position "+100+500"

    set dclient_win .dclient_win
    set dclient_position "+10+10"

    set pupil_win .topbox_win
    set pupil_position "+100+50"

    set seeing_win .seeing_win
    set seeing_position "+800+0"

    set slew_win .seeing_win
    set slew_position "+800+0"

    set debug_win .debug_win
    set debug_position "+1300+0"

    set sc_win .sc_win
    set sc_position "+1500+0"

    set volt_win .volt_win
    set volt_position "+1200+0"

    set display_win .display_win
    set display_position "+800+60"

    set pgplot_win .pgplot_win
    set pgplot_position "+100+20"

} else {

    set main_win .main_win
    set main_position "900x920+0+0"

    set lgs_win .lgs_win
    set lgs_position "-0-0"

    set power_win .power_win
    set power_position "+100+500"

    set dclient_win .dclient_win
    set dclient_position "+730+0"

    set topbox_win .topbox_win
    set topbox_position "+10+10"

    set pupil_win .pupil_win
    set pupil_position "+100+50"

    set seeing_win .seeing_win
    set seeing_position "+800+0"

    set slew_win .seeing_win
    set slew_position "+800+0"

    set debug_win .debug_win
    set debug_position "+913+0"

    set sc_win .sc_win
    set sc_position "+500+0"

    set volt_win .volt_win
    set volt_position "+400+0"

    set display_win .display_win
    set display_position "+470+0"

    set pgplot_win .pgplot_win
    set pgplot_position "+100+20"
}

set sys_win .sys_win
set back_win .back_win
set static_win .static_win
set sdsu_win .sdsu_win
set blinc_win .blinc_win
set sky_win .sky_win
set align_win .align_win
set dnet_win .dnet_win
set filter_win .filter_win
set fsm_win .fsm_win
set indigo_win .indigo_win
set f15stage_win .f15stage_win
set info_win .info_win
set tcs_win .tcs_win
set hex_win .hex_win
set offset_win .offset_win
set rot_win .rot_win
