/*===========================================================================*
**
**  VoltMeter_Connect.c
**
**      VoltMeter_Connect()
**      VoltMeter_Close()
**
**  Connect to the HP VoltMeter via and RS232 serial port on a Cyclades terminal server.
**
**  Returns :  0 success
**            -1 error
**
**  Written  30mar04  DLM  Started with OAP_Init.  Trying to make this a
**                           a generic motor driver.
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "VoltMeter.h"

/*=========================================
 * function: VoltMeter_Connect
 *=========================================*/

int VoltMeter_Connect(Motor_Info *Info, int debug)
{
  int sockfd;
  struct hostent *serv_ent;
  struct sockaddr_in  serv_addr;
  int status;

  char request[IO_LENGTH];

  /*
    Fill in the serv_addr structure
  */
  serv_ent = gethostbyname (Info->address);
  bzero (&serv_addr, sizeof (serv_addr));
  bcopy (serv_ent->h_addr, (char *)(& serv_addr.sin_addr), serv_ent->h_length);
  serv_addr.sin_port   = htons (Info->port);
  serv_addr.sin_family = AF_INET;

  if ( debug ) {
    printf("  VoltMeter_Connect: Opening %s:%s socket: %s, %d\n",
	   Info->motor, Info->name, Info->address, Info->port);
  }

  /*
    Check if there is an old socket connection.  If so, close it.
  */
  if ( Info->state.connected || Info->sockfd != 0 ) {
    printf("  VoltMeter_Connect: Closing old socket to %s:%s\n",
	   Info->motor, Info->name);
    close(Info->sockfd);
    Info->state.connected = 0;
    Info->sockfd = 0;
    usleep(500000);
  }

  /*
    Open a TCP socket (an Internet stream socket)
  */
  sockfd = socket (AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    sprintf( Info->in_line, "VoltMeter_Connect: Can't open stream socket to %s:%s\n\n", Info->motor, Info->name);
    Info->state.connected = 0;
    return(-1);
  }

  /*
    Connect to the server
  */
  if ( (status = connect (sockfd, (struct sockaddr *)(& serv_addr), sizeof (serv_addr)) ) < 0) {
    sprintf( Info->in_line, "VoltMeter_Connect: Can't connect to %s:%s server\n\n", Info->motor, Info->name);
    Info->state.connected = 0;
    return(-1);
  }

  /*
    Fill in the Info structure
  */

  Info->sockfd = sockfd;
  Info->state.connected = 1;
  Info->state.error = 0;

  /*
    Wait half a second to allow connect to complete before requesting if
      there are any errors
  */

  usleep(500000);
  
  strcpy( request, VOLTMETER_RETRIEVE_ERROR );
  strcat( request, "\012");
  status = VoltMeter_Get( Info, request);
  if ( status ) {
    sprintf( Info->in_line, "VoltMeter_Connect: Error communicating to %s:%s\n", Info->motor, Info->name);
    Info->state.error = 1;
    return(-1);
  }

  return(0);
}

/*=========================================
 * function: VoltMeter_Close
 *=========================================*/

int VoltMeter_Close (Motor_Info *Info, int debug)
{

  if ( debug ) {
    printf("  VoltMeter_Close: Closing %s:%s socket\n", Info->motor, Info->name);
  }

  /*
    Close the connection
  */
  close (Info->sockfd);

  /*
    Fill in the Info structure
  */
  Info->sockfd = 0;
  Info->state.connected = 0;
  Info->state.error = 0;

  return(0);
}
