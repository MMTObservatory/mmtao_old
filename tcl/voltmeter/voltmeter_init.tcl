###################################
#
# voltmeter_init.tcl
#
###################################
#
# Initialize the VoltMeter communication.
#
# The User Manual mentioned below is the document 3470-90003_users.pdf
#   available on the MMTAO wiki under DM:Power Supply:HP Voltmeter (?)
#
# Written  25mar04  Started with oap_init.tcl
#
# Altered   3jan07  DLM Converted to reading the resistance on the DM
#
#

proc voltmeter_init { parent_win } {

    global AO_GUI_HOME
    source $AO_GUI_HOME/tcl/voltmeter/voltmeter_globals.tcl

    global reply_data
#
# Configure all the channels we are going to use.  We can only send
#   one CONF command so all channels will have the same parameter
#   after this command.
#
# See User Manual, page 211, for more information about the CONFigure command
#
    set i 0
    set Setup($i) "CONF:RES 1000, (@101:115)"
#
# Now change the range of the desired channels with the SENS command. See
#   the User Manual, page 214
#
#    incr i
#    set Setup($i) "SENS:RES:RANG 1, (@102)"
#
# Setup the measuring procedure with the TRIGer command.  The parameters
#   SOURCE TIMER says that an internal timer is to be used.  See the User
#   Manual, page 228.
#
    incr i
    set Setup($i) "TRIG:SOURCE TIMER"
#
# The TIMER 1.0 sets the interval between scans of all configured channels
#   in seconds
#
    incr i
    set Setup($i) "TRIG:TIMER 1.0"
#
# COUNT 1 sets the number of times to perform the scan each time an INIT command
#   is given.  In this case, all channels are scanned only once
#
    incr i
    set Setup($i) "TRIG:COUNT 1"
    incr i
#
# The INIT command is sent elsewhere which initiates the scan of all channels
#   and then the measurements are READ.
#
    for { set j 0 } { $j < $i } { incr j } {

	set status [catch { VoltMeter_Cmd Set $Setup($j) } msg ]
	if { $status } {
	    tk_messageBox -message \
		"Error initializing the HP VoltMeter\n $reply_data" \
		-parent $parent_win \
		-icon error -type ok
	    return -1
	}
    }

    return 0
}
