#!/usr/bin/ruby

# A server for tailing log files
# The argument is the directory to search in.
# The glob pattern read on the socket is checked for
# funny characters that might allow the server to
# read outside the directory.  This is for security.
# The filenames matching the pattern are sorted.
# We assume that the filenames have a date/time stamp
# in them, so we just look at the last name in the
# sorted list, and assume that that one is the newest.
# Otherwise, we would have to check the modification
# time on each file, and that would take more time.

require 'resolv'
require 'socket'
require 'timeout'

unless ENV['LOGTAILPORT']
    begin
	srvname = ARGV[0]
	dns =  Resolv::DNS.open
	port = dns.getresource("_#{srvname}._tcp.mmto.arizona.edu", Resolv::DNS::Resource::IN::SRV).port.to_s
	ENV['LOGTAILPORT']=port
#       printf(STDERR, "%s", "variable LOGTAILPORT from DNS-SRV.  Using #{ENV['LOGTAILPORT']=port}\n")
    rescue Exception => why
        printf(STDERR, "%s", "variable LOGTAILPORT is undefined.  Using #{ENV['LOGTAILPORT']="7853"}\n")
    end
end

Dir.chdir(ARGV[1])

def logtail (session, pattern)
    lastfile = nil
    lastsize = 0

    while (true)
	file = Dir.glob(pattern, File::FNM_PATHNAME).sort.last
	# if no files match the pattern yet, just keep waiting
	if (! file.nil?)
	    if (file != lastfile)
		# if we're just starting, don't output anything old,
		# but if we're switching to a new file that just appeared,
		# output everything
		if (lastfile.nil?)
		    lastsize = File.stat(file).size
		else
		    lastsize = 0
		end
		lastfile = file
	    end
	    size = File.stat(file).size
	    # output any new stuff
	    if (size != lastsize)
		session.printf("%s", IO.read(file, size-lastsize, lastsize))
		session.flush
		lastsize = size
	    end
	end
	sleep (1)
    end
end

server = TCPServer.new(ENV['LOGTAILPORT'])
while (new_session = server.accept)
    Thread.new(new_session) do |session|
        begin
	    pattern = session.gets.strip
	    if (pattern.delete("A-Za-z_").empty?)
	        logtail(session, pattern + "*")
	    else
	        session.puts "illegal pattern"
	    end
	rescue Exception => why
#	    puts why
	ensure
	    session.close unless session.nil?
	end
    end
end
