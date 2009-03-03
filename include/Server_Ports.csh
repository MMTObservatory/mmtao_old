#!/bin/csh

if (! $?PCR_SRVHOST) then
    #set mmtservbin = /mmt/bin/
    set mmtservbin = ${MMTAO_HOME}/bin/

    setenv PCR_SRVHOST `${mmtservbin}mmtservhost pcr`
    setenv PCR_SRVPORT `${mmtservbin}mmtservport pcr`
    setenv PCR_WFSC_SRVPORT `${mmtservbin}mmtservport pcr-wfsc`
    setenv PCR_DM_SRVPORT `${mmtservbin}mmtservport pcr-dm`
    setenv PCR_SLOPES_SRVPORT `${mmtservbin}mmtservport pcr-slopes`
    setenv PCR_ERROR_SRVPORT `${mmtservbin}mmtservport pcr-error`

    setenv NGS_INFO_SRVHOST `${mmtservbin}mmtservhost ngs-info`
    setenv NGS_INFO_SRVPORT `${mmtservbin}mmtservport ngs-info`
    setenv NGS_INFO_WFSC_SRVPORT `${mmtservbin}mmtservport ngs-info-wfsc`
    setenv NGS_INFO_SLOPES_SRVPORT `${mmtservbin}mmtservport ngs-info-slopes`
    setenv NGS_INFO_DM_SRVPORT `${mmtservbin}mmtservport ngs-info-dm`
    setenv NGS_INFO_STATUS_SRVPORT `${mmtservbin}mmtservport ngs-info-status`

    setenv NGS_TOPBOX_SRVHOST `${mmtservbin}mmtservhost ngs-topbox`
    setenv NGS_TOPBOX_SRVPORT `${mmtservbin}mmtservport ngs-topbox`
    setenv NGS_TOPBOX_STATUS_SRVPORT `${mmtservbin}mmtservport ngs-topbox-status`

    setenv GIZMO_NGS_TOPBOX_FSM_TIP_SRVHOST `${mmtservbin}mmtservhost gizmo-ngs-topbox-fsm-tip`
    setenv GIZMO_NGS_TOPBOX_FSM_TIP_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-fsm-tip`
    setenv GIZMO_NGS_TOPBOX_FSM_TILT_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-fsm-tilt`
    setenv GIZMO_NGS_TOPBOX_OAP_TIP_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-oap-tip`
    setenv GIZMO_NGS_TOPBOX_OAP_TILT_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-oap-tilt`
    setenv GIZMO_NGS_TOPBOX_INDIGO_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-indigo`
    setenv GIZMO_NGS_TOPBOX_F15_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-f15`
    setenv GIZMO_NGS_TOPBOX_POWER_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-power`
    setenv GIZMO_NGS_TOPBOX_FWHEEL_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-fwheel`
    setenv GIZMO_NGS_TOPBOX_SMART_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-smart`
    setenv GIZMO_NGS_TOPBOX_ENCODER_SRVPORT `${mmtservbin}mmtservport gizmo-ngs-topbox-encoder`

    setenv MOUNT_SRVHOST `${mmtservbin}mmtservhost mount`
    setenv MOUNT_SRVPORT `${mmtservbin}mmtservport mount`

    setenv HEXAPOD_SRVHOST `${mmtservbin}mmtservhost hexapod`
    setenv HEXAPOD_SRVPORT `${mmtservbin}mmtservport hexapod`

    setenv SCIENCE_SERVER_SRVHOST `${mmtservbin}mmtservhost science-server`
    setenv SCIENCE_SERVER_SRVPORT `${mmtservbin}mmtservport science-server`

    setenv TSS_SERVER_SRVHOST `${mmtservbin}mmtservhost tss-server`
    setenv TSS_SERVER_SRVPORT `${mmtservbin}mmtservport tss-server`
endif
