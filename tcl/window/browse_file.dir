#####
#
# Procedure: browse_file
#
#   This procedure used the built-in tk procedure tk_getOpenFile.
#     The purpose of this wrapper procedures is to be able to send
#     only the directory and file names from the calling routine
#     and the choosen directory and file names are automatically
#     put in the the variables sent so they can be stored and used
#     in higher level routines.
#
# Written    Feb00  DLM
#
# Altered  14Feb01  DLM  The previous version sent not only the
#                          directory and filename, but also the full
#                          file name (directory+path) and the parent
#                          window.  These last two were did not need
#                          to be sent back to the calling routine and
#                          were removed.
#
# Altered   2feb06  DLM  Rename from open_browse_window.tcl
#
#
#####

proc browse_file { title file_suffix dir file parent} {

    upvar $dir dir_name
    upvar $file file_name

    set file_types "{ $file_suffix } \
	    { \"Text Files\" { .txt .TXT } } \
	    { \"All Files\" * } "

    set name [tk_getOpenFile \
	    -initialdir $dir_name \
	    -filetypes $file_types \
	    -title "$title" \
	    -parent $parent]

    if {$name != ""} {
	set dir_name [file dirname $name]
	set file_name [file tail $name]
	set error 0
    } else {
	set error 1
    }

    return $error
}
