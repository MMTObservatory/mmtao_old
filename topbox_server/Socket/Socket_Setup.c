/*===========================================================================*
**
**  Socket_Setup.c
**
**  State all the state values in the structure Socket->state to zero and
**    create the TclTk variable names need to update shared varibles.
**
**  Returns :  0 success
**            -1 error
**
**  Written   22mar05   DLM
**
**===========================================================================*/

#include "Topbox.h"
#include "Server_Ports.h"
#include "Socket.h"

/*=========================================
 * function: Socket_Setup
 *=========================================*/

#define MAX_HOST	64

int Socket_Setup( Socket_Info *Socket, char *name, int debug)
{
  char topbox[MAX_HOST];
  char *p;

  int status;

  /* Allow TOPBOX_HOST environment variable to use
   * the source server if desired
   */
  if ( (p = getenv ( "TOPBOX_HOST" )) )
    strncpy ( topbox, p, MAX_HOST );
  else
    strncpy ( topbox, TOPBOX_ADDRESS, MAX_HOST );
  
  /*
    Determine which address and port to use and put them in the structure
  */
  if ( !strncmp( name, "Master", strlen(name)) ) {

    strcpy( Socket->name, "Master Server");
    strcpy( Socket->address, "locahost");
    Socket->port = TOPBOX_MASTER_PORT;
    
  } else if ( !strncmp( name, "FSM_Tip", strlen(name)) ) {

    strcpy( Socket->name, "FSM Tip Motor");
    strcpy( Socket->address, topbox );
    Socket->port = FSM_TIP_PORT;
    
  } else if ( !strncmp( name, "FSM_Tilt", strlen(name)) ) {

    strcpy( Socket->name, "FSM Tilt Motor");
    strcpy( Socket->address, topbox );
    Socket->port = FSM_TILT_PORT;
    
  } else if ( !strncmp( name, "OAP_Tip", strlen(name)) ) {

    strcpy( Socket->name, "OAP Tip Motor");
    strcpy( Socket->address, topbox );
    Socket->port = OAP_TIP_PORT;
    
  } else if ( !strncmp( name, "OAP_Tilt", strlen(name)) ) {

    strcpy( Socket->name, "OAP Tilt Motor");
    strcpy( Socket->address, topbox );
    Socket->port = OAP_TILT_PORT;
    
  } else if ( !strncmp( name, "Indigo", strlen(name)) ) {

    strcpy( Socket->name, "Indigo Stage");
    strcpy( Socket->address, topbox );
    Socket->port = INDIGO_PORT;
    
  } else if ( !strncmp( name, "F15", strlen(name)) ) {

    strcpy( Socket->name, "F15 Stage");
    strcpy( Socket->address, topbox );
    Socket->port = F15_PORT;

  } else if ( !strncmp( name, "Fwheel", strlen(name)) ) {

    strcpy( Socket->name, "Filter Wheel");
    strcpy( Socket->address, topbox );
    Socket->port = FWHEEL_PORT;

  } else if ( !strncmp( name, "Power", strlen(name)) ) {

    strcpy( Socket->name, "Power");
    strcpy( Socket->address, topbox );
    Socket->port = POWER_PORT;
    
  } else if ( !strncmp( name, "Status", strlen(name)) ) {

    strcpy( Socket->name, "Status Server");
    strcpy( Socket->address, "localhost");
    Socket->port = STATUS_PORT;

  } else if ( !strncmp( name, "Smart", strlen(name)) ) {

    strcpy( Socket->name, "Smart");
    strcpy( Socket->address, topbox );
    Socket->port = SMART_PORT;

  } else if ( !strncmp( name, "Encoder", strlen(name)) ) {

    strcpy( Socket->name, "Encoder");
    strcpy( Socket->address, topbox );
    Socket->port = ENCODER_PORT;
    
  } else {

    printf("  Socket_Setup: name not found: %s\n", name);
    fflush(stdout);
    return(-1);

  }

  if ( debug ) {
    printf("  Socket_Setup: Server information\n");
    printf("                     Name = %s\n", Socket->name);
    printf("                  Address = %s\n", Socket->address);
    printf("                     Port = %d\n", Socket->port);
    fflush(stdout);
  }

  Socket->sockfd = -1;
  Socket->connected = 0;
  Socket->error = 0;
  status = pthread_mutex_init( &Socket->lock, NULL);
  if ( status ) {
    printf("  Socket_Setup: Error initializing mutex lock variable for = %s\n", Socket->name);
    fflush(stdout);
    return(-1);
  }

  return(0);
}
