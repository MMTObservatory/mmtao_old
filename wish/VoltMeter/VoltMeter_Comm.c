/*===========================================================================*
**
**  VoltMeter_Comm.c
**
**      VoltMeter_Flush()
**      VoltMeter_Read()
**      VoltMeter_Write()
**
**  Utility routines to read and write to a motor
**
**  Returns :  0 success
**            -1 error
**
**  Written  30mar04  DLM  Started with OAP_Comm.  Trying to make this a
**                           a generic motor driver.
**
**===========================================================================*/

#include "PCR_Wish.h"
#include "VoltMeter.h"

/*=========================================
 * function: VoltMeter_Flush
 *
 * Altered  21jan04  DLM  Previously OAP_Flush called OAP_Read, which uses
 *                          a select to wait for data.  This is not needed for
 *                          for a flush, plus it put in a delay.  Now just
 *                          read as fast as possible until the socket buffer is
 *                          empty
 *
 *=========================================*/

int VoltMeter_Flush (Motor_Info *Info)
{
  struct timeval delay;
  fd_set mySet;
  char buff [MOTOR_MAXLINE];
  int n;

  /*
    Set time to wait to 0
  */
  FD_ZERO (&mySet);
  FD_SET (Info->sockfd, &mySet);
  delay.tv_sec  = 0;
  delay.tv_usec = 0;

  /*
    Read until socket is empty
  */
  do {
    n =0;
    if ( select (FD_SETSIZE, &mySet, NULL, NULL, &delay) ) {
      n = read (Info->sockfd, buff, MOTOR_MAXLINE);
      //      printf("flush %d >%s<\n", n, buff);
    }
  } while ( n > 0 );

  return(0);
}

/*=========================================
 * function: VoltMeter_Write
 *=========================================*/

int VoltMeter_Write (Motor_Info *Info)
{
  int i;
  int debug_local = 0;

  if ( debug_local ) {
    printf("    VoltMeter_Write: Writing %s:%s --> %s", Info->motor, Info->name, Info->out_line);
  }

  /* 
     Flush any unexpected input
  */
  if ( (i = VoltMeter_Flush (Info)) ) return(i);

  /*
    Write out_line to sockfd
  */
  if ( (i = write (Info->sockfd, Info->out_line, strlen (Info->out_line)) ) !=  strlen (Info->out_line) ) {
    sprintf( Info->in_line, "VoltMeter_Write timed out in %d seconds", MOTOR_DELAY_S);
    if ( strncmp( Info->out_line, VOLTMETER_RETRIEVE_ERROR, strlen( VOLTMETER_RETRIEVE_ERROR )) ){
      Info->state.connected = 0;
      Info->state.error = 1;
    }
    return(i);
  }

  return(0);
}

/*=========================================
 * function: VoltMeter_Read
 *
 * Altered  21jan04  DLM  Previously only the carriage return ('\n' char(13))
 *                          denoting the beginning of string was used to start
 *                          the string.  Now include a search for a line feed
 *                          ('\r' char(10)) denoting the end of string.  Also
 *                          check for carriage return and NULL (char(0)).
 *                         
 *=========================================*/

int VoltMeter_Read (Motor_Info *Info)
{
  struct timeval delay;
  fd_set mySet;
  char buff [MOTOR_MAXLINE];
  int n, i;
  int debug_local = 0;

  /*
    Read data from the motor (open socket => Info->sockfd)
      => select is used to wait for the requested data to become
         available.
      => if nothing to read, return(1)
  */
  FD_ZERO (&mySet);
  FD_SET (Info->sockfd, &mySet);
  delay.tv_sec  = MOTOR_DELAY_S;
  delay.tv_usec = MOTOR_DELAY_U;

  /*
    If there was nothing to read from sock_fd, return
  */
  if (!select (FD_SETSIZE, &mySet, NULL, NULL, &delay) ) {
    sprintf( Info->in_line, "VoltMeter_Read timed out in %d seconds", MOTOR_DELAY_S);
    if ( strncmp( Info->out_line, VOLTMETER_RETRIEVE_ERROR, strlen( VOLTMETER_RETRIEVE_ERROR )) ){
      Info->state.connected = 1;
      Info->state.error = 1;
    } else {
      Info->state.connected = 0;
      Info->state.error = 1;
    }
    return(1);
  }

  /*
    Read in_line from sockfd
  */
  n = read (Info->sockfd, buff, MOTOR_MAXLINE);

  /*
    If the read didn't work, the connection died
  */
  if (n <= 0) return(-1);

  /*
    The return string ends usually at linefeed character ('\r' or char(10))
      => Also check for a carriage return ('\n' or char(13))
      => and NULL (char(0)).
  */
  for (i = 0; (buff [i] != 0 &&  buff [i] != 10 &&  buff [i] != 13 && i<n) ; i++) {
    Info->in_line [i] = buff [i];
  }

  /*
    Add a NULL to the end of the returned string
  */
  Info->in_line [i] = 0;

  if ( debug_local ) {
    printf("    VoltMeter_Read:  Reading %s:%s --> %s\n", Info->motor, Info->name, Info->in_line);
  }

  if ( (strncmp( Info->out_line, VOLTMETER_RETRIEVE_ERROR, strlen( VOLTMETER_RETRIEVE_ERROR )) == 0) &&
       (strncmp( Info->in_line, "+0", 2)) ) {
    Info->state.connected = 1;
    Info->state.error = 1;
    return(-1);
  }

  return(0);
}
