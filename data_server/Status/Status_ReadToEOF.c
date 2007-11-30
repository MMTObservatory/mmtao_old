/*
  ==========================
  Status_ReadToEOF.c
  ==========================

  Routine to read multiple line from the server until
    and EOF or OK is received.
 
  Written  2feb07  DLM  Started with Client_ReadToEOF.c

*/

#include "PCR.h"
#include "Info.h"
#include "Status.h"

/*================================================================================
 * Read from socket till EOF or OK
 *================================================================================*/
int Status_ReadToEOF( Socket_Info *Info, char *Status_Reply, long *Status_Lines, int debug)
{
  /*
    Local variables to read next line
  */
  int line_length;
  char recvline[IO_LENGTH];

  /*
    Clear Status_Reply variable used to pass the server's reply back to
      Tcl
  */
  Status_Reply[0] = 0;

  /*
   * Now read lines to reply until "EOF"/"OK" .
   */
  if ( debug ) {
    printf("  Status_ReadToEOF:\n    Status Reply = \n");
    fflush(stdout);
  }

  *Status_Lines = -1;
  do {
    (*Status_Lines)++;
    /*
      Read a line (to \n) from the socket.  The returned string includes
        the \n.
    */
    line_length = Status_ReadLine(Info->sockfd, recvline, IO_LENGTH);
    if (line_length < 0) {
      printf("  Status_StringReadToEOF: Error reading line from socket\n");
      fflush(stdout);
      Socket_Close( (Socket_Info *)Info, debug);
      return(-1);
    }

    line_length = strlen(recvline);
    if (line_length < 0) {
      printf("  Status_StringReadToEOF: Error reading from socket\n");
      fflush(stdout);
      return(-1);
    }

    /*
      Put the proper string termination on the end of the current string
    */
    recvline[line_length] = 0;

    /*
      Check that there is enough room in Status_Reply to add current line
    */
    if( (strlen(Status_Reply) + line_length) > STATUS_REPLY_LENGTH ) {

      printf("  Status_ReadToEOF: Status_Reply string not long enough\n");
      printf("     while reading from %s\n", Info->name);
      printf("     => Increase the size of STATUS_REPLY_LENGTH in Status.h\n");
      fflush(stdout);

    } else {

      strcat( Status_Reply, recvline);

    }

    if ( debug ) {
      printf("      %s", recvline);
      fflush(stdout);
    }

  } while( !Status_EndOfData(recvline) );

  return(0);
}
