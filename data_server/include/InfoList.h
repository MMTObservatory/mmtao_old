/**********************************************************
 * InfoList.h
 **********************************************************

  Setup the InfoList array once when the Info server is started.  This
    InfoList array will be passed to all subsequent routines

  The arrays to hold seeing and strehl estimates are also created here.

  Written 19nov06  DLM

*/

Info_Entry Info_List[] = {
  { "MMTAO_Home",            "MMTAO Base Directory", "s" },
  { "CAAO_Home",             "CAAO Base Directory", "s" },
  { "PCR_Home",              "PCR Base Directory", "s" },
  { "Setup_Dir",             "System Setup Directory", "s" },
  { "PCR_Client_Running",    "Pointer to a flag that the thread that connects to the PCR CMD, WFSC and DDS servers is running", "I" },
  { "PCR_CMD_Connected",     "Pointer to a flag for the connection of PCR_Server to the PCR Cmd port", "I" },
  { "PCR_CMD_Error",         "Pointer to a flag for an error on the PCR Cmd port", "I" },
  { "PCR_WFSC_Connected",    "Pointer to a flag for the connection of PCR_Server to the PCR WFSC port", "I" },
  { "PCR_WFSC_Error",        "Pointer to a flag for an error on the PCR WFSC port", "I" },
  { "PCR_WFSC_Receiving_Data","Pointer to a flag that we are receiving data on the PCR WFSC port", "I" },
  { "PCR_WFSC_Rate",         "Pointer to the rate that we are receiving data on the PCR WFSC port", "I" },
  { "PCR_DDS_Connected",     "Pointer to a flag for the connection of PCR_Server to the PCR DDS port", "I" },
  { "PCR_DDS_Error",         "Pointer to a flag for an error on the PCR DDS port", "I" },
  { "PCR_DDS_Receiving_Data","Pointer to a flag that we are receiving data on the PCR DDS port", "I" },
  { "PCR_DDS_Rate",          "Pointer to the rate that we are receiving data on the PCR DDS port", "I" },
  { "PCR_ERROR_Connected",   "Pointer to a flag for the connection of PCR_Server to the PCR Error port", "I" },
  { "PCR_ERROR_Error",       "Pointer to a flag for an error on the PCR Error port", "I" },
  { "Recon_Name",            "Name of the Reconstrutor files loaded into the PCR", "s" },
  { "DServ_WFSC_Running",    "Pointer to a flag indicating WFSC Data Server is running", "I" },
  { "DServ_DDS_Running",     "Pointer to a flag indicating DDS Data Server is running", "I" },
  { "Info_Running",          "Pointer to a flag indicating Info Server is running", "I" },
  { "WFSC_Initialized",      "Flag indicating WFSC is initialized", "i" },
  { "WFSC_Running",          "Flag indicating WFSC frames are being updated", "i" },
  { "WFSC_Period",           "Period of WFSC in milliseconds", "f" },
  { "WFSC_Frequency",        "Frequency of WFSC in Hertz", "i" },
  { "WFSC_Size",             "Wave Front Sensor Camera Size (Full, Bin)", "s" },
  { "WFSC_Source",           "WFSC Source (Test, Cam)", "s" },
  { "WFSC_Bias_0",           "WFSC Bias Level for quad 0", "i" },
  { "WFSC_Bias_1",           "WFSC Bias Level for quad 1", "i" },
  { "WFSC_Bias_2",           "WFSC Bias Level for quad 2", "i" },
  { "WFSC_Bias_3",           "WFSC Bias Level for quad 3", "i" },
  { "WFSC_Maximum_Pixel",    "Maximum pixel value seen on the WFSC", "s" },
  { "WFSC_Average_Counts",   "Avearge Counts per Sub-app on the WFSC", "s" },
  { "WFSC_Temp_1",           "Temperature of the WFSC", "s" },
  { "WFSC_Temp_2",           "Temperature of the WFSC", "s" },
  { "WFSC_Pupil_Tip",        "Pupil Tip seen at the WFSC", "s" },
  { "WFSC_Pupil_Tilt",       "Pupil Tilt seen at the WFSC", "s" },
  { "WFSC_Tip",              "Tip seen at the WFSC", "s" },
  { "WFSC_Tilt",             "Tilt seen at the WFSC", "s" },
  { "WFSC_Focus",            "Focus seen at the WFSC", "s" },
  { "WFSC_Astig_0",          "Astigmatism_0 seen at the WFSC", "s" },
  { "WFSC_Astig_45",         "Astigmatism_45 seen at the WFSC", "s" },
  { "WFSC_Coma_X",           "Coma_X seen at the WFSC", "s" },
  { "WFSC_Coma_Y",           "Coma_Y seen at the WFSC", "s" },
  { "WFSC_Trefoil_0",        "Trefoil_0 seen at the WFSC", "s" },
  { "WFSC_Trefoil_30",       "Trefoil_30 seen at the WFSC", "s" },
  { "WFSC_Spherical",        "Spherical seen at the WFSC", "s" },
  { "DM_Running",            "Flag indicating DM frames are being updated", "s" },
  { "DM_Tip",                "Tip seen at the DM", "s" },
  { "DM_Tilt",               "Tilt seen at the DM", "s" },
  { "DM_Focus",              "Focus seen at the DM", "s" },
  { "DM_Astig_0",            "Astigmatism_0 seen at the DM", "s" },
  { "DM_Astig_45",           "Astigmatism_45 seen at the DM", "s" },
  { "DM_Coma_X",             "Coma_X seen at the DM", "s" },
  { "DM_Coma_Y",             "Coma_Y seen at the DM", "s" },
  { "DM_Trefoil_0",          "Trefoil_0 seen at the DM", "s" },
  { "DM_Trefoil_30",         "Trefoil_30 seen at the DM", "s" },
  { "DM_Spherical",          "Spherical seen at the DM", "s" },
  { "Static_Running",        "Flag indicating static aberrations is being applied", "i" },
  { "Static_Astig_45",       "Applied static astigmatism at 45 degrees", "i" },
  { "Static_Astig_0",        "Applied static astigmatism at 0 degrees", "i" },
  { "Static_Coma_y",         "Applied static coma in Y direction", "i" },
  { "Static_Coma_x",         "Applied static coma in X direction", "i" },
  { "Static_Trefoil_30",     "Applied static trefoil at 30 degrees", "i" },
  { "Static_Trefoil_0",      "Applied static trefoil at 0 degrees", "i" },
  { "Static_Spherical",      "Applied static spherical Abberation", "i" },
  { "Modulation_Running",    "Flag indicating slope modulation is being applied", "i" },
  { "Seeing_Running",        "Flag indicating Seeing is being calculated", "s" },
  { "Seeing_Modulating",     "Flag indicating if slope modulation is bing applied", "s" },
  { "Seeing_Mode_File",      "Slope offset matrix file being used", "s" },
  { "Seeing_Total_Modes",    "The number of modes available", "s" },
  { "Seeing_Mode",           "Mode currently being applied (starting at tip = 1)", "s" },
  { "Seeing_Scale_Factor",   "Factor slope offsets multiplied by when when loaded (usually 1)", "s" },
  { "Seeing_Period",         "Number of frames for one period", "s" },
  { "Seeing_Frequency",      "Frequency of modulation in Hertz (depends on period and WSFC rate)", "s" },
  { "Seeing_Gain_Change",    "Flag for Seeing monitor that the loop gain has changed", "s" },
  { "Seeing_Value",          "Current seeing value", "s" },
  { "Strehl_Value",          "Current strehl value", "s" },
  { "Science_Observing",     "Are science observations being taking?", "s" },
  { "Background_Dynamic",    "Flag for dyanamic background subtraction", "i" },
  { "Background_Static",     "Flag for static background subtraction", "i" },
  { "Background_File",       "Background File if type is static", "s" },
  { "Loop_Running",          "AO Loop is Running", "i" },
  { "Loop_Gain",             "User Gain", "f" },
  { "Loop_Save_Data",        "Flag indicating the DDS server should save data", "i" },
};

int Info_ListLength = ( sizeof(Info_List)/sizeof(struct _Info_Entry) );

Info_Array Seeing = { SEEING_VALUES, 0, 0 };

Info_Array Strehl = { SEEING_VALUES, 0, 0 };
