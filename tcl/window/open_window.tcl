#####
#
# Procedure: open_window
#
# Check to see if window has already been created.  There
#   are several possibilities: 
#         1) Window has never been created and thus no window name exits
#            in parent.  In this case, call the procedure which creates
#            the window.
#         2) Window has been create, but closed and thus appears to be
#            the same as 1) so call precedure to create the window.
#         3) Window has been created but not properly closed (ie destroyed)
#            and thus window is "withdrawn".  In this case must tell the
#            window manager to recreate it.
#         4) Window has been created but is "iconic".  Tell window
#            manager to deconify
#         5) Window has been created, "normal" but is hidden behind other windows.
#            Tell the window manager to raise the window to the top
#
# Argument check
#
#
#####

proc open_window { proc_name args } {

    set args [split $args { }]
    if { [llength $args] < 1 } {
	tk_messageBox -parent . -title "Argument Error" -type ok -icon error \
		-message "Wrong number of arguments in call to open_window.tcl. \
		Expected 2 and found [expr [llength $args]+1]"
	return 1
    }

    set win_name [ lindex $args 0]
    if { [winfo exists $win_name] } {
	set state [wm state $win_name]
	if { $state == "normal" } {
	    raise $win_name
	} elseif { $state == "iconic" || $state == "withdrawn" } {
	    wm deiconify $win_name
	}
    } else {
	$proc_name $args
    }
}
