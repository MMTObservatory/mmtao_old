/*=======================================================================
 * Info_SeeingArray.c
 *=======================================================================

  This routine sends the client all the stored Seeing or Strehl values
    along with their timestamps, starting with the oldest value.  The
    Seeing and Strehl values are kept in a circular array so when data
    are passed to the client, the position of the oldest datum must be
    determined and data flow starting from that position.

  Written  23may05  DLM  Started with Info_Get.c

  Altered  13dec07  DLM  Changed name from Info_ServerSeeingArray.c
                         Started with VME version and converted to PCR version

*/

#include <pthread.h>

#include "PCR.h"
#include "Info.h"

/*================================================================================*
 * Return an Info Parameter and its value
 *================================================================================*/
int Info_SeeingArray( int sockfd, Info_Array *Array)
{

  /* local variables */

  char reply[STRING_LENGTH];
  int i, status;

  /*
    The number of seeing values we have will determine how
      we send them
  */
  if ( Array->total_number <=0 ) {

    /*
      No data to send.  Send only EOF.  Client will have to determine what to do
    */

  } else if ( Array->total_number <= Array->array_size ) {

    /*
      The oldest data is in [0] so will will send from [0] to Array->total_number
    */
    for ( i=0; i<Array->total_number; i++ ) {

      sprintf(reply, "%s %f\n", Array->time[i], Array->value[i]);

      /*
	Return this Info Parameter
      */
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  Info_SeeingArray: Error in Socket_StringWrite sending number of bytes\n");
	return(-1);
      }

    }

  } else {

    /*
      The oldest data at the end of the circular array so send it first
    */
    for ( i=Array->position; i<Array->array_size; i++ ) {

      sprintf(reply, "%s %f\n", Array->time[i], Array->value[i]);

      /*
	Return this Info Parameter
      */
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  Info_SeeingArray: Error in Socket_StringWrite sending number of bytes\n");
	return(-1);
      }

    }

    /*
      Now send the beginning of the seeing array
    */
    for ( i=0; i<Array->position; i++ ) {

      sprintf(reply, "%s %f\n", Array->time[i], Array->value[i]);

      /*
	Return this Info Parameter
      */
      status = Socket_StringWrite( sockfd, reply, strlen(reply));
      if ( status != strlen(reply) ) {
	printf("  Info_SeeingArray: Error in Socket_StringWrite sending number of bytes\n");
	return(-1);
      }

    }

  }

  /*
    Return end of Info Parameter signal
  */
  strcpy( reply, "EOF\n");
  status = Socket_StringWrite( sockfd, reply, strlen(reply));
  if ( status != strlen(reply) ) {
    printf("  Info_SeeingArray: Error in Socket_StringWrite sending number of bytes\n");
    return(-1);
  }

  return(0);
}
