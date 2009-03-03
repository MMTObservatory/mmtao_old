/*
  Define the ports that different server will listen
    to.  This header file is read by routines in the
    directories
    ~/ao/wish
    ~/ao/data_server
    ~/ao/topbox_server

  If you change this include file, then you only need
    to type
      % make
    in the ~/ao directory for all routines to be updated

  Written  5sept03  DLM

  Altered  6sep07  DLM  Added Topbox Server information

*/

/*****************************************************************
 *
 * PCR address and port numbers
 *
 *****************************************************************/
#define PCR_ADDRESS       (getenv("PCR_SRVHOST"))

/* Ports to connect to the PCR */
#define	PCR_CMD_PORT      (atoi(getenv("PCR_SRVPORT")))
#define	PCR_WFSC_PORT     (atoi(getenv("PCR_WFSC_SRVPORT")))
#define	PCR_DM_PORT       (atoi(getenv("PCR_DM_SRVPORT")))
#define	PCR_SLOPES_PORT   (atoi(getenv("PCR_SLOPES_SRVPORT")))
#define	PCR_ERROR_PORT    (atoi(getenv("PCR_ERROR_SRVPORT")))

/*****************************************************************
 *
 * PCR Server address and port numbers
 *
 *****************************************************************/
#define PCR_SERVER_ADDRESS     (getenv("NGS_INFO_SRVHOST"))

/* Command Server Ports */
#define	PCR_SERVER_CMD_PORT    (atoi(getenv("NGS_INFO_SRVPORT")))

/* Data Server Ports */
#define	PCR_SERVER_WFSC_PORT   (atoi(getenv("NGS_INFO_WFSC_SRVPORT")))
#define	PCR_SERVER_SLOPES_PORT (atoi(getenv("NGS_INFO_SLOPES_SRVPORT")))
#define	PCR_SERVER_DM_PORT     (atoi(getenv("NGS_INFO_DM_SRVPORT")))

/* Info Port from the PCR_Server */
#define	INFO_PORT              (atoi(getenv("NGS_INFO_STATUS_SRVPORT")))

/*****************************************************************
 *
 * Topbox Server address and port numbers
 *
 *****************************************************************/
#define TOPBOX_SERVER_ADDRESS     (getenv("NGS_TOPBOX_SRVHOST"))

/* Status Port from the Topbox_Server */
#define STATUS_PORT              (atoi(getenv("NGS_TOPBOX_STATUS_SRVPORT")))
#define TOPBOX_MASTER_PORT       (atoi(getenv("NGS_TOPBOX_SRVPORT")))

/*****************************************************************
 *
 * Topbox Cyclades address and port numbers
 *
 *****************************************************************/
#define TOPBOX_ADDRESS     (getenv("GIZMO_NGS_TOPBOX_FSM_TIP_SRVHOST"))

/* Device Ports for the Topbox */
#define FSM_TIP_PORT       (atoi(getenv("GIZMO_NGS_TOPBOX_FSM_TIP_SRVPORT")))
#define FSM_TILT_PORT      (atoi(getenv("GIZMO_NGS_TOPBOX_FSM_TILT_SRVPORT")))
#define OAP_TIP_PORT       (atoi(getenv("GIZMO_NGS_TOPBOX_OAP_TIP_SRVPORT")))
#define OAP_TILT_PORT      (atoi(getenv("GIZMO_NGS_TOPBOX_OAP_TILT_SRVPORT")))
#define INDIGO_PORT        (atoi(getenv("GIZMO_NGS_TOPBOX_INDIGO_SRVPORT")))
#define F15_PORT           (atoi(getenv("GIZMO_NGS_TOPBOX_F15_SRVPORT")))
#define POWER_PORT         (atoi(getenv("GIZMO_NGS_TOPBOX_POWER_SRVPORT")))
#define FWHEEL_PORT        (atoi(getenv("GIZMO_NGS_TOPBOX_FWHEEL_SRVPORT")))
#define SMART_PORT         (atoi(getenv("GIZMO_NGS_TOPBOX_SMART_SRVPORT")))
#define ENCODER_PORT       (atoi(getenv("GIZMO_NGS_TOPBOX_ENCODER_SRVPORT")))

/*****************************************************************
 *
 * TCS address and port numbers
 *
 *****************************************************************/
#define TCS_ADDRESS              (getenv("MOUNT_SRVHOST"))
#define	TCS_PORT                 (atoi(getenv("MOUNT_SRVPORT")))

/*****************************************************************
 *
 * Hexapod address and port numbers
 *
 *****************************************************************/
#define HEXAPOD_ADDRESS       (getenv("HEXAPOD_SRVHOST"))
#define	HEXAPOD_PORT          (atoi(getenv("HEXAPOD_SRVPORT")))

/*****************************************************************
 *
 * Science Server address and port numbers
 *
 *****************************************************************/
#define SCIENCE_SERVER_ADDRESS     (getenv("SCIENCE_SERVER_SRVHOST"))
#define SCIENCE_SERVER_PORT      (atoi(getenv("SCIENCE_SERVER_SRVPORT")))

/*****************************************************************
 *
 * TSS Server address and port numbers
 *
 *****************************************************************/
#define TSS_SERVER_ADDRESS     (getenv("TSS_SERVER_SRVHOST"))
#define TSS_SERVER_PORT      (atoi(getenv("TSS_SERVER_SRVPORT")))
