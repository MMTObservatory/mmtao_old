################################################## 
#
# tss_page.tcl
#
####
#
# Create window to deal with tss stuff.
#
# 2008-11-03 skip
#
##################################################

proc tss_page { tss_win page } {

#
    upvar #0 TSS TSS
#
#
# Create the tix NoteBook frame
#
    global tss_page
    set tss_page [ $tss_win subwidget $page ]
#
# Build the page
#
    frame $tss_page.data0 -relief ridge -border 4
#
# 
#
    label $tss_page.data0.l_tss_status -text "tss_status"
    label $tss_page.data0.v_tss_status -textvar TSS(tss_status) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_total_current -text "total_current"
    label $tss_page.data0.v_total_current -textvar TSS(total_current) -bg grey84 -relief sunken -width 10

    label $tss_page.data0.l_power_vccp -text "power_vccp"
    label $tss_page.data0.v_power_vccp -textvar TSS(power_vccp) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_power_vssp -text "power_vssp"
    label $tss_page.data0.v_power_vssp -textvar TSS(power_vssp) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_voltage_vccp -text "voltage_vccp"
    label $tss_page.data0.v_voltage_vccp -textvar TSS(voltage_vccp) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_voltage_vssp -text "voltage_vssp"
    label $tss_page.data0.v_voltage_vssp -textvar TSS(voltage_vssp) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_voltage_vcca -text "voltage_vcca"
    label $tss_page.data0.v_voltage_vcca -textvar TSS(voltage_vcca) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_voltage_vssa -text "voltage_vssa"
    label $tss_page.data0.v_voltage_vssa -textvar TSS(voltage_vssa) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_voltage_vccl -text "voltage_vccl"
    label $tss_page.data0.v_voltage_vccl -textvar TSS(voltage_vccl) -bg grey84 -relief sunken -width 10

    label $tss_page.data0.l_temp_crate1 -text "temp_crate1"
    label $tss_page.data0.v_temp_crate1 -textvar TSS(temp_crate1) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_temp_crate2 -text "temp_crate2"
    label $tss_page.data0.v_temp_crate2 -textvar TSS(temp_crate2) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_temp_crate3 -text "temp_crate3"
    label $tss_page.data0.v_temp_crate3 -textvar TSS(temp_crate3) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_max_crate_temp -text "max_crate_temp"
    label $tss_page.data0.v_max_crate_temp -textvar TSS(max_crate_temp) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_temp_ambient -text "temp_ambient"
    label $tss_page.data0.v_temp_ambient -textvar TSS(temp_ambient) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_pressure_inlet -text "pressure_inlet"
    label $tss_page.data0.v_pressure_inlet -textvar TSS(pressure_inlet) -bg grey84 -relief sunken -width 10

    label $tss_page.data0.l_temp_water_inlet -text "temp_water_inlet"
    label $tss_page.data0.v_temp_water_inlet -textvar TSS(temp_water_inlet) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_temp_water_outlet -text "temp_water_outlet"
    label $tss_page.data0.v_temp_water_outlet -textvar TSS(temp_water_outlet) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_temp_coldplate1 -text "temp_coldplate1"
    label $tss_page.data0.v_temp_coldplate1 -textvar TSS(temp_coldplate1) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_temp_coldplate2 -text "temp_coldplate2"
    label $tss_page.data0.l_humidity_ambient -text "humidity_ambient"
    label $tss_page.data0.v_humidity_ambient -textvar TSS(humidity_ambient) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.v_temp_coldplate2 -textvar TSS(temp_coldplate2) -bg grey84 -relief sunken -width 10
    label $tss_page.data0.l_pressure_outlet -text "pressure_outlet"
    label $tss_page.data0.v_pressure_outlet -textvar TSS(pressure_outlet) -bg grey84 -relief sunken -width 10

    proc tss_set {variable value} {
	set string "set\n$variable\n$value"
        set status [catch { TSS_Cmd send $string} msg ]
    }
#
# Put this panel together
#
    set c 0 ; set r 0
    grid config $tss_page.data0.l_tss_status -row $r -column $c ; incr r
    grid config $tss_page.data0.l_total_current -row $r -column $c ; incr r
    incr c ; set r 0
    grid config $tss_page.data0.v_tss_status -row $r -column $c ; incr r
    grid config $tss_page.data0.v_total_current -row $r -column $c ; incr r
    incr c ; set r 0
    grid config $tss_page.data0.l_power_vccp -row $r -column $c ; incr r
    grid config $tss_page.data0.l_power_vssp -row $r -column $c ; incr r
    grid config $tss_page.data0.l_voltage_vccp -row $r -column $c ; incr r
    grid config $tss_page.data0.l_voltage_vssp -row $r -column $c ; incr r
    grid config $tss_page.data0.l_voltage_vcca -row $r -column $c ; incr r
    grid config $tss_page.data0.l_voltage_vssa -row $r -column $c ; incr r
    grid config $tss_page.data0.l_voltage_vccl -row $r -column $c ; incr r
    incr c ; set r 0
    grid config $tss_page.data0.v_power_vccp -row $r -column $c ; incr r
    grid config $tss_page.data0.v_power_vssp -row $r -column $c ; incr r
    grid config $tss_page.data0.v_voltage_vccp -row $r -column $c ; incr r
    grid config $tss_page.data0.v_voltage_vssp -row $r -column $c ; incr r
    grid config $tss_page.data0.v_voltage_vcca -row $r -column $c ; incr r
    grid config $tss_page.data0.v_voltage_vssa -row $r -column $c ; incr r
    grid config $tss_page.data0.v_voltage_vccl -row $r -column $c ; incr r
    incr c ; set r 0
    grid config $tss_page.data0.l_temp_crate1 -row $r -column $c ; incr r
    grid config $tss_page.data0.l_temp_crate2 -row $r -column $c ; incr r
    grid config $tss_page.data0.l_temp_crate3 -row $r -column $c ; incr r
    grid config $tss_page.data0.l_max_crate_temp -row $r -column $c ; incr r
    grid config $tss_page.data0.l_temp_ambient -row $r -column $c ; incr r
    grid config $tss_page.data0.l_pressure_inlet -row $r -column $c ; incr r
    incr c ; set r 0
    grid config $tss_page.data0.v_temp_crate1 -row $r -column $c ; incr r
    grid config $tss_page.data0.v_temp_crate2 -row $r -column $c ; incr r
    grid config $tss_page.data0.v_temp_crate3 -row $r -column $c ; incr r
    grid config $tss_page.data0.v_max_crate_temp -row $r -column $c ; incr r
    grid config $tss_page.data0.v_temp_ambient -row $r -column $c ; incr r
    grid config $tss_page.data0.v_pressure_inlet -row $r -column $c ; incr r
    incr c ; set r 0
    grid config $tss_page.data0.l_temp_water_inlet -row $r -column $c ; incr r
    grid config $tss_page.data0.l_temp_water_outlet -row $r -column $c ; incr r
    grid config $tss_page.data0.l_temp_coldplate1 -row $r -column $c ; incr r
    grid config $tss_page.data0.l_temp_coldplate2 -row $r -column $c ; incr r
    grid config $tss_page.data0.l_humidity_ambient -row $r -column $c ; incr r
    grid config $tss_page.data0.l_pressure_outlet -row $r -column $c ; incr r
    incr c ; set r 0
    grid config $tss_page.data0.v_temp_water_inlet -row $r -column $c ; incr r
    grid config $tss_page.data0.v_temp_water_outlet -row $r -column $c ; incr r
    grid config $tss_page.data0.v_temp_coldplate1 -row $r -column $c ; incr r
    grid config $tss_page.data0.v_temp_coldplate2 -row $r -column $c ; incr r
    grid config $tss_page.data0.v_humidity_ambient -row $r -column $c ; incr r
    grid config $tss_page.data0.v_pressure_outlet -row $r -column $c ; incr r
    incr c ; set r 0
#
#---------------------------------------------------
# Final packing
#---------------------------------------------------
#
    pack $tss_page.data0 -side top -anchor w

}
