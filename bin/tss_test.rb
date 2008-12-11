#!/usr/bin/ruby

# The AO tss test server

require 'socket'
require 'timeout'

puts "environment variable TSSPORT is undefined.  Using #{ENV['TSSPORT']="7003"}\n" unless ENV['TSSPORT']

# all times are in seconds, floats OK

@@log_timeout = 1
@@log_cleanup_timeout = 0.1
@@server_print_timeout = 5

## start of main server command handlers
##

# all command handler
#
def science_all
    result = ""
    result += "total_current 42\n"
    result += "total_voltage 4.2\n"
    result += ".EOF\n"
end

# the main server
# and its utility routines

# get a stripped lowercase string. handle nil
def getstripdown (io)
    str = io.gets
    str = str.strip.downcase if str
    return str
end

# try to print "processing ..." back to client
def tprint (io, msg)
    timeout(@@server_print_timeout) do
	io.print msg
    end
end

# the idea here is to run a separate thread for each client
# so that multiple clients can connect at the same time.
# all threads should be independent of each other.
# since this is a long running program, we want to be sure
# that resources are not consumed ad nauseum.
# so we try to catch all exceptions so that all resources,
# memory, file descriptors, etc., are freed when the thread ends.
# we want no leaks.  the ruby system may do some of this for us,
# but the documentation is not clear on this, so we try to be
# very careful.  We also try to make sure nothing hangs, so all
# potential for that has timeouts.
# note that if a thread ends with an exception,
# ruby holds it until the main thread can reap it (with a join).
# Since this may tie up resources, and we really have no need to join,
# we avoid any unhandled exceptions.

# the protcol for this server is simple.
# the command is on a line by itself.
# any necessary arguments follow, one per line.
# the reply is a line starting with Done: or Error:

server = TCPServer.new(ENV['TSSPORT'])
while (new_session = server.accept)
    Thread.new(new_session) do |session|
	cmd = nil
	why = nil
        begin
	    while cmd = getstripdown(session)
		result = nil
		case cmd
		when "all"
		    result = science_all
		else
		    result = "Error: Unknown request: #{cmd}\n"
		    sc_log(result)
		end
		timeout(@@server_print_timeout) do
		    session.print result if result
		end
	    end
	    session.close
	rescue Exception => why
	end

	# try to send back an error message to the client,
	# but don't hang or cause an exception, if we can't.
	# the client is probably gone if we reach here anyway.
	if why
	    begin
		timeout(@@server_print_timeout) do
		    session.print("Error: #{cmd} #{why}\n")
		    session.close
		end
	    rescue Exception
	    end
	end
    end
end
