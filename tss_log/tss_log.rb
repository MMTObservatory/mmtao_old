#!/usr/bin/ruby

data = Hash.new

loop {
     date = `date -u +"%D %H:%M:%S"`.chomp
     `echo "all" | nc ao-server 7003`.each_line { |l|
     	   l.chomp!
     	   file = l.split(' ')[0]
	   system("echo \"#{date} #{l}\" >> logs/#{file}")
      }
      sleep(1)
}
