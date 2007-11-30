###################################
#
# pgplot_open.tcl
#
###################################
#
# Self calling prodecure to update the pgplot image
#
# Written   14may07    DLM  Started with smart_looper.tcl
#

proc pgplot_open { name path parent_win } {

    global PCR_HOME
    source $PCR_HOME/tcl/pgplot/pgplot_globals.tcl

    global reply_data

    set status [catch { PGplot_Cmd $name Open $path } msg ]
    if { $status } {
	tk_messageBox -message $reply_data \
	    -parent $parent_win \
	    -icon error -type ok
    }

}
