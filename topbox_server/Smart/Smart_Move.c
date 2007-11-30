/*
  Smart_Move.c

  Move the filter wheel.

  Modified  0sep07  tjt

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Smart.h"

int
Smart_MoveAbsolute ( Socket_Info *Socket, Device_Data *Data, double raw_pos, int debug)
{
    double pos;
    char cmd[STRING_LENGTH];
    int rv;

    if ( ! Data->home )
    	return -1;

    pos = raw_pos * Data->gain;

    if ( debug )
	printf ( "Smart %s Move Abs:  %.4f (%.4f)\n", Socket->name, raw_pos, pos );

    sprintf ( cmd, "P=%d", (int) pos );
    rv = Smart_Cmdw ( Socket, Data, cmd, debug );
    if ( rv )
	return -1;

    rv = Smart_Cmdw ( Socket, Data, "G", debug );
    if ( rv )
	return -1;

    return 0;
}


int
Smart_MoveRelative ( Socket_Info *Socket, Device_Data *Data, double raw_pos, int debug)
{
    double pos;
    char cmd[STRING_LENGTH];
    int rv;

    if ( ! Data->home )
    	return -1;

    pos = raw_pos * Data->gain;

    if ( debug )
	printf ( "Smart %s Move Rel:  %.4f (%.4f)\n", Socket->name, raw_pos, pos );

    sprintf ( cmd, "D=%d", (int) pos );
    rv = Smart_Cmdw ( Socket, Data, cmd, debug );
    if ( rv )
	return -1;

    rv = Smart_Cmdw ( Socket, Data, "G", debug );
    if ( rv )
	return -1;

    return 0;
}

int
Smart_MoveAbort ( Socket_Info *Socket, Device_Data *Data, int debug)
{
    int rv;

    if ( ! Data->home )
    	return -1;

    if ( debug )
	printf ( "Smart %s Move Abort!\n", Socket->name );

    rv = Smart_Cmdw ( Socket, Data, "S", debug );
    if ( rv )
	return -1;

    return 0;
}

/* THE END */
