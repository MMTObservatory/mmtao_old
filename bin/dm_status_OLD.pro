;--------------------------------------------------------------------------
; Project Name : TSS status window
; Author : Vidhya Vaitheeswaran
; Version : 1.0
; Date : Dec/2004
; Modifications made on Jan/05/2005
; Version : 1.1
; Date : Apr/08/2005
;        Included a signal LED button to display the status while updating
;--------------------------------------------------------------------------

; tjt - the following routine never fully worked right, but did
; help sort out some things, used in debug only
pro str_hex, str
    for i = 0B, 32B do begin
    	if str eq string(2B) then begin
		print, "<STX>"
		return
	endif
    	if str eq string(251B) then begin
		print, "<251>"
		return
	endif
    	if str eq string(255B) then begin
		print, "<255>"
		return
	endif
    	if str eq string(i) then begin
		print, "binary: " + i
		return
	endif
    end
    print, str
end

; another debug routine.
pro str_dumper, data
    n = strlen ( data )
    print, "tjt: dump: " + data
    print, "tjt: dump: " + string(n)
    for i = 0, n do begin
    	str_hex, strmid ( data, i, 1 )
    end
end

; -----------------------------------------
; Procedure to update the GUI
;  called whenever we have new data from the TSS
;  data is an array of lines
; -----------------------------------------

; why copy the data ?
; pro tss_update_param, Event,data
; tss = data

pro tss_update_param, Event, tss

; nlines = size(tss,/N_ELEMENTS)
; print, "tjt: tss data line count: ", string ( nlines )
; print, "tjt: first tss line is: " + tss[0]
; print, "tjt: second tss line is: " + tss[1]
; print, "tjt: nnext to last tss line is: " + tss[nlines-3]
; print, "tjt: next to last tss line is: " + tss[nlines-2]
; print, "tjt: last tss line is: " + tss[nlines-1]
; 
; str_dumper, tss[0]

; check for a valid block of TSS data.
if size(tss,/N_ELEMENTS) ne 27 then return

; Our test data had embedded 0x00, so the IDL string got
; corrupted (the STX after the null byte is never seen)
; I am not sure what this test really bought anyway
; (but it clearly worked with the actual TSS)
;if strpos ( tss[i], string(2B) ) ne -1 then begin

; The following is the original test
; if tss[0] eq string(2B) then return

i = 0

tss_id_2=widget_info(Event.top, FIND_BY_UNAME="tss_current_fld")
widget_control, tss_id_2, SET_VALUE=string(tss[i+1]/100.0, FORMAT="(F10.2)")
tss_id_3=widget_info(Event.top, FIND_BY_UNAME="power_vccp_fld")
widget_control, tss_id_3, SET_VALUE=string(tss[i+2]/100.0, FORMAT="(F10.2)")
tss_id_4=widget_info(Event.top, FIND_BY_UNAME="power_vssp_fld")
widget_control, tss_id_4, SET_VALUE=string(tss[i+3]/100.0, FORMAT="(F10.2)")
tss_id_5=widget_info(Event.top, FIND_BY_UNAME="voltage_vccp_fld")
widget_control, tss_id_5, SET_VALUE=string(tss[i+4]/1000.0, FORMAT="(F10.3)")
tss_id_6=widget_info(Event.top, FIND_BY_UNAME="voltage_vssp_fld")
widget_control, tss_id_6, SET_VALUE=string(tss[i+5]/1000.0, FORMAT="(F10.3)")
tss_id_7=widget_info(Event.top, FIND_BY_UNAME="voltage_vcca_fld")
widget_control, tss_id_7, SET_VALUE=string(tss[i+6]/1000.0, FORMAT="(F10.3)")
tss_id_9=widget_info(Event.top, FIND_BY_UNAME="voltage_vssa_fld")
widget_control, tss_id_9, SET_VALUE=string(tss[i+7]/1000.0, FORMAT="(F10.3)")
tss_id_8=widget_info(Event.top, FIND_BY_UNAME="voltage_vccl_fld")
widget_control, tss_id_8, SET_VALUE=string(tss[i+8]/1000.0, FORMAT="(F10.3)")

temp = 0.0
temp = tss[i+9]/100.0
tss_id_10=widget_info(Event.top, FIND_BY_UNAME="temp_crate1_fld")
widget_control, tss_id_10, SET_VALUE=string(temp, FORMAT="(F10.2)")
temp = tss[i+10]/100.0
tss_id_11=widget_info(Event.top, FIND_BY_UNAME="temp_crate2_fld")
widget_control, tss_id_11, SET_VALUE=string(temp, FORMAT="(F10.2)")
temp = tss[i+11]/100.0
tss_id_12=widget_info(Event.top, FIND_BY_UNAME="temp_crate3_fld")
widget_control, tss_id_12, SET_VALUE=string(temp, FORMAT="(F10.2)")
temp = tss[i+12]/100.0
tss_id_13=widget_info(Event.top, FIND_BY_UNAME="temp_water_inlet")
widget_control, tss_id_13, SET_VALUE=string(temp, FORMAT="(F10.2)")
tss_id_14=widget_info(Event.top, FIND_BY_UNAME="temp_water_outlet")
widget_control, tss_id_14, SET_VALUE=string(tss[i+13]/100.0, FORMAT="(F10.2)")
tss_id_15=widget_info(Event.top, FIND_BY_UNAME="temp_coldplate1")
widget_control, tss_id_15, SET_VALUE=string(tss[i+14]/100.0, FORMAT="(F10.2)")
tss_id_16=widget_info(Event.top, FIND_BY_UNAME="temp_coldplate2")
widget_control, tss_id_16, SET_VALUE=string(tss[i+15]/100.0, FORMAT="(F10.2)")
tss_id_17=widget_info(Event.top, FIND_BY_UNAME="temp_ambient")
widget_control, tss_id_17, SET_VALUE=string(tss[i+16]/100.0, FORMAT="(F10.2)")
tss_id_18=widget_info(Event.top, FIND_BY_UNAME="temp_humidity")
widget_control, tss_id_18, SET_VALUE=string(tss[i+17]/10.0, FORMAT="(F10.2)")+'%'
tss_id_19=widget_info(Event.top, FIND_BY_UNAME="pressure_inlet")
widget_control, tss_id_19, SET_VALUE=string(tss[i+18]/100.0, FORMAT="(F10.2)")
tss_id_20=widget_info(Event.top, FIND_BY_UNAME="pressure_outlet")
widget_control, tss_id_20, SET_VALUE=string(tss[i+19]/100.0, FORMAT="(F10.2)")
tss_id_21=widget_info(Event.top, FIND_BY_UNAME="max_crate_temp")
widget_control, tss_id_21, SET_VALUE=string(tss[i+20]/100.0, FORMAT="(F10.2)")
tss_id_22=widget_info(Event.top,FIND_BY_UNAME="current_on_tss")
widget_control, tss_id_22,SET_VALUE=string(tss[i+21],FORMAT='(a)')
tss_id_23=widget_info(Event.top,FIND_BY_UNAME="max_hold1")
widget_control,tss_id_23,SET_VALUE=string(tss[i+22],FORMAT="(a)")
tss_id_24=widget_info(Event.top,FIND_BY_UNAME="max_hold2")
widget_control,tss_id_24,SET_VALUE=string(tss[i+23],FORMAT="(a)")
tss_id_25=widget_info(Event.top,FIND_BY_UNAME="max_hold3")
widget_control,tss_id_25,SET_VALUE=string(tss[i+24],FORMAT="(a)")

tss_id_26=widget_info(Event.top,FIND_BY_UNAME="over_curr_vccp")
if (string(strmid(tss[i+25],0,1))) eq '1' then begin
widget_control,tss_id_26,SET_VALUE='Yes'
endif else begin
widget_control,tss_id_26,SET_VALUE='No'
endelse

tss_id_27=widget_info(Event.top,FIND_BY_UNAME="over_curr_vssp")
if (string(strmid(tss[i+25],1,1))) eq '1' then begin
widget_control,tss_id_27,SET_VALUE='Yes'
endif else begin
widget_control,tss_id_27,SET_VALUE='No'
endelse

tss_id_28=widget_info(Event.top,FIND_BY_UNAME="over_curr_push")
widget_control,tss_id_28,SET_VALUE=string(strmid(tss[i+25],2,1),FORMAT="(a)")
tss_id_29=widget_info(Event.top,FIND_BY_UNAME="over_curr_pull")
widget_control,tss_id_29,SET_VALUE=string(strmid(tss[i+25],3,1),FORMAT="(a)")
tss_id_29_1=widget_info(Event.top,FIND_BY_UNAME="max_hold4")
;widget_control,tss_id_29_1,SET_VALUE=string(strmid(tss[i+25],4,1),FORMAT="(a)")

tss_id_30=widget_info(Event.top,FIND_BY_UNAME="enable_coil_sw")
if (string(strmid(tss[i+25],5,1))) eq '1' then begin
widget_control,tss_id_30,SET_VALUE='ON'
endif else begin
widget_control,tss_id_30,SET_VALUE='OFF'
endelse

;widget_control,tss_id_30,SET_VALUE=string(strmid(tss[i+25],5,1),FORMAT="(a)")
tss_id_31=widget_info(Event.top,FIND_BY_UNAME="enable_coil_hw")
if (string(strmid(tss[i+25],6,1))) eq '1' then begin
widget_control,tss_id_31,SET_VALUE='ON'
endif else begin
widget_control,tss_id_31,SET_VALUE='OFF'
endelse

;widget_control,tss_id_31,SET_VALUE=string(strmid(tss[i+25],6,1),FORMAT="(a)")
tss_id_32=widget_info(Event.top,FIND_BY_UNAME="coil_status")
if (string(strmid(tss[i+25],7,1))) eq '1' then begin
widget_control,tss_id_32,SET_VALUE='Yes'
endif else begin
widget_control,tss_id_32,SET_VALUE='No'
endelse

;widget_control,tss_id_32,SET_VALUE=string(strmid(tss[i+25],7,1),FORMAT="(a)")
tss_id_33=widget_info(Event.top, FIND_BY_UNAME="tss_status_fld")
if (string(strmid(tss[i+25],8,1))) eq '1' then begin
widget_control,tss_id_33,SET_VALUE='SAFE MODE'
endif else begin
widget_control,tss_id_33,SET_VALUE='OPERATING MODE(CAUTION)'
endelse

;widget_control, tss_id_33, SET_VALUE=string(strmid(tss[i+25],8,1),FORMAT="(a)")
tss_id_34=widget_info(Event.top, FIND_BY_UNAME="tss_power_fld")
if (string(strmid(tss[i+25],10,1))) eq '1' then begin
widget_control,tss_id_34,SET_VALUE='ON'
endif else begin
widget_control,tss_id_34,SET_VALUE='OFF'
endelse

;widget_control, tss_id_34, SET_VALUE=string(strmid(tss[i+25],10,1),FORMAT="(a)")
tss_id_35=widget_info(Event.top, FIND_BY_UNAME="tss_fiber_fld")
if (string(strmid(tss[i+25],11,1))) eq '1' then begin
widget_control,tss_id_35,SET_VALUE='DISCONNECTED'
endif else begin
widget_control,tss_id_35,SET_VALUE='CONNECTED'
endelse

;widget_control, tss_id_35, SET_VALUE=string(strmid(tss[i+25],11,1),FORMAT="(a)")
tss_id_36=widget_info(Event.top, FIND_BY_UNAME="tss_command_fld")
if (string(strmid(tss[i+25],9,1))) eq '1' then begin
widget_control,tss_id_36,SET_VALUE='ON'
endif else begin
widget_control,tss_id_36,SET_VALUE='OFF'
endelse

;widget_control, tss_id_36, SET_VALUE=string(strmid(tss[i+25],9,1),FORMAT="(a)")
tss_id_37=widget_info(Event.top, FIND_BY_UNAME="tss_wrong_fld")
if (string(strmid(tss[i+25],12,1))) eq '1' then begin
widget_control,tss_id_37,SET_VALUE='Yes'
endif else begin
widget_control,tss_id_37,SET_VALUE='No'
endelse

;widget_control, tss_id_37, SET_VALUE=string(strmid(tss[i+25],12,1),FORMAT="(a)")
END

; -----------------------------------------
; Function to read a line from the TSS
; -----------------------------------------

function tss_read, tss_proc_buf, VERBOSE=verbose

;@adsec_common

;read data from tss via terminal server
common tss_lun_block,tss_lun

;connect_timeout = 1.0
;read_timeout = 0.2
;write_timeout = 0.2

com_connection_on = 0B
socket_connection_on = 1B

command = string(2B)+'J000'+string(3B)

; The real TSS via cyclades.
;tssip = 'mmtts2.mmto.arizona.edu'
;tssport = 7003

; TSS "demo" thingie on cholla
tssip = 'cholla.mmto.arizona.edu'
;tssport = 7200
tssport = 7400

if tss_lun eq 0 then begin
    socket, tss_lun, /GET_LUN, tssip, tssport, CONNECT_TIMEOUT=1.0,ERROR=err,READ_TIMEOUT=1.0
    if err ne 0 then begin
       ; msg = strjoin( "TSS CONNECT ERROR ( ", tssip, " )" )
       msg = "TSS CONNECT ERROR ( " + tssip + "/" + string(tssport) + " )"
       answ = dialog_message([msg],/ERROR)
       ok = 1
       return, ok
    endif
endif

; generate an array of 27 empty strings (just as many lines as we
; expect the TSS to return), this count apparently drives readf below,
; because it does in fact read 27 lines. The first (line 0) is the bogus binary
; junk (or if you are lucky, just the STX byte).  The last (line 26) is an
; empty line, but 1 through 25 are what you be wanting.

tss_proc_buf = strarr(27)

valid = 0 ; Valid flag
MAX_ATTEMPT=10
ATTEMPT=0

ON_IOERROR, bad_num
WHILE (valid EQ 0) AND (ATTEMPT LE MAX_ATTEMPT) DO BEGIN
    ; note that the following printf does append a newline we don't need.
    ; also note that readf gets an array of lines.
    printf,tss_lun,command
    wait,.1
    readf,tss_lun,tss_proc_buf
    VALID = 1
    bad_num:
	ATTEMPT=ATTEMPT+1
ENDWHILE

if ATTEMPT GT MAX_ATTEMPT then begin
    answ = dialog_message(["TSS COMMUNICATION ERROR"],/ERROR)
    not_ok = 1
    return, not_ok
endif else begin
    ok = 0
    return, ok
endelse
End

; -----------------------------------------
; Procedure to handle a GUI event
; -----------------------------------------

PRO dm_status_gui_event, event

common tss_block,dm_status
common debug_block,debug
common tss_lun_block,tss_lun
common red_block, red_led
common green_block,green_led
common base_block,bmp_button

if (event.id eq event.handler) then begin
    if(dm_status eq 1B) then begin
       widget_control,event.id,TIMER=1.0
       err =0
       err = tss_read(data,/verbose)
       if err eq 0 then begin
        ; widget_control,bmp_button, SET_VALUE = green_led
         wait,0.1
         tss_update_param,event,data
         ;widget_control,bmp_button, SET_VALUE= " "
       endif else begin
             ;widget_control,bmp_button, SET_VALUE= red_led
             dm_status = 0B

        endelse
    endif
endif else begin
    widget_control, event.id, GET_UVALUE=uvalue
    case uvalue of
       'exit' : begin
          widget_control,event.top,/DESTROY
	  if tss_lun ne 0 then begin
          free_lun,tss_lun
	  endif
          end
       'snooze' :  begin
	      if tss_lun ne 0 then begin	
              free_lun,tss_lun
              tss_lun=0
              dm_status = 0B
              endif
              end
       'update' : begin
                 dm_status = 1B
                 widget_control,event.handler, TIMER=1.0
                 end
        'bmp' : begin
               end
        endcase
end

END

; -----------------------------------------
; Procedure to set up the GUI
; -----------------------------------------

PRO dm_status, GROUP = group
common tss_block,dm_status
common debug_block,debug
common tss_lun_block,tss_lun
common red_block, red_led
common green_block,green_led
common base_block,bmp_button

device, true=24
dm_status = 1B
debug = 0B
tss_lun = 0

red_led = BYTARR(3, 20, 20)
red_led[0, *, *] = 255

green_led = BYTARR(3,20,20)

green_led[1, *, *] = 255


;create a top level widget
base1 = widget_base(row=3,title = "DM336 Diagnostic Data", $
    tlb_frame_attr=1, /frame)
WIDGET_CONTROL, /MANAGED, base1
;- create a main base to hold everything except buttons
main = widget_base(base1,col=2,frame=1)
first = widget_base(base1,col=2,/frame)
second = widget_base(base1,col=2,/frame)
third = widget_base(base1,col=2,/frame)
fourth = widget_base(base1,col=2,/frame)
fifth = widget_base(base1,col=2,/frame)
sixth = widget_base(base1,col=1,/frame)

;-create tss widgets

dmbase = widget_base(main,row=8,/grid_layout,/frame,/align_center)

slb3= widget_label(dmbase,value = "ADAPTIVE SECONDARY STATUS")
tss_status_fld = widget_text(dmbase, uname="tss_status_fld")

tlb2 = widget_label(dmbase,value = "Current Applied (Amps)",xsize=10)
tss_current_fld = widget_text(dmbase, uname="tss_current_fld",xoffset=1,$
    yoffset=1,/no_newline,value= '',xsize=1, ysize=1)
sbase = widget_base(first,row=7,/grid_layout,/frame,/align_left)
clc2= widget_label(sbase, value = "HW coil(red mushroom)")
enable_coil_sw = widget_text(sbase,uname = "enable_coil_sw")
clc3= widget_label(sbase, value = "SW coil(tss logic)")
enable_coil_hw = widget_text(sbase,uname = "enable_coil_hw")
slb2 = widget_label(sbase,value = "Coil Status(HW & SW)")
coil_status = widget_text(sbase,uname = "coil_status")
slb4= widget_label(sbase,value = "TSS Command")
tss_command_fld = widget_text(sbase, uname="tss_command_fld")
slb5= widget_label(sbase,value = "Power Status")
tss_powwer_fld = widget_text(sbase, uname="tss_power_fld")
slb6= widget_label(sbase,value = "Fiber to DM")
tss_fiber_fld = widget_text(sbase, uname="tss_fiber_fld")
slb7= widget_label(sbase,value = "Wrong Command sent")
tss_wrong_fld = widget_text(sbase, uname="tss_wrong_fld")

;-create DM power display
pbase = widget_base(main,row=7, /grid_layout,/align_left,/frame)

plb1 = widget_label(pbase,value = "VCC_P Current(A)")
power_vccp_fld = widget_text(pbase, uname="power_vccp_fld",xoffset=1,$
    yoffset=1,/no_newline,value= '',xsize=1, ysize=1)
plb2 = widget_label(pbase,value = "VSS_P Current(A)")
power_vssp_fld = widget_text(pbase, uname="power_vssp_fld",xoffset=1,$
    yoffset=1,/no_newline,value= '',xsize=1, ysize=1)
;vbase = widget_base(main,row=5, /grid_layout,/align_left,/frame)
vplb1 = widget_label(pbase,value = "VCC_P Voltage(V)")
voltage_vccp_fld = widget_text(pbase, uname="voltage_vccp_fld",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=1, ysize=1)
vplb2 = widget_label(pbase,value = "VSS_P Voltage(V)")
voltage_vssp_fld = widget_text(pbase, uname="voltage_vssp_fld",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=1, ysize=1)
vplb3 = widget_label(pbase,value = "VCC_A Voltage(V)")
voltage_vcca_fld = widget_text(pbase, uname="voltage_vcca_fld",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=1, ysize=1)
vplb4 = widget_label(pbase,value = "VSS_A Voltage(V)")
voltage_vssa_fld = widget_text(pbase, uname="voltage_vssa_fld",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=1, ysize=1)
vplb5 = widget_label(pbase,value = "VCC_L Voltage(V)")
voltage_vccl_fld = widget_text(pbase, uname="voltage_vccl_fld",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=1, ysize=1)



;- create DM atmospheric display
mbase = widget_base(third, row=6,/grid_layout,/align_left,/frame)
mlb2 = widget_label(mbase,value = "Temperature of Crate 1(C)")
temp_crate1_fld = widget_text(mbase, uname="temp_crate1_fld",xoffset=1,$
    yoffset=10,/no_newline,value='',xsize=3, ysize=1)
mlb6 = widget_label(mbase,value = "Water inlet Temperature(C)")
temp_water_inlet = widget_text(mbase, uname="temp_water_inlet",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=3, ysize=1)
mlb3 = widget_label(mbase,value = "Temperature of Crate 2(C)")
temp_crate2_fld = widget_text(mbase, uname="temp_crate2_fld",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=3, ysize=1)
mlb7 = widget_label(mbase,value = "Water outlet Temperature(C)")
temp_water_outlet = widget_text(mbase, uname="temp_water_outlet",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=3, ysize=1)
mlb4 = widget_label(mbase,value = "Temperature of Crate 3(C)")
temp_crate3_fld = widget_text(mbase, uname="temp_crate3_fld",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=3, ysize=1)


mlbi1 = widget_label(mbase,value="Coldplate[1] Temperature(C)")
temp_coldplate1 = widget_text(mbase, uname="temp_coldplate1",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=3, ysize=1)
mlpi3 = widget_label(mbase, value = "Max crate Temperature(C)")
max_crate_temp = widget_text(mbase,uname="max_crate_temp",xoffset=1,$
    yoffset=10,value = '',xsize=3,ysize=1)
mlbi2 = widget_label(mbase,value="Coldplate[2] Temperature(C)")
temp_coldplate2 = widget_text(mbase, uname="temp_coldplate2",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=3, ysize=1)
mlbi3 = widget_label(mbase,value ="Ambient Temperature(C)")
temp_ambient = widget_text(mbase, uname="temp_ambient",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=3, ysize=1)
mlbi4 = widget_label(mbase,value ="Ambient Humidity(RU)")
temp_humidity = widget_text(mbase, uname="temp_humidity",xoffset=1,$
    yoffset=10,/no_newline,value= '',xsize=3, ysize=1)

mlpi1 = widget_label(mbase,value = "Water inlet Pressure(PSI)")
pressure_inlet = widget_text(mbase,uname="pressure_inlet",xoffset=1,$
    yoffset=10,value ='',xsize=3,ysize=1)
mlpi2 = widget_label(mbase,value = "Water outlet Pressure(PSI)")
pressure_outlet = widget_text(mbase,uname="pressure_outlet",xoffset=1,$
    yoffset=10,value = '',xsize=3,ysize=1)

cbase = widget_base(fifth,row=4, /grid_layout,/frame, /align_left)
clb1 = widget_label(cbase, value = "Set Curr on TSS")
current_on_tss = widget_text(cbase,uname="current_on_tss",xsize=3,ysize=1)
clb2 = widget_label(cbase, value = "Max curren VCCP & VSSP rail")
max_hold1 = widget_text(cbase,uname = "max_hold1",xsize=3,ysize=1)
clb3 = widget_label(cbase, value = "Delta Curr for(Push)")
max_hold2 = widget_text(cbase,uname = "max_hold2",xsize=3,ysize=1)
clb4 = widget_label(cbase, value = "Delta Curr for(Pull)")
max_hold3 = widget_text(cbase,uname = "max_hold3",xsize=3,ysize=1)
clb5 = widget_label(cbase, value = "Overcurr on VCCP rail")
over_curr_vccp = widget_text(cbase,uname = "over_curr_vccp",xsize=3,ysize=1)
clb6 = widget_label(cbase, value = "Overcurr on VSSP rail")
over_curr_vssp = widget_text(cbase,uname = "over_curr_vssp")
clb7 = widget_label(cbase,value = "Overcurr on vccp-vssp")
over_curr_push = widget_text(cbase,uname = "over_curr_push")
clb8 = widget_label(cbase,value = "Overcur on vssp-vccp")
over_curr_pull = widget_text(cbase,uname = "over_curr_pull")
;clc1= widget_label(cbase, value = "Current Fault")
;max_hold4 = widget_text(cbase,uname = "max_hold4")
;clc2 = widget_label(cbase, value = "")

;bbase = widget_base(main, row=4,/frame,/align_right)
button1 = widget_button(dmbase, value="UPDATE", $
             xsize=175,uvalue='update')
button2 = widget_button(dmbase, value="SNOOZE",xsize=75,uvalue='snooze')
button3 = widget_button(dmbase, value="QUIT", xsize=75,uvalue='exit')
red_led = transpose (red_led,[1,2,0])
green_led = transpose (green_led, [1,2,0])
;bmp_button = widget_button(dmbase,VALUE=red_led,uvalue = 'bmp', /sensitive)
widget_control,base1, /realize

; tjt added this so it starts running by default
; (if you don't want this, set dm_status to 0B
if(dm_status eq 1B) then begin
  widget_control,base1, TIMER=1.0
endif

XMANAGER, CATCH=(debug eq 0B)
xmanager,'dm_status_gui',base1,/no_block, GROUP_LEADER= group

END

PRO Main
    dm_status
END

; THE END
