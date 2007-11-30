/*
  ==========================
  Status_ReadLine.c
  ==========================

  This routine Reads one line from the given socket.  The returned string
    includes the carriage return (\n)
 
  Written  2feb07  DLM  Started with Client_ReadLine.c

*/

#include "PCR.h"
#include "Info.h"
#include "Status.h"

/*================================================================================
 * Status_ReadLine: Read one line from socket
 *================================================================================*/
int Status_ReadLine( register int fd, register char *ptr, register int maxlen )
{
  int  n, rc;
  char c;

  for( n=1; n<maxlen; n++ ) {
    if( ( rc=read( fd, &c, 1 ) )==1 ) {
      *ptr++ = c;
      if( c=='\n' )
        break;
    } else if( rc==0 ) {
      if( n==1 )
        return(0); /* EOF, no data read */
      else
        break;    /* EOF, some data was read */
    } else {
      return(-1);  /* error */
    }
  }

  *ptr = 0;
  return(n);
}
