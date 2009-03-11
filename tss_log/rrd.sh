#!/bin/sh

rrdtool create tss.rrd -b 1100000000 -s 5 \
DS:voltage_vssp:GAUGE:400:U:U \
DS:voltage_vssa:GAUGE:400:U:U \
DS:voltage_vccp:GAUGE:400:U:U \
DS:voltage_vccl:GAUGE:400:U:U \
DS:voltage_vcca:GAUGE:400:U:U \
DS:total_current:GAUGE:400:U:U \
DS:temp_water_outlet:GAUGE:400:U:U \
DS:temp_water_inlet:GAUGE:400:U:U \
DS:temp_crate3:GAUGE:400:U:U \
DS:temp_crate2:GAUGE:400:U:U \
DS:temp_crate1:GAUGE:400:U:U \
DS:temp_coldplate2:GAUGE:400:U:U \
DS:temp_coldplate1:GAUGE:400:U:U \
DS:temp_ambient:GAUGE:400:U:U \
DS:pressure_outlet:GAUGE:400:U:U \
DS:pressure_inlet:GAUGE:400:U:U \
DS:power_vssp:GAUGE:400:U:U \
DS:power_vccp:GAUGE:400:U:U \
DS:humidity_ambient:GAUGE:400:U:U \
RRA:AVERAGE:0.5:1:720 \
RRA:AVERAGE:0.5:6:2880 \
RRA:AVERAGE:0.5:180:2976 \
RRA:AVERAGE:0.5:17280:1850 
