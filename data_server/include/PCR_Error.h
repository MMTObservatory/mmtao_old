
#ifndef ERROR_FILE_SEEN
#define ERROR_FILE_SEEN

#define  NO_ERROR                               0
#define  PCR_OK 				0
#define  PCR_NOT_OK				(!PCR_OK)

#define PCR_LOOP_PANIC				-32768
#define PCR_OPEN_BUT_SAFE_EXIT  		-32000
#define PCR_RTR_DELTAS_TOO_LARGE  		-31000
#define PCR_DM_COMM_ERROR			-10000 

#define PCR_COMM_ERROR				-1000
#define PCR_GENERIC_ERROR			-1001
#define PCR_INPUT_ERROR 			-1002
#define PCR_TYPE_ERROR				-1003
#define PCR_FILEIO_ERROR			-1004
#define PCR_NETWORK_ERROR			-1005
#define PCR_THREAD_ERROR			-1006
#define PCR_DISKFULL_ERROR			-1007
#define PCR_FIFO_ERROR				-1008
#define PCR_MEMORY_ALLOC_ERROR			-1009


// camera related return codes
#define PCR_NGS_CAMERA_ERROR			-100
#define PCR_NGS_CONFIG_ERR0R			-101
#define PCR_NGS_FRAME_RATE_ERROR 		-102
#define PCR_NGS_COMM_ERROR     			-103
#define PCR_NGS_CAMSIZE_ERROR     		-103
#define PCR_NGS_CAM_SERIALPORT_ERROR     	-103


// INIT errors
#define PCR_VARIABLES_INIT_ERROR 		-201

// SSC related codes
#define PCR_SSC_ERROR				-300
#define PCR_SSC_SLUTREAD_OK			-301
#define PCR_SSC_SLUTREAD_NOTOK			-302
#define PCR_MODAL_MATRIX_READ_NOTOK 		-303
#define PCR_SYSTEM_GAIN_READ_NOTOK 		-304
#define PCR_ACT_READ_NOTOK 			-305
#define PCR_SSC_MALLOC_FAILED   		-306
#define PCR_MODAL_INIT_ERROR    		-307

// RTR related codes
#define PCR_RTR_ERROR				-400
#define PCR_RTR_MATRIX_READ_OK  		-401
#define PCR_RTR_MATRIX_READ_NOTOK 		-402
#define PCR_SLOPE_MALLOC_FAILED 		-403
#define PCR_RTR_MALLOC_FAILED   		-404


// DMI related codes
#define PCR_DMI_ERROR				-500
#define PCR_DMI_FLAT_POS_ERROR  		-501
#define PCR_DMI_MODAL_TERM_CALC_ERROR 		-502

//BCU codes
#define PCR_BCU_ERROR				-600
#define PCR_BCU_SOCKET_LOCK_ERROR		-601
#define PCR_BCU_ETH_ERROR  			-602
#define PCR_BCU_JUMBOFRAME_ERROR 		-603
#define PCR_BCU_TIMEOUT_ERROR 			-604

//DDS related codes
#define PCR_DDS_ERROR				-700
#define PCR_DDS_FIFO_OPEN_NOTOK			-701
#define PCR_DDS_BUFFER_ERROR    		-702

// Modal terms error

//database related errors
#define PCR_DBASE_ERROR				-800


typedef struct {
    int pcr_code;
    char * errmsg;
} ErrorList;
				
#endif // ERROR_FILE_SEEN
