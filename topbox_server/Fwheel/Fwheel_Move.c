/*
  Fwheel_Move.c

  Move the filter wheel.

  Modified  24aug07  tjt

*/

#include "Topbox.h"
#include "Device.h"
#include "Status.h"
#include "Fwheel.h"

/*=========================================
 * Send a absolute move request to the device
 *=========================================*/

int
Fwheel_MoveCount ( Socket_Info *Socket, Device_Data *Data, int count, int debug)
{
	Data->moving = 1;
	while ( count-- > 0 ) {
	    Fwheel_Step ( Socket, debug );
	    usleep ( FW_MOVE_DELAY );
	}
	Data->moving = 0;
	return 0;
}

/* Doug used this to move 1/3 of a position (11 steps) to correct
 * funky out of position vignetting errors and such
 */
int
Fwheel_MoveThird ( Socket_Info *Socket, Device_Data *Data, int debug)
{
    return Fwheel_MoveCount ( Socket, Data, FW_STEPSIZE / 3, debug );
}

static char *filternames[] = {
 "ND-2.0", "ND-1.5", "ND-1.0", "ND-0.5",
 "Clear", "Z-Band", "ND-3.0", "ND-2.5" };

void
Fwheel_FilterSetPos ( Device_Data *Data, int pos )
{
    Data->position = pos;

    if ( pos < 1 || pos > FW_NFILTERS )
	strcpy ( Data->filter, "bogus" );

    strcpy ( Data->filter, filternames[pos-1] );
}

/* As far as we know, this thing only moves one way, forward!
 */
int
Fwheel_MoveAbsolute ( Socket_Info *Socket, Device_Data *Data, double position, int debug)
{
    int cur_pos = Data->position;
    int new_pos = position;
    int count;
    int at_home;
    int rv;

    if ( ! Data->home )
    	return -1;

    while ( new_pos < cur_pos )
    	new_pos += FW_NFILTERS;

    while ( new_pos-FW_NFILTERS >= cur_pos )
    	new_pos -= FW_NFILTERS;

    if ( debug )
	printf ( "FW Move Abs:  %d --> %d\n", cur_pos, new_pos );

    if ( new_pos == cur_pos )
    	return 0;

    if ( new_pos <= FW_NFILTERS ) {
	/* simple case, we don't cross home */
	count = (new_pos - cur_pos ) * FW_STEPSIZE;
	rv =  Fwheel_MoveCount ( Socket, Data, count, debug );
    } else {
	/* Move to position 8, then find home, then on */
	if ( cur_pos < FW_NFILTERS ) {
	    count = (FW_NFILTERS - cur_pos ) * FW_STEPSIZE;
	    (void) Fwheel_MoveCount ( Socket, Data, count, debug );
	}

	at_home = Fwheel_SeekHome ( Socket, Data, FW_STEPSIZE, debug );
	if ( ! at_home ) {
	    Fwheel_FilterSetPos ( Data, 0 );
	    Data->home = 0;
	    return -1;
	}

	(void) Fwheel_MoveCount ( Socket, Data, FW_OFFSET, debug );
	cur_pos = 1;

	while ( new_pos-FW_NFILTERS >= cur_pos )
	    new_pos -= FW_NFILTERS;

	if ( new_pos > cur_pos ) {
	    if ( debug )
		printf ( "FW Move Abs:  1 --> %d\n", new_pos );
	    count = (new_pos - 1) * FW_STEPSIZE;
	    rv =  Fwheel_MoveCount ( Socket, Data, count, debug );
	}
    }

    Fwheel_FilterSetPos ( Data, new_pos );

    return rv;
}

int
Fwheel_MoveRelative ( Socket_Info *Socket, Device_Data *Data, double delta, int debug)
{
    return Fwheel_MoveAbsolute ( Socket, Data, Data->position + delta, debug );
}

/* THE END */
