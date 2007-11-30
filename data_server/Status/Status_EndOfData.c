/*
  ==========================
  Status_EndOfData.c
  ==========================

  This routine checks to see if this the passed line is EOF
    or OK.  If EOF or OK is found, 1 is returned.  Else, 0
    is returned
 
  Written  2feb07  DLM  Started with Client_EndOfData.c

*/

#include "PCR.h"
#include "Info.h"
#include "Status.h"

/*================================================================================
 * Status_EndOfData: Check for "EOF" or "OK" indicating the end of the reply
 *================================================================================*/
int Status_EndOfData( char *input )
{
  if( ( strlen( input ) > 3 )
   && ( input[ 0 ] == '.' )
   && ( input[ 1 ] == 'E' )
   && ( input[ 2 ] == 'O' )
   && ( input[ 3 ] == 'F' ) )
    return(1);

  if( ( strlen( input ) > 2 )
   && ( input[ 0 ] == 'E' )
   && ( input[ 1 ] == 'O' )
   && ( input[ 2 ] == 'F' ) )
    return(1);

  if( ( strlen( input ) > 1 )
   && ( input[ 0 ] == 'O' )
   && ( input[ 1 ] == 'K' ) )
    return(1);

  return(0);
}
