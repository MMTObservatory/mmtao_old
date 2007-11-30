#####
#
# Procedure: browse_dir
#
#   This procedure used the built-in tk procedure tk_getOpenFile.
#     The purpose of this wrapper procedures is to be able to send
#     only the directory and file names from the calling routine
#     and the choosen directory and file names are automatically
#     put in the the variables sent so they can be stored and used
#     in higher level routines.
#
# Written  13may03  DLM  Started with open_browse_window
#
#
#####

proc browse_dir { title base dir parent} {

    upvar $base base_name
    upvar $dir dir_name

    set temp_dir [file join $base_name $dir_name ]

    set name [tk_chooseDirectory \
	    -initialdir $temp_dir \
	    -title "$title" \
	    -parent $parent]

    if {$name != ""} {
	if { ![file exist $name ] || ![file isdirectory $name ] } {
	    puts "$name directory does not exist"
	    set error 1
	} else {
	    set base_name [file dirname $name]
	    set dir_name [file tail $name]
	    set error 0
	}
    } else {
	set error 1
    }

    return $error
}
