
if [ -z "$PCR_SRVHOST" ]; then
    #mmtservbin="/mmt/bin/"
    mmtservbin="${MMTAO_HOME}/bin/"

    export PCR_SRVHOST=`${mmtservbin}mmtservhost pcr`
    export PCR_SRVPORT=`${mmtservbin}mmtservport pcr`
    export PCR_WFSC_SRVPORT=`${mmtservbin}mmtservport pcr-wfsc`
    export PCR_DM_SRVPORT=`${mmtservbin}mmtservport pcr-dm`
    export PCR_SLOPES_SRVPORT=`${mmtservbin}mmtservport pcr-slopes`
    export PCR_ERROR_SRVPORT=`${mmtservbin}mmtservport pcr-error`

    export NGS_INFO_SRVHOST=`${mmtservbin}mmtservhost ngs-info`
    export NGS_INFO_SRVPORT=`${mmtservbin}mmtservport ngs-info`
    export NGS_INFO_WFSC_SRVPORT=`${mmtservbin}mmtservport ngs-info-wfsc`
    export NGS_INFO_SLOPES_SRVPORT=`${mmtservbin}mmtservport ngs-info-slopes`
    export NGS_INFO_DM_SRVPORT=`${mmtservbin}mmtservport ngs-info-dm`
    export NGS_INFO_STATUS_SRVPORT=`${mmtservbin}mmtservport ngs-info-status`

    export NGS_TOPBOX_SRVHOST=`${mmtservbin}mmtservhost ngs-topbox`
    export NGS_TOPBOX_SRVPORT=`${mmtservbin}mmtservport ngs-topbox`
    export NGS_TOPBOX_STATUS_SRVPORT=`${mmtservbin}mmtservport ngs-topbox-status`

    export GIZMO_NGS_TOPBOX_FSM_TIP_SRVHOST=`${mmtservbin}mmtservhost gizmo-ngs-topbox-fsm-tip`
    export GIZMO_NGS_TOPBOX_FSM_TIP_SRVPORT=`${mmtservbin}mmtservport gizmo-ngs-topbox-fsm-tip`
    export GIZMO_NGS_TOPBOX_FSM_TILT_SRVPORT=`${mmtservbin}mmtservport gizmo-ngs-topbox-fsm-tilt`
    export GIZMO_NGS_TOPBOX_OAP_TIP_SRVPORT=`${mmtservbin}mmtservport gizmo-ngs-topbox-oap-tip`
    export GIZMO_NGS_TOPBOX_OAP_TILT_SRVPORT=`${mmtservbin}mmtservport gizmo-ngs-topbox-oap-tilt`
    export GIZMO_NGS_TOPBOX_INDIGO_SRVPORT=`${mmtservbin}mmtservport gizmo-ngs-topbox-indigo`
    export GIZMO_NGS_TOPBOX_F15_SRVPORT=`${mmtservbin}mmtservport gizmo-ngs-topbox-f15`
    export GIZMO_NGS_TOPBOX_POWER_SRVPORT=`${mmtservbin}mmtservport gizmo-ngs-topbox-power`
    export GIZMO_NGS_TOPBOX_FWHEEL_SRVPORT=`${mmtservbin}mmtservport gizmo-ngs-topbox-fwheel`
    export GIZMO_NGS_TOPBOX_SMART_SRVPORT=`${mmtservbin}mmtservport gizmo-ngs-topbox-smart`
    export GIZMO_NGS_TOPBOX_ENCODER_SRVPORT=`${mmtservbin}mmtservport gizmo-ngs-topbox-encoder`

    export MOUNT_SRVHOST=`${mmtservbin}mmtservhost mount`
    export MOUNT_SRVPORT=`${mmtservbin}mmtservport mount`

    export HEXAPOD_SRVHOST=`${mmtservbin}mmtservhost hexapod`
    export HEXAPOD_SRVPORT=`${mmtservbin}mmtservport hexapod`

    export SCIENCE_SERVER_SRVHOST=`${mmtservbin}mmtservhost science-server`
    export SCIENCE_SERVER_SRVPORT=`${mmtservbin}mmtservport science-server`

    export TSS_SERVER_SRVHOST=`${mmtservbin}mmtservhost tss-server`
    export TSS_SERVER_SRVPORT=`${mmtservbin}mmtservport tss-server`
fi
