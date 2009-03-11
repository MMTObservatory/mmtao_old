#!/usr/bin/ruby

require 'resolv'

def srv_lookup(name)
  srv = "_#{name}._tcp.mmto.arizona.edu"
  dns =  Resolv::DNS.open
  port = dns.getresource(srv, Resolv::DNS::Resource::IN::SRV).port
  host = dns.getresource(srv, Resolv::DNS::Resource::IN::SRV).target.to_s
  return host, port
end

data = Hash.new

keys = [
"voltage_vssp",
"voltage_vssa",
"voltage_vccp",
"voltage_vccl",
"voltage_vcca",
"total_current",
"temp_water_outlet",
"temp_water_inlet",
"temp_crate3",
"temp_crate2",
"temp_crate1",
"temp_coldplate2",
"temp_coldplate1",
"temp_ambient",
"pressure_outlet",
"pressure_inlet",
"power_vssp",
"power_vccp",
"humidity_ambient"
]

host, port = srv_lookup('tss-server')

loop {
  `echo "all" | nc #{host} #{port}`.each_line { |l|
    l.chomp!
    key = l.split(' ')[0]
    val = l.split(' ')[1]
    data[key] = val
  }

  string = "N"
  keys.each { |key|
    string = string + ":#{data[key]}"
  }

  system("rrdtool update /home/mmtao/ao/tss_log/tss.rrd #{string}")

  sleep(5)
}

