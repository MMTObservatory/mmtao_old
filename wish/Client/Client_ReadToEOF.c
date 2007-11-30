/*
  ==========================
  Client_ReadToEOF.c
  ==========================

  Routine to read multiple line from the server until
    and EOF or OK is received.
 
  Written  16jun05  DLM  Started with TCS_Request.

*/

#include "PCR_Wish.h"
#include "Client.h"

/*================================================================================
 * Read from socket till EOF or OK
 *================================================================================*/
int Client_ReadToEOF( Client_Info *Info, char *Client_Reply, char *errorMsg, int debug)
{
  /*
    Local variables to read next line
  */
  int line_length;
  char recvline[IO_LENGTH];

  /*
    Clear Client_Reply variable used to pass the server's reply back to
      Tcl
  */
  Client_Reply[0] = 0;

  /*
   * Now read lines to reply until "EOF"/"OK" .
   */
  if ( debug ) {
    printf("  Client_ReadToEOF:\n    Client Reply = \n");
  }

  
  do {

    /*
      Read a line (to \n) from the socket.  The returned string includes
        the \n.
    */
    line_length = Client_ReadLine(Info->fd, recvline, IO_LENGTH);
    if (line_length < 0) {
      strcpy( errorMsg, "Client_StringReadToEOF: Error reading from socket");
      return(-1);
    }

    /*
      Put the proper string termination on the end of the current string
    */
    recvline[line_length] = 0;

    /*
      Check that there is enough room in Client_Reply to add current line
    */
    if( (strlen(Client_Reply) + line_length) > CLIENT_REPLY_LENGTH ) {

      strcpy( errorMsg, "Client_ReadToEOF: Client_Reply string not long enough\n");
      printf("%s", errorMsg);
      printf("   while reading from %s\n", Info->name);
      printf("   => Increase the size of CLIENT_REPLY_LENGTH in AO_Wish.h\n");

    } else {

      strcat( Client_Reply, recvline);

    }

    if ( debug ) {
      printf("      %s", recvline);
    }

  } while( !Client_EndOfData(recvline) );

  return(0);
}
