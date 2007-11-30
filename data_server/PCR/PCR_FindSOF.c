/*
  ==========================
  PCR_FindSOF.c
  ==========================

  This routine reads from the given socket until 'SOF' is
    found (returns 0) or the connection is closed (returns -1)
 
  Written   3may06  DLM  Started with Client_EndOfData.c

*/

#include "PCR.h"
#include "DServ.h"

/*================================================================================
 * PCR_FindSOF: find SOF from the stream socket
 *================================================================================*/
int PCR_FindSOF( char *sof, int sockfd, int debug)
{

  char reply[STRING_LENGTH];
  char *nxtChar;

  reply[0] = 0;
  nxtChar = reply;

  do {

    while ( nxtChar[0] != 'S' ) {
      /*
	Read one character until 'S' is found
      */
      if ( Socket_Read( sockfd, nxtChar, sizeof(char)) ) {
	if ( debug ) {
	  printf("  PCR_FindSOF: Error reading S in Socket_Read\n");
	  fflush(stdout);
	}
	return(-1);
      }
    }

    nxtChar++;

    /*
      Read one character and check that it is '0'
    */
    if ( Socket_Read( sockfd, nxtChar, sizeof(char)) ) {
      if ( debug ) {
	printf("  PCR_FindSOF: Error reading O in Socket_Read\n");
	fflush(stdout);
      }
      return(-1);
    }

    if ( !strncmp ( nxtChar, "S", sizeof(char) ) ) {
      continue;
    }

    nxtChar++;

    /*
      Read one character and check that it is 'F'
    */
    if ( Socket_Read( sockfd, nxtChar, sizeof(char)) ) {
      if ( debug ) {
	printf("  PCR_FindSOF: Error reading F in Socket_Read\n");
	fflush(stdout);
      }
      return(-1);
    }

    if ( !strncmp ( reply, "F", sizeof(char) ) ) {
      continue;
    } else {
      strcpy( sof, (nxtChar-2));
      return(0);
    }

  } while (1);

  /*
    Should never get here
  */
  return(-2);

}
