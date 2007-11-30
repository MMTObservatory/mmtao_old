/*
  ==========================
  Info_AverageSeeing.c
  ==========================

  This routine convert the Info_Reply string into two arrays, a time history
    of seeing values and the times of these measurements.  It then calculates
    the running average of the seeing.  This value is written into the Info
    server on the VME.  This value can be used to set the seeing dependent
    loop gain.
 
  Written  10feb06  DLM

*/

#include "PCR_Wish.h"
#include "Server_Ports.h"
#include "Info.h"

/*================================================================================
 * Calculate the running average seeing
 *================================================================================*/
int Info_AverageSeeing( char *Info_Reply, double *Seeing_Average, char *errorMsg, int debug)
{
  /*
    Local variables to read next line
  */
  double Seeing_Time[SEEING_VALUES];
  double Seeing_Value[SEEING_VALUES];

  char seeing_reply[CLIENT_REPLY_LENGTH];
  char temp[SEEING_VALUES][STRING_LENGTH];
  char temp_time[STRING_LENGTH];

  int num, count, i;
  char *next;
  double hours, minutes, seconds;

  double Seeing_Avg;
  double Seeing_Weight = 0.2;
  double Seeing_MaxTime = 0.5;
  double Seeing_MinValue = 0.3;
  double Seeing_MaxValue = 1.5;
  int Seeing_MinCount = 3;

  int debug_local = 0;

  /* Time variables */

  time_t *tp;
  struct tm *cur_time;
  double current_time;

  /*
    Put returned string it a new string because this routine corrupts
      the string it is operating on
  */
  strcpy( seeing_reply, Info_Reply);

  /*
    Split up the seeing_reply at each carriage return (\n) and store in a temporary
      array
  */
  num=0;
  next = strtok( seeing_reply, "\n");
  if ( strncmp(next, "EOF", strlen("EOF")) ) {
    strcpy(temp[num], next);
  } else {
    if ( debug ) {
      printf("  Info_AverageSeeing: No seeing values available\n");
    }
    return(0);
  }

  while ( strncmp( next = strtok(NULL, "\n") , "EOF", strlen("EOF") ) ) {
    num++;
    strcpy(temp[num], next);
  }

  /*
    Parse the temporary array, convert the time and seeing value to floats
      and store to Seeing_Time and Seeing_Value arrays
  */
  for ( i=0; i<=num; i++) {
    next = strtok( temp[i], " ");
    next = strtok( NULL, " ");
    next = strtok( NULL, " ");
    next = strtok( NULL, " ");
    strcpy( temp_time, next);
    next = strtok( NULL, " ");
    next = strtok( NULL, " ");
    Seeing_Value[i] = atof( next);
    hours = atof( next = strtok( temp_time, ":") );
    minutes = atof( next = strtok( NULL, ":") );
    seconds = atof( next = strtok( NULL, ":") );
    Seeing_Time[i] = hours + minutes/60.0 + seconds/3600.0;
    /*
      This method gave compiler warnings.  30nov07  DLM

      Seeing_Time[i] = atof( next = strtok( temp_time, ":") ) +
          atof( next = strtok( NULL, ":") ) / 60.0;
          atof( next = strtok( NULL, ":") ) / 60.0 +
          atof( next = strtok( NULL, ":") ) / 3600.0;
    */
    if ( debug ) {
      printf("  Info_AverageSeeing: Seeing[%d] = %f  %f\n", i, Seeing_Time[i], Seeing_Value[i]);
    }
  }

  /*
    Determine current UT time
  */
  tp = (time_t *)malloc( sizeof(time_t) );
  time( (time_t *)tp );
  cur_time = gmtime(  tp);
  current_time = cur_time->tm_hour + cur_time->tm_min/60.0 + cur_time->tm_sec/3600.0;
    if ( debug ) {
      printf("  Info_AverageSeeing: Current Time = %f\n", current_time);
    }

  /*
    Calculate running average.  We limit the amount of lookback time in hours
      with Seeing_MaxTime, the lowest seeing value used by MinValue and the
      highest seeing value by Seeing_MaxValue.

    Find the first value which meets the time criterium
  */
  Seeing_Avg = 0.0;
  for ( i=0; i<=num; i++ ) {
    if ( debug_local ) {
      printf("  Info_AverageSeeing: Delta Time %f\n", current_time - Seeing_Time[i]);
    }
    if ( (current_time - Seeing_Time[i])  < Seeing_MaxTime )  {
      Seeing_Avg = Seeing_Value[i];
      if ( debug_local ) {
	printf("  Info_AverageSeeing: used %d\n", i);
      }
      break;
    } else {
      if ( debug_local ) {
	printf("  Info_AverageSeeing: not %d\n", i);
      }
    }
  }
  count = 0;
  /*
    Find and use the seeing values between the maximum and minimum values
  */
  for ( i=i; i<=num; i++ ) {
    if ( ( Seeing_Value[i] >= Seeing_MinValue ) &&
	 ( Seeing_Value[i] <= Seeing_MaxValue ) ) {
      Seeing_Avg = Seeing_Avg * (1.0 - Seeing_Weight) + Seeing_Value[i] * Seeing_Weight;
      count++;
      if ( debug_local ) {
	printf("  Info_AverageSeeing: used %d  %f  %f\n", i, Seeing_Value[i], Seeing_Avg);
      }
    } else {
      if ( debug_local ) {
	printf("  Info_AverageSeeing: not %d  %f  %f\n", i, Seeing_Value[i], Seeing_Avg);
      }
    }
  }

  if ( count < Seeing_MinCount ) {
    if ( debug ) {
      printf("  Info_AverageSeeing: too few usable values: %d\n", count);
    }
    *Seeing_Average = 0.0;
  } else {
    *Seeing_Average = Seeing_Avg;
  }

  return(0);
}
