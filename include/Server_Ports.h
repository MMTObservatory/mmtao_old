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
//#define PCR_ADDRESS       "localhost"
//#define PCR_ADDRESS       "spherical.as.arizona.edu"
//#define PCR_ADDRESS       "trefoil.mmto.arizona.edu"
#define PCR_ADDRESS       "ao-pcr.mmto.arizona.edu"
//#define PCR_ADDRESS       "tilt.as.arizona.edu"
//#define PCR_ADDRESS       "tuco.as.arizona.edu"
//#define PCR_ADDRESS       "astigmatism"

/* Ports to connect to the PCR */
#define	PCR_CMD_PORT      8001
#define	PCR_WFSC_PORT     8002
#define	PCR_DM_PORT       8003
#define	PCR_SLOPES_PORT   8007
#define	PCR_ERROR_PORT    8004

/*****************************************************************
 *
 * PCR Server address and port numbers
 *
 *****************************************************************/
//#define PCR_SERVER_ADDRESS     "localhost"
#define PCR_SERVER_ADDRESS     "ao-server.mmto.arizona.edu"
//#define PCR_SERVER_ADDRESS     "hoseclamp.mmto.arizona.edu"
//#define PCR_SERVER_ADDRESS     "tilt.as.arizona.edu"
//#define PCR_SERVER_ADDRESS     "tuco"
//#define PCR_SERVER_ADDRESS     "astigmatism"
//#define PCR_SERVER_ADDRESS     "aocc"

/* Command Server Ports */
#define	PCR_SERVER_CMD_PORT    7501

/* Data Server Ports */
#define	PCR_SERVER_WFSC_PORT   7502
#define	PCR_SERVER_SLOPES_PORT 7503
#define	PCR_SERVER_DM_PORT     7504

/* Info Port from the PCR_Server */
#define	INFO_PORT              7500

/*****************************************************************
 *
 * Topbox Server address and port numbers
 *
 *****************************************************************/
//#define TOPBOX_SERVER_ADDRESS     "localhost"
#define TOPBOX_SERVER_ADDRESS     "ao-server.mmto.arizona.edu"

/* Status Port from the Topbox_Server */
#define STATUS_PORT              7400
#define TOPBOX_MASTER_PORT       7401

/*****************************************************************
 *
 * Topbox Cyclades address and port numbers
 *
 *****************************************************************/
//#define TOPBOX_ADDRESS     "localhost"
#define TOPBOX_ADDRESS     "ao-topbox.mmto.arizona.edu"

/* Device Ports for the Topbox */
#define FSM_TIP_PORT       7001
#define FSM_TILT_PORT      7002
#define OAP_TIP_PORT       7003
#define OAP_TILT_PORT      7004
#define INDIGO_PORT        7005
#define F15_PORT           7006
#define POWER_PORT         7007
#define FWHEEL_PORT        7008
#define SMART_PORT         7009
#define ENCODER_PORT       7010

/*****************************************************************
 *
 * TCS address and port numbers
 *
 *****************************************************************/
#define TCS_ADDRESS              "mount.mmto.arizona.edu"
#define	TCS_PORT                 5240

/*****************************************************************
 *
 * Hexapod address and port numbers
 *
 *****************************************************************/
#define HEXAPOD_ADDRESS       "hexapod.mmto.arizona.edu"
#define	HEXAPOD_PORT          5340

/*****************************************************************
 *
 * Science Server address and port numbers
 *
 *****************************************************************/
#define SCIENCE_SERVER_ADDRESS     "ao-server.mmto.arizona.edu"
#define SCIENCE_SERVER_PORT      7700
