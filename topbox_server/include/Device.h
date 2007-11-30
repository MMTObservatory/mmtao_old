/*
  Device.h

  Definitions needed to communicate and stop information about each
    device.

  Written  13aug07  DLM  Started with DServ.h from pcr_server

*/

#include "Socket.h"

typedef struct _Device_Data {
  char name[16];       // For Smart device, this is Rotation or Focus (device name).
  float gain;          // Conversion factor to go from device units to user units
  int speed;           // Speed to move the device
  int accel;           // Acceleration for starting and stopping device motion
  int home;            // Flag to indicate if device has been homed
  float position;      // Value of the current position of the device
  char filter[12];     // Name of the filter at the current position
  int moving;          // Flag to indicate if device is moving
  int in_position;     // Flag to indicate if device is in the requested position
  int error;           // Flag to indicate if device has an error
  int enabled;         // Flag to indicate if device is enabled to move
  int negative;        // Flag to indicate if device is at a negative limit
  int positive;        // Flag to indicate if device is at a positive limit
  int event_2;         // Flag to indicate if OAP home flag is tripped
  int relays[8];       // Flags for power control relays
} Device_Data;

typedef struct _Device_Info {
  Socket_Info *socket;   // Socket structure in Socket.h
  Device_Data *data;     // Data structure as defined above
  Device_Data *data2;    // Second data structure as defined above (for Smart controller)
  int *continueRunning;  // Flag used to shutdown the status thread for this device
  int *running;          // Flag indicating if the status thread for this device is running
  int *status_debug;     // Flag used for debugging the status thread for this device
  int *cmd_debug;        // Flag used for debugging the command thread for this device
} Device_Info;
