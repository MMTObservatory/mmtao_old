
proc SC_Server { {win_title} {host} {port 7700} } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    set SC_Name $win_title

    socket -server SC_Accept $port
    set sock [socket $host $port]
    fconfigure $sock -buffering line
}

# Accept incoming client connections
proc SC_Accept { sock addr port } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    # Destroy any existing interpreter for socket.
    SC_Delete_Interp $sock

    # Create an interpreter for the socket
    interp create Interp_$sock
    load {} Tk Interp_$sock

    Interp_$sock eval {wm withdraw .}

    if { $SC_Name == "SC_Server" } {

      # Set up socket options for asynchronous input.
      fileevent $sock readable [list SC_Read $sock]
      fconfigure $sock -buffering line

      set SC_Name ""

    } else {

      # Set up socket options for asynchronous input.
      fileevent $sock readable [list SC_Read $sock ]
      fconfigure $sock -buffering line
      set SC_Connected 1
    }
}

# Reads data from client connection.
proc SC_Read { sock } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    if { [catch {set cmd [gets $sock]} msg] == 1 } {
      #dead client.. remove it from interp_list
      SC_Delete_Interp $sock
      return
    }
    
    # Check for errors
    if { [string compare $cmd ""] == 0 } {
	if { [eof $sock] } {
	    # Destroy interpreter.
	    SC_Delete_Interp $sock
	}
	return
    }

    set SC_Request($SC_Parameters) [ string trim $cmd ]
    incr SC_Parameters
#
# Check to see what request is being made (if it is the first request string)
#
    if { $SC_Parameters == "1" } {

	#
	# Check the request as a lower case string
	#
	set request [ string tolower $SC_Request(0) ]

	if { $request == "all" } {
	    set SC_Arguements 1
	} elseif { $request == "?" } {
	    set SC_Arguements 1
	} elseif { $request == "h" } {
	    set SC_Arguements 1
	} elseif { $request == "help" } {
	    set SC_Arguements 1
	} elseif { $request == "set_home" } {
	    set SC_Arguements 1
	} elseif { $request == "go_home" } {
	    set SC_Arguements 1
	} elseif { $request == "focus" } {
	    set SC_Arguements 2
	} elseif { $request == "static" } {
	    set SC_Arguements 3
	} elseif { $request == "offset" } {
	    set SC_Arguements 3
	} else {
	    set SC_Arguements 0
	}
    }

    if { $SC_Arguements == "0" } {

	#
	# The request was not recognized so send the client a message and
	#   reset the request list
	#
	SC_Log "sc_server: Error: Unknown request: $SC_Request(0)"
	SC_Write $sock "Error: Unknown request: $SC_Request(0)"
	set SC_Arguements 0
	for {set i 0} {$i < $SC_Parameters} { incr i } {
	    set SC_Request($i) --
	}
	set SC_Parameters 0

    } elseif { $SC_Arguements == $SC_Parameters } {

	#
	# Enough parameters were read for the current request so call the
	#   sc_processing routine to do the request
	#
	sc_processing $sock

    } elseif { $SC_Arguements < $SC_Parameters } {

	#
	# We should never get here, but just in case...
	#
	SC_Log "sc_server: Error: Unknown request: $SC_Request(0)"
	SC_Write $sock "Error: Too many parameter read for $SC_Request(0)"
	set SC_Arguements 0
	for {set i 0} {$i < $SC_Parameters} { incr i } {
	    set SC_Request($i) --
	}
	set SC_Parameters 0

    }

}

proc SC_Write { sock msg } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    if { $sock != 0 } {
	catch {puts $sock $msg} err
    }
}

# Deletes GUI interperter.
proc SC_Delete_Interp { sock } {

    global PCR_HOME
    source $PCR_HOME/tcl/sc_server/sc_globals.tcl

    if { [ interp exists Interp_$sock ] } {
	interp delete Interp_$sock
	close $sock
	set SC_Connected 0
    }
}
