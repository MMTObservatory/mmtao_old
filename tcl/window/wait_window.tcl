#####
#
# Procedure: wait_window
#
# This procedure pops up a window asking the user to wait, along with an
#   informative message.
#
# Arguments"
#     <wait_win> - The name of this window.  This is a passed name so the
#                    the calling procedure can later destroy the window once
#                    the time consuming request has completed.
#   <parent_win> - Path to the parent window.  This is used to place this
#                    window in the center of its parent window.
#          <msg> - A short informative message to display 
#
# Written  8apr03  DLM
#
#####

proc wait_window { wait_win parent_win msg } {

    if { [winfo exists $wait_win] } {
	destroy $wait_win
    }

    toplevel $wait_win
    wm title $wait_win "Wait, processing request..."

# The informative message is passed to this Wait Window in the call

    frame $wait_win.box -relief ridge -border 4
    label $wait_win.box.wait -text "Please wait..." -bg red
    label $wait_win.box.msg -text $msg

    pack $wait_win.box.wait \
	-side top -anchor center
    pack $wait_win.box.msg \
	-side top -anchor e

    pack $wait_win.box \
	-side top -anchor e

# Force this Wait Window to be displayed in the middle of its parent
#   window.  No widgets are places in the window.

    ::tk::PlaceWindow $wait_win widget $parent_win

# Force the widgets (ie message) to be displayed

    update

}
