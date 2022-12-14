/**********************************************************
 * InfoList.h
 **********************************************************

  Setup the InfoList array once when the Info server is started.  This
    InfoList array will be passed to all subsequent routines

  The arrays to hold seeing and strehl estimates are also created here.

  Written 19nov06  DLM

*/

Status_Entry Status_List[] = {
  { "FSM_Tip_Connected",         "Pointer to a flag indicating if connected to the device", "I" },
  { "FSM_Tip_Connection_Error",  "Pointer to a flag indicating a connection error to the device", "I" },
  { "FSM_Tip_Home",              "Pointer to a flag indicating if the device is homed", "I" },
  { "FSM_Tip_Position",          "Pointer to a the position of the device", "F" },
  { "FSM_Tip_Moving",            "Pointer to a flag indicating the device is moving", "I" },
  { "FSM_Tip_In_Position",       "Pointer to a flag indicating the device is in position", "I" },
  { "FSM_Tip_Error",             "Pointer to a flag indicating the device has an error", "I" },
  { "FSM_Tip_Enabled",           "Pointer to a flag indicating the device is enabled", "I" },
  { "FSM_Tip_Negative",          "Pointer to a flag indicating if the device is at a negative limit", "I" },
  { "FSM_Tip_Positive",          "Pointer to a flag indicating if the device is at a positive limit", "I" },
  { "FSM_Tilt_Connected",        "Pointer to a flag indicating if connected to the device", "I" },
  { "FSM_Tilt_Connection_Error", "Pointer to a flag indicating a connection error to the device", "I" },
  { "FSM_Tilt_Home",             "Pointer to a flag indicating if the device is homed", "I" },
  { "FSM_Tilt_Position",         "Pointer to a the position of the device", "F" },
  { "FSM_Tilt_Moving",           "Pointer to a flag indicating the device is moving", "I" },
  { "FSM_Tilt_In_Position",      "Pointer to a flag indicating the device is in position", "I" },
  { "FSM_Tilt_Error",            "Pointer to a flag indicating the device has an error", "I" },
  { "FSM_Tilt_Enabled",          "Pointer to a flag indicating the device is enabled", "I" },
  { "FSM_Tilt_Negative",         "Pointer to a flag indicating if the device is at a negative limit", "I" },
  { "FSM_Tilt_Positive",         "Pointer to a flag indicating if the device is at a positive limit", "I" },
  { "OAP_Tip_Connected",         "Pointer to a flag indicating if connected to the device", "I" },
  { "OAP_Tip_Connection_Error",  "Pointer to a flag indicating a connection error to the device", "I" },
  { "OAP_Tip_Home",              "Pointer to a flag indicating if the device is homed", "I" },
  { "OAP_Tip_Position",          "Pointer to a the position of the device", "F" },
  { "OAP_Tip_Moving",            "Pointer to a flag indicating the device is moving", "I" },
  { "OAP_Tip_In_Position",       "Pointer to a flag indicating the device is in position", "I" },
  { "OAP_Tip_Error",             "Pointer to a flag indicating the device has an error", "I" },
  { "OAP_Tip_Enabled",           "Pointer to a flag indicating the device is enabled", "I" },
  { "OAP_Tip_Negative",          "Pointer to a flag indicating if the device is at a negative limit", "I" },
  { "OAP_Tip_Positive",          "Pointer to a flag indicating if the device is at a positive limit", "I" },
  { "OAP_Tip_Event_2",           "Pointer to a flag indicating the device's home flag is tripped", "I" },
  { "OAP_Tilt_Connected",        "Pointer to a flag indicating if connected to the device", "I" },
  { "OAP_Tilt_Connection_Error", "Pointer to a flag indicating a connection error to the device", "I" },
  { "OAP_Tilt_Home",             "Pointer to a flag indicating if the device is homed", "I" },
  { "OAP_Tilt_Position",         "Pointer to a the position of the device", "F" },
  { "OAP_Tilt_Moving",           "Pointer to a flag indicating the device is moving", "I" },
  { "OAP_Tilt_In_Position",      "Pointer to a flag indicating the device is in position", "I" },
  { "OAP_Tilt_Error",            "Pointer to a flag indicating the device has an error", "I" },
  { "OAP_Tilt_Enabled",          "Pointer to a flag indicating the device is enabled", "I" },
  { "OAP_Tilt_Negative",         "Pointer to a flag indicating if the device is at a negative limit", "I" },
  { "OAP_Tilt_Positive",         "Pointer to a flag indicating if the device is at a positive limit", "I" },
  { "OAP_Tilt_Event_2",          "Pointer to a flag indicating the device's home flag is tripped", "I" },
  { "Indigo_Connected",          "Pointer to a flag indicating if connected to the device", "I" },
  { "Indigo_Connection_Error",   "Pointer to a flag indicating a connection error to the device", "I" },
  { "Indigo_Home",               "Pointer to a flag indicating if the device is homed", "I" },
  { "Indigo_Position",           "Pointer to a the position of the device", "F" },
  { "Indigo_Moving",             "Pointer to a flag indicating the device is moving", "I" },
  { "Indigo_Enabled",            "Pointer to a flag indicating the device is enabled", "I" },
  { "Indigo_Negative",           "Pointer to a flag indicating if the device is at a negative limit", "I" },
  { "Indigo_Positive",           "Pointer to a flag indicating if the device is at a positive limit", "I" },
  { "F15_Connected",             "Pointer to a flag indicating if connected to the device", "I" },
  { "F15_Connection_Error",      "Pointer to a flag indicating a connection error to the device", "I" },
  { "F15_Home",                  "Pointer to a flag indicating if the device is homed", "I" },
  { "F15_Position",              "Pointer to a the position of the device", "F" },
  { "F15_Moving",                "Pointer to a flag indicating the device is moving", "I" },
  { "F15_Enabled",               "Pointer to a flag indicating the device is enabled", "I" },
  { "F15_Negative",              "Pointer to a flag indicating if the device is at a negative limit", "I" },
  { "F15_Positive",              "Pointer to a flag indicating if the device is at a positive limit", "I" },
  { "Fwheel_Connected",          "Pointer to a flag indicating if connected to the device", "I" },
  { "Fwheel_Connection_Error",   "Pointer to a flag indicating a connection error to the device", "I" },
  { "Fwheel_Home",               "Pointer to a flag indicating if the device is homed", "I" },
  { "Fwheel_Position",           "Pointer to a the position of the device", "F" },
  { "Fwheel_Name",               "Pointer to string holding the name of the filter", "S" },
  { "Fwheel_Moving",             "Pointer to a flag indicating the device is moving", "I" },
  { "Power_Connected",           "Pointer to a flag indicating if connected to the device", "I" },
  { "Power_Connection_Error",    "Pointer to a flag indicating a connection error to the device", "I" },
  { "Power_WFSC",                "Pointer to a flag indicating status of this power relay", "I" },
  { "Power_Peltier",             "Pointer to a flag indicating status of this power relay", "I" },
  { "Power_Stella",              "Pointer to a flag indicating status of this power relay", "I" },
  { "Power_Alignment_Laser",     "Pointer to a flag indicating status of this power relay", "I" },
  { "Power_F15_Laser",           "Pointer to a flag indicating status of this power relay", "I" },
  { "Power_Indigo",              "Pointer to a flag indicating status of this power relay", "I" },
  { "Power_Spare1",              "Pointer to a flag indicating status of this power relay", "I" },
  { "Power_Spare2",              "Pointer to a flag indicating status of this power relay", "I" },
  { "Focus_Connected",           "Pointer to a flag indicating if connected to the device", "I" },
  { "Focus_Connection_Error",    "Pointer to a flag indicating a connection error to the device", "I" },
  { "Focus_Home",                "Pointer to a flag indicating if the device is homed", "I" },
  { "Focus_Position",            "Pointer to a the position of the device", "F" },
  { "Focus_Moving",              "Pointer to a flag indicating the device is moving", "I" },
  { "Focus_Enabled",             "Pointer to a flag indicating the device is enabled", "I" },
  { "Focus_Negative",            "Pointer to a flag indicating if the device is at a negative limit", "I" },
  { "Focus_Positive",            "Pointer to a flag indicating if the device is at a positive limit", "I" },
  { "Rotation_Connected",        "Pointer to a flag indicating if connected to the device", "I" },
  { "Rotation_Connection_Error", "Pointer to a flag indicating a connection error to the device", "I" },
  { "Rotation_Home",             "Pointer to a flag indicating if the device is homed", "I" },
  { "Rotation_Position",         "Pointer to a the position of the device", "F" },
  { "Rotation_Moving",           "Pointer to a flag indicating the device is moving", "I" },
  { "Rotation_Enabled",          "Pointer to a flag indicating the device is enabled", "I" },
  { "Rotation_Negative",         "Pointer to a flag indicating if the device is at a negative limit", "I" },
  { "Rotation_Positive",         "Pointer to a flag indicating if the device is at a positive limit", "I" },
};

int Status_ListLength = ( sizeof(Status_List)/sizeof(struct _Status_Entry) );
