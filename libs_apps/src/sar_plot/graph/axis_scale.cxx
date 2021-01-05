
#include "axis_scale.h"
#include <math.h>

/*!
	recalculate val_per_pix and pix_per_val
*/
void axis_scale_linear::recalc()
{
	val_per_pix =  (val_hi - val_lo) / (flt80)(scn_hi - scn_lo);
	pix_per_val =  1.0 / val_per_pix;
}

/*!
	set the screen pix range, and value range
*/
void axis_scale_linear::set_scn_val_range( int s1, int s2, flt80 v1, flt80 v2 )
{
	if( v1 == v2 ) v2 = v1 + 1;
	val_lo = v1;
	val_hi = v2;
	if( s1 == s2 ) s2 = s1 + 1;
	scn_lo = s1;
	scn_hi = s2;
	recalc();
}

/*!
	set the lo and hi values
*/
void axis_scale_linear::set_val_range( flt80 v1, flt80 v2 )
{
	if( v1 == v2 ) v2 = v1 + 1;
	val_lo = v1;
	val_hi = v2;
	recalc();
}

/*!
	set the lo and hi screen positions
*/
void axis_scale_linear::set_scn_range( int s1, int s2 )
{
	if( s1 == s2 ) s2 = s1 + 1;
	scn_lo = s1;
	scn_hi = s2;
	recalc();
}

/*!
	calc float val corresponding to screen (pixel) pos
*/
flt80 axis_scale_linear::val_of_scn( int scn )
{
	return (flt80)(scn-scn_lo) * val_per_pix + val_lo;
}

/*!
	calc integer screen pos of floating value
*/
int axis_scale_linear::scn_of_val( flt80 val )
{
	return (int)rint((val-val_lo) * pix_per_val + scn_lo);
}


