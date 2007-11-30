###################################
#
# debug_globals.tcl
#
###################################
#
# The *_globals.tcl file is sourced by the upper most level of TCL
#   source code (ie. AO_GUI_startup.tcl) as well as the all lower
#   level of tcl code that need these variables.
#
# The *_globals_set.tcl file is sourced by only the upper most level
#   TCL code (usually *_startup.tcl) so values are set to the global
#   variables only once.  In *_globals_set.tcl file, the trace command
#   can be used to force the update of gui variables, colors, buttons,
#   etc on all gui panels.
#
###################################
#
# We use Debug_$el instead of Debug($el) because these variables are linked
#   to C variables and I don't know how (if possible) to link a tcl array to
#   a C array and retain the name (eg $el)
#
global Debug_List
set Debug_List [ list \
		     Info\
		     Motor \
		     Msg \
		     TCS \
		     SDSU \
		     PCR \
		     BG \
		     Static \
		     Status \
		     Topbox \
		     Mod \
		     BG \
		     Utility \
		     ]

foreach el $Debug_List {
    global debug_$el
}
