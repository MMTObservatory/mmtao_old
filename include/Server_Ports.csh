#!/bin/csh

#set mmtservbin = /mmt/bin/
set mmtservbin = ${MMTAO_HOME}/bin/

if (! $?PCR_SRVHOST) then
    setenv PCR_SRVHOST `${mmtservbin}mmtservhost pcr`
    setenv PCR_SRVPORT `${mmtservbin}mmtservport pcr`
    setenv PCR-WFSC_SRVPORT `${mmtservbin}mmtservport pcr-wfsc`
    setenv PCR-DM_SRVPORT `${mmtservbin}mmtservport pcr-dm`
    setenv PCR-SLOPES_SRVPORT `${mmtservbin}mmtservport pcr-slopes`
    setenv PCR-ERROR_SRVPORT `${mmtservbin}mmtservport pcr-error`

    setenv NGS-INFO_SRVHOST `${mmtservbin}mmtservhost ngs-info`
    setenv NGS-INFO_SRVPORT `${mmtservbin}mmtservport ngs-info`
    setenv NGS-INFO-WFSC_SRVPORT `${mmtservbin}mmtservport ngs-info-wfsc`
    setenv NGS-INFO-SLOPES_SRVPORT `${mmtservbin}mmtservport ngs-info-slopes`
    setenv NGS-INFO-DM_SRVPORT `${mmtservbin}mmtservport ngs-info-dm`
    setenv NGS-INFO-STATUS_SRVPORT `${mmtservbin}mmtservport ngs-info-status`

    setenv NGS-TOPBOX_SRVHOST `${mmtservbin}mmtservhost ngs-topbox`
    setenv NGS-TOPBOX_SRVPORT `${mmtservbin}mmtservport ngs-topbox`
    setenv NGS-TOPBOX-STATUS_SRVPORT `${mmtservbin}mmtservport ngs-topbox-status`

    setenv GIZMO-NGS-TOPBOX-FSM-TIP_SRVHOST `${mmtservbin}mmtservhost gizmo-ngs-topbox-fsm-tip`
    setenv GIZMO-NGS-TOPBOX-FSM-TIP_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-fsm-tip`
    setenv GIZMO-NGS-TOPBOX-FSM-TILT_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-fsm-tilt`
    setenv GIZMO-NGS-TOPBOX-OAP-TIP_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-oap-tip`
    setenv GIZMO-NGS-TOPBOX-OAP-TILT_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-oap-tilt`
    setenv GIZMO-NGS-TOPBOX-INDIGO_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-indigo`
    setenv GIZMO-NGS-TOPBOX-F15_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-f15`
    setenv GIZMO-NGS-TOPBOX-POWER_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-power`
    setenv GIZMO-NGS-TOPBOX-FWHEEL_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-fwheel`
    setenv GIZMO-NGS-TOPBOX-SMART_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-smart`
    setenv GIZMO-NGS-TOPBOX-ENCODER_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-encoder`

    setenv MOUNT_SRVHOST `${mmtservbin}mmtservhost mount`
    setenv MOUNT_SRVPORT `${mmtservbin}mmtservport mount`

    setenv HEXAPOD_SRVHOST `${mmtservbin}mmtservhost hexapod`
    setenv HEXAPOD_SRVPORT `${mmtservbin}mmtservport hexapod`

    setenv SCIENCE-SERVER_SRVHOST `${mmtservbin}mmtservhost science-server`
    setenv SCIENCE-SERVER_SRVPORT `${mmtservbin}mmtservport science-server`

    setenv TSS-SERVER_SRVHOST `${mmtservbin}mmtservhost tss-server`
    setenv TSS-SERVER_SRVPORT `${mmtservbin}mmtservport tss-server`
endif
