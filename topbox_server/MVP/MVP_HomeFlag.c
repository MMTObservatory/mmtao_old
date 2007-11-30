/*
  MVP_HomeFlag.c

  This routine gets the home variable for the given device

  Written  24aug07  DLM  Started with MVP_SetHomeFlag.c

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "MVP.h"

/*=========================================
 * Get the home variable
 *=========================================*/

int MVP_HomeFlag( Socket_Info *Socket, Device_Data *Data, int debug)
{
  int status;
  char request[STRING_LENGTH];
  char reply[STRING_LENGTH];
  int local_debug = 0;
  int home;

  /*
    Lock the mutex variable "Device"->lock so other threads do not try
      to read or write while this thread is retrieving status information.
    If another thread has the variable "Device"->lock locked, this process
      is blocked until the mutex is unlocked.  Likewise, if the mutex
      variable is locked here, other processed are blocked until the mutex
      is unlock by this thread.
    In all other routines that communication on the "Device" port the
      same mutex lock function is called.
  */
  status = pthread_mutex_lock( &Socket->lock);

  /*
    Set the string to return the device home variable
  */
  strcpy( request, "0 ANO");

  /*
    Send the request
  */
  if ( MVP_Write( Socket, request, debug) ) {
    if ( debug ) {
      printf("  MVP_HomeFlag: Error sending the home variable request to %s\n", Socket->name);
      printf("                Connection closed\n");
      fflush(stdout);
    }
    status = Socket_Close( Socket, local_debug);
    status = Status_ZeroFlags( Data);
    /*
      Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
    */
    pthread_mutex_unlock( &Socket->lock);
    return(-1);
  }

  /*
    Get the reply.  Must use a special MVP read instead of Socket_Read
  */
  status = MVP_Read( Socket, reply, local_debug);

  /*
    Unlock the mutex variable "Device"->lock so other threads may use the "Device" port
  */
  pthread_mutex_unlock( &Socket->lock);

  if ( status ) {
    if ( debug ) {
      printf("  MVP_HomeFlag: Error retrieving the home variable from %s\n", Socket->name);
      fflush(stdout);
    }
    return(-1);
  }

  /*
    Check that a reply was received
  */
  if ( strlen( reply) < 9 ) {
    printf("  MVP_HomeFlag: Incorrect size of reply from MVP_Read\n");
    printf("                Expected at least 9 characters\n");
    printf("                Received %d --> %s\n", (int) strlen( reply), reply);
    fflush(stdout);
    return(-1);
  }
    
  /*
    Retrieve the appropriate part of the message
  */
  sscanf ( reply + 5, "%x", &home);

  /*
    If the value of the home flag is 1, then the device has been homed.  Any other
      value means the device has not been homed (eg default value of ANO is 0800 hex)
  */
  if ( home == 1 ) {
    Data->home = 1;
  } else {
    Data->home = 0;
  }
    
  if ( debug ) {
    printf("  MVP_HomeFlag: Home flag for %s = %d\n", Socket->name, Data->home);
    fflush(stdout);
  }

  return(0);
}

