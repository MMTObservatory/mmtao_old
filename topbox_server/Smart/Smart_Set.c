/*
  Smart_Set.c

  Written  12sep07  tjt

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Smart.h"

int
Smart_SetPosition ( Socket_Info *Socket, Device_Data *Data, int debug)
{
    char buf[STRING_LENGTH];
    int rv;
    int position;

    if ( strcmp ( Data->name, "Focus" ) == 0 ) {
	position = 0;
    } else {	/* Rotation */
    	rv = Smart_ReadEncoder ( &position, debug );
	if ( rv ) return -1;
    }

    sprintf ( buf, "O=%d", position );
    rv = Smart_Cmd ( Socket, Data, buf, buf, debug );
    if ( rv ) return -1;

    /* set the homed variable */
    rv = Smart_Cmd ( Socket, Data, "h=1", buf, debug );
    if ( rv ) return -1;

    return 0;
}

int
Smart_SetSpeed ( Socket_Info *Socket, Device_Data *Data, double speed, int debug)
{
    double val;
    char cmd[STRING_LENGTH];
    int rv;

    val = speed * Data->gain;
    sprintf ( cmd, "vm=%5.2f", val );

    /* Define current position as zero */
    rv = Smart_Cmdw ( Socket, Data, cmd, debug );
    if ( rv )
	return -1;

    return 0;
}

/* THE END */
