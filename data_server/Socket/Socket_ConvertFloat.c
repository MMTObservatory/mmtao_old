/************************************************************************

  Socket_ConvertFloat.c

  Convert the given string from big endian to little endian.  This
    is basically performing a ntohl() or htonl().

  Written  12dec05  DLM  Needed to send a float vector from the Mac
                           powerpc to an i86 linux machine

************************************************************************/

#include "Socket.h"

/************************************************************************
 * Convert the float
 ************************************************************************/
int Socket_ConvertFloat( float *raw, float *flipped, int num)
{
  int i;
  char *raw_char, *flipped_char;

  raw_char = (char *)raw;
  flipped_char = (char *)flipped;
  for ( i=0; i<num; i++) {
    *flipped_char++ = raw_char[3];
    *flipped_char++ = raw_char[2];
    *flipped_char++ = raw_char[1];
    *flipped_char++ = raw_char[0];
    raw_char += 4;
  }

  return(0);
}

