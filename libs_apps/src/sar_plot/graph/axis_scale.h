#ifndef axis_scale_H
#define axis_scale_H

#include "ints.h"

/*!
	axis_scale linear keeps the lo/hi as pixels and values

	It provides functions to convert VAL to POS
*/
class axis_scale_linear : public GRP_sar_plot
{

	void recalc();
 public:

	int scn_lo;
	int scn_hi;

	flt80 val_lo;
	flt80 val_hi;

	flt80 val_per_pix;
	flt80 pix_per_val;

	axis_scale_linear()
	{
		scn_lo = 0;
		scn_hi = 1; // no div zero
		val_lo = 0;
		val_hi = 1; // ditto 1/x
	}

	void set_scn_val_range( int scn1, int scn2, flt80 val1, flt80 val2 );
	void set_val_range( flt80 v1, flt80 v2 );
	void set_scn_range( int v1, int v2 );

	flt80 val_of_scn( int scn );
	int scn_of_val( flt80 val );

	//! Useful for when VAL type is unix time, not float
	time_t time_val_of_scn( int scn )
	{
		return (time_t) val_of_scn( scn );
	}

};

#endif
