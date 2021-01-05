
#include "w_graph3.h"
#include "sar_top_app.h"
#include "dgb_fail.h"

w_graph3::w_graph3( sar_top_app * _owner )
: w_graph2()
, owner( _owner )
{
	if(!owner)
	{
		e_print("w_graph3::Constructor() - NULL owner\n");
		THROW_dgb_fail( "w_graph3::Constructor() - NULL owner");
	}
}

/*!
	When the mouse is moved, the crosshairs are redrawn
	and the owner app told
*/
void w_graph3::event_mouse_moved(int x, int y)
{
	xor_grid_hairs( x, y );
	//
	time_t t_hair = xmap.time_val_of_scn( x );
	flt80   yval  = ymap.val_of_scn( y );
	if(owner) owner->cross_hairs_at( t_hair, yval );
	else
		e_print("w_graph3::event_mouse_moved() - NULL owner\n");
}
