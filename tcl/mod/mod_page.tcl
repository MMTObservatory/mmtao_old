################################################## 
#
# mod_page.tcl
#
####
#
# Create window to apply modulated slope offsets
#
# Written  9jan06  DLM  Started with static_page.tcl
#
# Almost totally rewritten 23feb10 Skip according to Vidhya's request
##################################################

proc mod_page { mod_win page } {

    global PCR_HOME
    source $PCR_HOME/tcl/mod/mod_globals.tcl
#
    upvar #0 Info Info
#
    global reply_data

    global USER_HOME
    global SlopeOffsetDir
    global ModOffsetDir
    global ModRateDir
    set SlopeOffsetDir $USER_HOME
    set ModOffsetDir $USER_HOME
    set ModRateDir $USER_HOME
#
# Create the tix NoteBook frame
#
    global mod_page
    set mod_page [ $mod_win subwidget $page ]
#
# Build frames for slope offsets and mod offsets
#
    frame $mod_page.slope -relief ridge -border 4
    label $mod_page.slope.label -text "slope offsets"
    label $mod_page.slope.entry_label -text "slope offset pathname"
    entry $mod_page.slope.entry -width 40 -relief sunken -textvar SlopeOffsetPath
    button $mod_page.slope.browse -text "Browse" \
        -command {
            global SlopeOffsetDir
            set SlopeOffsetFile ""
            set status [browse_file \
                            "slope offset pathname" \
                            { "slope offset file" { .slopeoffset }} \
                            SlopeOffsetDir \
                            SlopeOffsetFile \
                            $mod_page ]
            if { $status } {
		set SlopeOffsetDir $USER_HOME
		set SlopeOffsetFile ""
                set SlopeOffsetPath ""
            } else {
                set SlopeOffsetPath [file join $SlopeOffsetDir $SlopeOffsetFile ]
            }
        }
    button $mod_page.slope.set -text "Set" \
        -command {
        }
    button $mod_page.slope.on -text "On" \
        -command {
            set status [catch { PCR_Cmd slope_offset_on } msg ]
            if { $status }  {
	        tk_messageBox -message "Error setting slope offset on" \
                    -parent $mod_win -icon error -type ok
	    }
        }
    button $mod_page.slope.off -text "Off" \
        -command {
            set status [catch { PCR_Cmd slope_offset_off } msg ]
            if { $status }  {
	        tk_messageBox -message "Error setting slope offset off" \
                    -parent $mod_win -icon error -type ok
	    }
        }

    set i 0
    grid config $mod_page.slope.label -row $i -column 0 -columnspan 6
    incr i
    grid config $mod_page.slope.entry_label -row $i -column 0
    grid config $mod_page.slope.entry -row $i -column 1 -columnspan 3
    grid config $mod_page.slope.browse -row $i -column 4
    grid config $mod_page.slope.set -row $i -column 5
    incr i
    grid config $mod_page.slope.on  -row $i -column 2
    grid config $mod_page.slope.off -row $i -column 3

    frame $mod_page.mod -relief ridge -border 4
    label $mod_page.mod.label -text "mod offsets"
    label $mod_page.mod.offset_entry_label -text "mod offset pathname"
    entry $mod_page.mod.offset_entry -width 40 -relief sunken -textvar ModOffsetPath
    button $mod_page.mod.offset_browse -text "Browse" \
        -command {
            global ModOffsetDir
            set ModOffsetFile ""
            set status [browse_file \
                            "mod offset pathname" \
                            { "mod offset file" { .modoffset }} \
                            ModOffsetDir \
                            ModOffsetFile \
                            $mod_page ]
            if { $status } {
		set ModOffsetDir $USER_HOME
		set ModOffsetFile ""
                set ModOffsetPath ""
            } else {
                set ModOffsetPath [file join $ModOffsetDir $ModOffsetFile ]
            }
        }
    button $mod_page.mod.offset_set -text "Set" \
        -command {
        }
    label $mod_page.mod.rate_entry_label -text "mod rate pathname"
    entry $mod_page.mod.rate_entry -width 40 -relief sunken -textvar ModRatePath
    button $mod_page.mod.rate_browse -text "Browse" \
        -command {
            global ModRateDir
            set ModRateFile ""
            set status [browse_file \
                            "mod rate pathname" \
                            { "mod rate file" { .modrate }} \
                            ModRateDir \
                            ModRateFile \
                            $mod_page ]
            if { $status } {
		set ModRateDir $USER_HOME
		set ModRateFile ""
                set ModRatePath ""
            } else {
                set ModRatePath [file join $ModRateDir $ModRateFile ]
            }
        }
    button $mod_page.mod.rate_set -text "Set" \
        -command {
            mod_rate_send $ModRatePath $mod_page
        }
    button $mod_page.mod.on -text "On" \
        -command {
            set status [catch { PCR_Cmd mod_offset_on } msg ]
            if { $status }  {
	        tk_messageBox -message "Error setting mod offset on" \
                    -parent $mod_win -icon error -type ok
	    }
        }
    button $mod_page.mod.off -text "Off" \
        -command {
            set status [catch { PCR_Cmd mod_offset_off } msg ]
            if { $status }  {
	        tk_messageBox -message "Error setting mod offset off" \
                    -parent $mod_win -icon error -type ok
	    }
        }

    set i 0
    grid config $mod_page.mod.label -row $i -column 0 -columnspan 6
    incr i
    grid config $mod_page.mod.offset_entry_label -row $i -column 0
    grid config $mod_page.mod.offset_entry -row $i -column 1 -columnspan 3
    grid config $mod_page.mod.offset_browse -row $i -column 4
    grid config $mod_page.mod.offset_set -row $i -column 5
    incr i
    grid config $mod_page.mod.rate_entry_label -row $i -column 0
    grid config $mod_page.mod.rate_entry -row $i -column 1 -columnspan 3
    grid config $mod_page.mod.rate_browse -row $i -column 4
    grid config $mod_page.mod.rate_set -row $i -column 5
    incr i
    grid config $mod_page.mod.on  -row $i -column 2
    grid config $mod_page.mod.off -row $i -column 3

    pack $mod_page.slope $mod_page.mod -side top

}
