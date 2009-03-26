#!/usr/bin/ruby

# The AO science server

require 'socket'
require 'timeout'

puts "environment variable SCIENCE_SERVER_SRVPORT is undefined.  Using #{ENV['SCIENCE_SERVER_SRVPORT']="7700"}\n" unless ENV['SCIENCE_SERVER_SRVPORT']
puts "environment variable HEXAPOD_SRVHOST is undefined.  Using #{ENV['HEXAPOD_SRVHOST']="hexapod"}\n" unless ENV['HEXAPOD_SRVHOST'] # hexapod.mmto.arizona.edu
puts "environment variable HEXAPOD_SRVPORT is undefined.  Using #{ENV['HEXAPOD_SRVPORT']="5340"}\n" unless ENV['HEXAPOD_SRVPORT']
puts "environment variable NGS_TOPBOX_SRVHOST is undefined.  Using #{ENV['NGS_TOPBOX_SRVHOST']="ao-server"}\n" unless ENV['NGS_TOPBOX_SRVHOST'] # ao-server.mmto.arizona.edu
puts "environment variable NGS_TOPBOX_STATUS_SRVPORT is undefined.  Using #{ENV['NGS_TOPBOX_STATUS_SRVPORT']="7400"}\n" unless ENV['NGS_TOPBOX_STATUS_SRVPORT']
puts "environment variable NGS_TOPBOX_SRVPORT is undefined.  Using #{ENV['NGS_TOPBOX_SRVPORT']="7401"}\n" unless ENV['NGS_TOPBOX_SRVPORT']
puts "environment variable NGS_INFO_SRVHOST is undefined.  Using #{ENV['NGS_INFO_SRVHOST']="ao-server"}\n" unless ENV['NGS_INFO_SRVHOST'] # ao-server.mmto.arizona.edu
puts "environment variable NGS_INFO_STATUS_SRVPORT is undefined.  Using #{ENV['NGS_INFO_STATUS_SRVPORT']="7500"}\n" unless ENV['NGS_INFO_STATUS_SRVPORT']
puts "environment variable NGS_INFO_SRVPORT is undefined.  Using #{ENV['NGS_INFO_SRVPORT']="7501"}\n" unless ENV['NGS_INFO_SRVPORT']
puts "environment variable MOUNT_SRVHOST is undefined.  Using #{ENV['MOUNT_SRVHOST']="mount"}\n" unless ENV['MOUNT_SRVHOST'] # mount.mmto.arizona.edu
puts "environment variable MOUNT_SRVPORT is undefined.  Using #{ENV['MOUNT_SRVPORT']="5240"}\n" unless ENV['MOUNT_SRVPORT']


puts "environment variable MATRIX_PATHNAME is undefined.  Using #{ENV['MATRIX_PATHNAME']="/home/mmtao/ao/tcl/static/robin_slopes2008_03_1.dat"}\n" unless ENV['MATRIX_PATHNAME']
puts "environment variable SCIENCE_LOG_FILENAME is undefined.  Using #{ENV['SCIENCE_LOG_FILENAME']="science_server_log.txt"}\n" unless ENV['SCIENCE_LOG_FILENAME']

@@matrix_pathname = ENV['MATRIX_PATHNAME']
@@Log_Filename = ENV['SCIENCE_LOG_FILENAME']

# all times are in seconds, floats OK

@@log_timeout = 1
@@log_cleanup_timeout = 0.1
@@server_print_timeout = 5
@@offset_timeout = 30
@@static_timeout = 30
@@focus_timeout = 30
@@update_tcs_timeout = 5
@@update_topbox_timeout = 5
@@update_info_timeout = 5
@@update_period = 1

# but these are in milliseconds
@@offset_closed_delay1 =  200
@@offset_closed_delay2 =   10
@@offset_closed_delay3 = 2000
@@offset_closed_delay4 =  500
@@offset_closed_delay5 =  500
@@offset_closed_delay6 = 2000

# all distances in arc seconds

@@offset_max = 40.0
@@offset_min = 0.01
@@offset_closed_max = 0.4
@@offset_closed_upper = 0.2
@@offset_closed_lower = -0.2
@@offset_total_upper = 50.0
@@offset_total_lower = -50.0

@@focus_max = 20.0	# microns

@@Science_Tip_Home  = "0.00"
@@Science_Tip_Off   = "0.00"
@@Science_Tilt_Home = "0.00"
@@Science_Tilt_Off  = "0.00"
@@Science_RA_Home   = "0.00"
@@Science_DEC_Home  = "0.00"
@@Science_Home_Set  = "0"

## this is the start of the status update thread
## it is called periodically to get all the latest
## info from the servers we need to talk to

# the status databases
@@TCS = Hash.new
@@TOPBOX = Hash.new
@@INFO = Hash.new

# interesting status variables
@@INFO_LIST = [
"Loop_Running",
"Loop_Gain",
"WFSC_Frequency",
"WFSC_Rotation",
"WFSC_Focus",
"Static_Astig_0",
"Static_Astig_45",
"Static_Coma_x",
"Static_Coma_y",
"Static_Trefoil_0",
"Static_Trefoil_30",
"Static_Spherical",
"Seeing_Value",
"Background_Dynamic",
"Background_Static",
"Background_File"
]
@@TCS_CHAR_LIST = [
"cat_id",
"cat_ra",
"cat_dec",
"cat_rapm",
"cat_decpm",
"cat_epoch",
"cat_epoch_str",
"ut",
"ra",
"dec"
]
@@TCS_FLOAT_LIST = [
"az",
"alt",
"rot",
"pa",
"rotpa",
"offrots",
"ao_off_alt",
"ao_off_az",
"ao_off_ra",
"ao_off_dec"
]
@@TOPBOX_FLOAT_LIST = [
"FSM_Tip_Position",
"FSM_Tilt_Position",
"OAP_Tip_Position",
"OAP_Tilt_Position",
"Indigo_Position",
"F15_Position",
"Focus_Position",
"Rotation_Position"
]
@@INFO_FLOAT_LIST = [
"WFSC_Period",
"WFSC_Temp_1",
"WFSC_Temp_2",
"WFSC_Pupil_Tip",
"WFSC_Pupil_Tilt",
"WFSC_Tip",
"WFSC_Tilt",
"WFSC_Focus",
"WFSC_Astig_0",
"WFSC_Astig_45",
"WFSC_Coma_X",
"WFSC_Coma_Y",
"WFSC_Trefoil_0",
"WFSC_Trefoil_30",
"WFSC_Spherical",
"DM_Tip",
"DM_Tilt",
"DM_Focus",
"DM_Astig_0",
"DM_Astig_45",
"DM_Coma_X",
"DM_Coma_Y",
"DM_Trefoil_0",
"DM_Trefoil_30",
"DM_Spherical",
"Seeing_Scale_Factor",
"Seeing_Period",
"Seeing_Value",
"Strehl_Value",
"Loop_Gain",
"Loop_Save_Data"
]

## status update utility routines
# check for the last line of an all command response
def not_last_line (line)
    return false if (line == ".EOF")
    return false if (line == "EOF")
    return true
end

# special formatting routines
def format_tcs (key, value)
    begin
	if (value != nil)
	    if (key == "alt")
		value = format("%.8f", value.to_f)
	    elsif (key == "az")
		value = format("%.8f", value.to_f)
	    elsif (@@TCS_FLOAT_LIST.include?(key))
		value = format("%.2f", value.to_f)
	    end
	end
    rescue Exception => why
    end
    return value
end

def format_topbox (key, value)
    begin
	if (value != nil)
	    if (@@TOPBOX_FLOAT_LIST.include?(key))
		value = format("%.2f", value.to_f)
	    end
	end
    rescue Exception => why
    end
    return value
end

def format_info (key, value)
    begin
	if (value != nil)
	    if (key == "WFSC_Period")
		value = format("%.1f", value.to_f)
	    elsif (key == "User_Gain")
		value = format("%.3f", value.to_f)
	    elsif (@@INFO_FLOAT_LIST.include?(key))
		value = format("%.2f", value.to_f)
	    end
	end
    rescue Exception => why
    end
    return value
end

# get all status from tcs
def update_tcs

    cmd = "update_tcs"
    why = nil
    result = nil
    io = nil
    begin
	timeout(@@update_tcs_timeout) do

	    io = TCPSocket.open(ENV['MOUNT_SRVHOST'],ENV['MOUNT_SRVPORT'].to_i)
	    io.puts("all")
	    while (not_last_line(line = io.gets.strip))
		words = line.split
		key = words[0]
		value = line.gsub(/^#{key}/,"").strip
		value = format_tcs(key, value)
		@@TCS[key] = value
	    end
	    io.close
	    io = nil

	    result = "Done: Update_tcs complete\n"
        end
    rescue Exception => why
	result = "Error: #{cmd} #{why}\n"
    end

    # try to clean up
    if why
	begin
	    timeout(@@server_print_timeout) do
		io.close if io
	    end
	rescue Exception
	end
    end
    return result
end

# get all status from topbox
def update_topbox
    cmd = "update_topbox"
    why = nil
    result = nil
    io = nil
    begin
	timeout(@@update_topbox_timeout) do

	    io = TCPSocket.open(ENV['NGS_TOPBOX_SRVHOST'],ENV['NGS_TOPBOX_STATUS_SRVPORT'].to_i)
	    io.puts("all")
	    while (not_last_line(line = io.gets.strip))
		words = line.split
		key = words[0]
		value = line.gsub(/^#{key}/,"").strip
		value = format_topbox(key, value)
		@@TOPBOX[key] = value
	    end
	    io.close
	    io = nil

	    # special case
	    @@INFO["WFSC_Rotation"] = @@TOPBOX["Rotation_Position"]

	    result = "Done: Update_topbox complete\n"
        end
    rescue Exception => why
	result = "Error: #{cmd} #{why}\n"
    end

    # try to clean up
    if why
	begin
	    timeout(@@server_print_timeout) do
		io.close if io
	    end
	rescue Exception
	end
    end
    return result
end

# get all status from info server
def update_info
    cmd = "update_info"
    why = nil
    result = nil
    io = nil
    begin
	timeout(@@update_info_timeout) do

	    io = TCPSocket.open(ENV['NGS_INFO_SRVHOST'],ENV['NGS_INFO_STATUS_SRVPORT'].to_i)
	    io.puts("all")
	    while (not_last_line(line = io.gets.strip))
		words = line.split
		key = words[0]
		value = line.gsub(/^#{key}/,"").strip
		value = format_info(key, value)
		@@INFO[key] = value
	    end
	    io.close
	    io = nil

	    result = "Done: Update_info complete\n"
        end
    rescue Exception => why
	result = "Error: #{cmd} #{why}\n"
    end

    # try to clean up
    if why
	begin
	    timeout(@@server_print_timeout) do
		io.close if io
	    end
	rescue Exception
	end
    end
    return result
end

def update_all
    update_tcs
    update_info
    update_topbox
end

## start the update thread now
Thread.new do
    while (true)
	update_all
	sleep(@@update_period)
    end
end

## end of status update thread code

## logging routines

def sc_log (msg)
    cmd = "log"
    why = nil
    result = nil
    io = nil
    begin
	timeout(@@log_timeout) do

	    timestamp = Time.now.utc
	    io = File.open(@@Log_Filename, "a")
	    io.puts "#{timestamp} #{msg}"

	    io.close
	    io = nil

	    result = "Done: Log complete\n"
	end
    rescue Exception => why
	result = "Error: #{cmd} #{why}\n"
    end

    # try to clean up
    if why
	begin
	    timeout(@@log_cleanup_timeout) do
		io.close if io
	    end
	rescue Exception
	end
    end
    return result
end

## start of main server command handlers
##

# all command handler
#
def science_all
    result = ""
    @@TCS_FLOAT_LIST.each { |key|
	value = @@TCS[key]
	if (value != nil)
	    result += "#{key} #{value}\n"
	end
    }
    @@TCS_CHAR_LIST.each { |key|
	value = @@TCS[key]
	if (value != nil)
	    result += "#{key} #{value}\n"
	end
    }
    @@INFO_LIST.each { |key|
	value = @@INFO[key]
	if (value != nil)
	    result += "#{key} #{value}\n"
	end
    }
    result += "Hexapod_Focus 0\n"
    result += "RTR_Reconstructor rtr_56m\n"
    result += "SC_Accepting 1\n"
    result += "offset_closed_delay1 #{@@offset_closed_delay1}\n"
    result += "offset_closed_delay2 #{@@offset_closed_delay2}\n"
    result += "offset_closed_delay3 #{@@offset_closed_delay3}\n"
    result += "offset_closed_delay4 #{@@offset_closed_delay4}\n"
    result += "offset_closed_delay5 #{@@offset_closed_delay5}\n"
    result += "EOF\n"
end

# static command handler
#

def science_static (arg1, arg2)
    cmd = "static"
    step = ""
    why = nil
    result = nil
    io = nil
    begin
	timeout(@@static_timeout) do

	    step = "matrix"
	    # get the matrix file
	    io = File.open(@@matrix_pathname, "r")

	    # skip leading comments
	    line = io.gets
	    line = io.gets while (line[0,1] == "#")

	    num_slopes = line.to_i
	    num_modes  = io.gets.to_i
	    mode_scale_factor = io.gets.to_f
	    mode_scale_factor *= 1.0e-18

	    scale = Array.new(num_modes, 0.0)
	    scale[0] = arg2.to_f	# astig 45
	    scale[1] = arg1.to_f	# astig  0
	    				# coma y
					# coma x

	    # calculate offsets
	    offsets = Array.new(num_slopes, 0.0)
	    offsets.each_index do |s|
		modes = io.gets.split
		modes.each_index do |m|
		    offsets[s] += modes[m].to_f * mode_scale_factor * scale[m]
		end
	    end
	    io.close
	    io = nil
	    step = ""

	    step = "info"
	    # now send to the INFO server
	    io = TCPSocket.open(ENV['NGS_INFO_SRVHOST'],ENV['NGS_INFO_SRVPORT'].to_i)
	    io.puts("slope_offset_set")
	    scale.each do |sm|
	        io.puts(sm.to_s)
	    end
	    io.print(offsets.pack("f*"))
	    io.close
	    io = nil
	    step = ""

	    result = "Done: Astigmatism application complete\n"
	end
    rescue Exception => why
	result = "Error: #{cmd} #{step} #{why}\n"
    end

    # try to clean up
    if why
	begin
	    timeout(@@server_print_timeout) do
		io.close if io
	    end
	rescue Exception
	end
    end
    return result
end

# focus command handler
#

def science_focus (arg)
    cmd = "focus"
    step = ""
    why = nil
    result = nil
    io = nil
    begin
	timeout(@@focus_timeout) do

	    if (arg.to_f.abs > @@focus_max)
	        raise "change too large"
	    end

	    step = "hexapod"
	    # run the hexapod command
	    io = TCPSocket.open(ENV['HEXAPOD_SRVHOST'],ENV['HEXAPOD_SRVPORT'].to_i)
	    io.puts("offset_inc wfs z #{arg}")
	    io.gets		# read back the "OK"
	    io.puts("apply_offsets")
	    io.gets		# read back the "OK"
	    io.close
	    io = nil
	    step = ""

	    step = "topbox"
	    # run the topbox command
	    io = TCPSocket.open(ENV['NGS_TOPBOX_SRVHOST'],ENV['NGS_TOPBOX_SRVPORT'].to_i)
	    io.puts("Focus Rel #{arg}")
	    # nothing to read back - this command makes no response
	    io.close
	    io = nil
	    step = ""

	    result = "Done: Focus complete\n"
	end
    rescue Exception => why
	result = "Error: #{cmd} #{step} #{why}\n"
    end

    # try to clean up
    if why
	begin
	    timeout(@@server_print_timeout) do
		io.close if io
	    end
	rescue Exception
	end
    end
    return result
end

# offset command handler
#

def science_offset (arg1, arg2)
    cmd = "offset"
    step = ""
    why = nil
    result = nil
    io = nil
    begin
	timeout(@@offset_timeout) do

	    ra  = arg1.to_f
	    dec = arg2.to_f

	    if (ra.abs > @@offset_max || dec.abs > @@offset_max)
	        raise "too large"
	    end

	    # There are three cases:
	    # If the loop is open,
	    #     we can just send the offsets to the topbox and tcs.
	    # If the loop is closed but the offset is not very small,
	    #     we have to set the gain to zero while we offset.
	    # If the loop is closed and the offset is very small,
	    #     we can offset without zeroing the gain,
	    #     but only in even smaller steps.

	    original_gain = @@INFO["Loop_Gain"]
	    loop_closed = (@@INFO["Loop_Running"] == "1")
	    offset_small = (Math.sqrt(ra*ra + dec*dec) <= @@offset_closed_max)

	    offset_open = !loop_closed
	    offset_closed_large = !offset_small && loop_closed
	    offset_closed_small =  offset_small && loop_closed

	    ra_left = ra
	    dec_left = dec

	    # check to see if the offset to too small to be worth doing
	    while (ra_left.abs > @@offset_min || dec_left.abs > @@offset_min)

		# bound the increments
	        if (offset_closed_small)
		    if (ra_left > @@offset_closed_upper)
		        ra = @@offset_closed_upper
		    elsif (ra_left < @@offset_closed_lower)
		        ra = @@offset_closed_lower
		    else
		        ra = ra_left
		    end
		    if (dec_left > @@offset_closed_upper)
		        dec = @@offset_closed_upper
		    elsif (dec_left < @@offset_closed_lower)
		        dec = @@offset_closed_lower
		    else
		        dec = dec_left
		    end
		end

		# check to make sure our total offsets for the night aren't too large
		total_ra_off  = ra  + @@TCS["ao_off_ra"].to_f
		total_dec_off = dec + @@TCS["ao_off_dec"].to_f
		unless (@@offset_total_lower < total_ra_off && total_ra_off < @@offset_total_upper)
		    raise "total ra too large"
		end
		unless (@@offset_total_lower < total_dec_off && total_dec_off < @@offset_total_upper)
		    raise "total dec too large"
		end

		if (offset_closed_large)

		    step = "info"
		    # set the gain to zero
		    io = TCPSocket.open(ENV['NGS_INFO_SRVHOST'],ENV['NGS_INFO_SRVPORT'].to_i)
		    io.puts("loop_gain\n0.00\n")
		    # nothing to read back - this command makes no response
		    io.close
		    io = nil
		    step = ""

		    sleep(@@offset_closed_delay1.to_f/1000.0)
		end

		# convert offsets on the sky from ra/dec to az/el
		angle = (@@TCS["pa"].to_f - @@TCS["rot"].to_f) * Math::PI / 180.0
		tip  = ra * Math.sin(angle) + dec * Math.cos(angle)
		tilt = ra * Math.cos(angle) - dec * Math.sin(angle)

		step = "topbox"
		# run the topbox commands
		io = TCPSocket.open(ENV['NGS_TOPBOX_SRVHOST'],ENV['NGS_TOPBOX_SRVPORT'].to_i)
		io.puts("FSM_Tip Rel #{tip}")
		# nothing to read back - this command makes no response
		io.puts("FSM_Tilt Rel #{tilt}")
		# nothing to read back - this command makes no response
		io.close
		io = nil
		step = ""

		if (offset_closed_large)
		    sleep(@@offset_closed_delay2.to_f/1000.0)
		end

		step = "tcs"
		# run the tcs command
		io = TCPSocket.open(ENV['MOUNT_SRVHOST'],ENV['MOUNT_SRVPORT'].to_i)
		io.puts("setrdao\n#{total_ra_off}\n#{total_dec_off}")
		io.gets		# read back the "OK"
		io.close
		io = nil
		step = ""

		if (offset_closed_large)
		    sleep(@@offset_closed_delay3.to_f/1000.0)

		    step = "info"
		    # set the gain to 0.05
		    io = TCPSocket.open(ENV['NGS_INFO_SRVHOST'],ENV['NGS_INFO_SRVPORT'].to_i)
		    io.puts("loop_gain\n0.05\n")
		    # nothing to read back - this command makes no response

		    sleep(@@offset_closed_delay4.to_f/1000.0)

		    # set the gain to original
		    io.puts("loop_gain\n#{original_gain}\n")
		    # nothing to read back - this command makes no response
		    io.close
		    io = nil
		    step = ""

		    sleep(@@offset_closed_delay5.to_f/1000.0)
		end

		if (offset_closed_small)
		    sleep(@@offset_closed_delay6.to_f/1000.0)
		end

		ra_left  -= ra
		dec_left -= dec
	    end

	    result = "Done: Offset complete\n"
	end
    rescue Exception => why
	result = "Error: #{cmd} #{step} #{why}\n"
    end

    # try to clean up
    if why
	begin
	    timeout(@@server_print_timeout) do
		io.close if io
	    end
	rescue Exception
	end
    end
    return result
end

# set_home command handler
#

def science_set_home
    cmd = "set_home"
    step = ""
    why = nil
    result = nil
    io = nil
    begin
	@@Science_Tip_Home  = @@TOPBOX["FSM_Tip_Position"]
	@@Science_Tip_Off   = "0.00"
	@@Science_Tilt_Home = @@TOPBOX["FSM_Tilt_Position"]
	@@Science_Tilt_Off  = "0.00"
	@@Science_RA_Home   = @@TCS["ao_off_ra"]
	@@Science_DEC_Home  = @@TCS["ao_off_dec"]
	@@Science_Home_Set  = "1"

	sc_log("Home Set")
	sc_log("    Science_Tip_Home  = #{@@Science_Tip_Home}")
	sc_log("    Science_Tilt_Home = #{@@Science_Tilt_Home}")
	sc_log("    Science_RA_Home   = #{@@Science_RA_Home}")
	sc_log("    Science_DEC_Home  = #{@@Science_DEC_Home}")

	result = "Done: Home Set\n"
    rescue Exception => why
	result = "Error: #{cmd} #{step} #{why}\n"
    end

    # try to clean up
    if why
	begin
	    timeout(@@server_print_timeout) do
		io.close if io
	    end
	rescue Exception
	end
    end
    return result
end

# go_home command handler
#

def science_go_home
    cmd = "go_home"
    step = ""
    why = nil
    result = nil
    io = nil
    begin
	ra  = @@Science_RA_Home.to_f  - @@TCS["ao_off_ra"].to_f
	dec = @@Science_DEC_Home.to_f - @@TCS["ao_off_dec"].to_f
	result = science_offset(ra.to_s, dec.to_s)

    rescue Exception => why
	result = "Error: #{cmd} #{step} #{why}\n"
    end

    # try to clean up
    if why
	begin
	    timeout(@@server_print_timeout) do
		io.close if io
	    end
	rescue Exception
	end
    end
    return result
end

# set command handler
#

def science_set (arg1, arg2)
    cmd = "set"
    step = ""
    why = nil
    result = nil
    io = nil
    begin
	case arg1
	when "offset_closed_delay1"
	    @@offset_closed_delay1 = arg2.to_i
	when "offset_closed_delay2"
	    @@offset_closed_delay2 = arg2.to_i
	when "offset_closed_delay3"
	    @@offset_closed_delay3 = arg2.to_i
	when "offset_closed_delay4"
	    @@offset_closed_delay4 = arg2.to_i
	when "offset_closed_delay5"
	    @@offset_closed_delay5 = arg2.to_i
	else
	    raise "#{arg1} #{arg2} - unknown parameter"
	end

	result = "Done: Set complete\n"
    rescue Exception => why
	result = "Error: #{cmd} #{step} #{why}\n"
    end

    # try to clean up
    if why
	begin
	    timeout(@@server_print_timeout) do
		io.close if io
	    end
	rescue Exception
	end
    end
    return result
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

server = TCPServer.new(ENV['SCIENCE_SERVER_SRVPORT'])
while (new_session = server.accept)
    Thread.new(new_session) do |session|
	cmd = nil
	why = nil
        begin
	    while cmd = getstripdown(session)
		result = nil
		case cmd
                when ""
                    next
		when "all"
		    result = science_all
		when "set_home"
		    sc_log("#{cmd}")
		    tprint(session, "OK: Processing #{cmd}...\n")
		    result = science_set_home
		    sc_log(result)
		when "go_home"
		    sc_log("#{cmd}")
		    tprint(session, "OK: Processing #{cmd}...\n")
		    result = science_go_home
		    sc_log(result)
		when "focus"
		    break unless arg = getstripdown(session)
		    sc_log("#{cmd} #{arg}")
		    tprint(session, "OK: Processing #{cmd}...\n")
		    result = science_focus(arg)
		    sc_log(result)
		when "static"
		    break unless arg1 = getstripdown(session)
		    break unless arg2 = getstripdown(session)
		    sc_log("#{cmd} #{arg1} #{arg2}")
#		    tprint(session, "OK: Processing #{cmd}...\n")
		    result = science_static(arg1, arg2)
		    sc_log(result)
		when "offset"
		    break unless arg1 = getstripdown(session)
		    break unless arg2 = getstripdown(session)
		    sc_log("#{cmd} #{arg1} #{arg2}")
		    tprint(session, "OK: Processing #{cmd}...\n")
		    result = science_offset(arg1, arg2)
		    sc_log(result)
		when "set"
		    break unless arg1 = getstripdown(session)
		    break unless arg2 = getstripdown(session)
		    sc_log("#{cmd} #{arg1} #{arg2}")
		    result = science_set(arg1, arg2)
		    sc_log(result)
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
