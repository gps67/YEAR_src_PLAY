
#include "axis_draw.h"

/*!
	constructor - reasonable defaults
*/
axis_draw_y::axis_draw_y( w_widget0 * graph )
: axis_draw_base( graph )
, line_dotted( graph )
{
	digits_left = 3;
	digits_right = 1;
	val_step1 = 1;
	tick1_width = 7;
	tick2_width = 4;
	line_dotted.set_line_dotted();
	line_dotted.set_fg("brown");
	tick2_per_tick1 = 5;
}

/*!
	recalculate tick intervals, digit space, etc

	This is the rather cute auto-scaler, which hopes to ensure
	that tick2's are not too close together (presuming 5),
	and that tick1's land on sensible "whole number" values,
	with not too few and not too many ...

	INPUTS:
		scale.val_hi
		scale.val_lo
	OUTPUTS:
		val_step1	-- cornerstone

		digits_right
		digits_left -- not set though ?

	It works by presuming that the graph is of sensible size,
	and simply aiming for 3-10 main ticks (y_range5, whenere 5 =3 )
*/
void axis_draw_y::recalc_layout()	// after set_scale, y_hi
{
	digits_right = 0;
	digits_left = 0; // left of decimal point, 1 happens anyway
	flt80 unit = 1;
	flt80 y_range = scale.val_hi - scale.val_lo;

	y_range *= 1.09; // slight overrun (pref by step/3 as used below)

	if( y_range < 0 ) unit = -1;
	// if( y_range < 0 ) y_range = scale.val_lo - scale.val_hi;
	// if( y_range < 0 ) return;
	flt80 y_range5 = y_range / 3;

	// scale to 1 .1 .01 .001 ...
	while( unit > y_range5 ) {
		unit /= 10;	
		digits_right++;
		digits_right++;
		if( digits_right > 10 ) return;
	}

	// scale unit back up one and then 1 10 100 1000 ...
	while( unit < y_range5 ) {
		unit *= 10;
		digits_left++;
	}
	unit /= 10;	

	if( (y_range / unit ) > 10 ) unit *= 2;

	val_step1 = unit;

	e_print("scale.val_lo/y_step/scale.val_hi %4.*f %4.*f %4.*f \n",
		digits_right, (float) scale.val_lo,
		digits_right, (float) val_step1,
		digits_right, (float) scale.val_hi
	);
}

/*!
	The width for the y-axis markings, to the left of the axis
*/
int axis_draw_y::layout_width()
{
	return text.width1 * (digits_left + digits_right + 1 );
}

/*!
	sprintf the value label into a static buffer
*/
char * axis_draw_y::str_val( flt80 val )
{
	static char buff[40];
	if( val_step1 >= 1 )
	{
		sprintf( buff, "%3.*f", 0, (float)val );
	} else {
		sprintf( buff, "%4.*f", digits_right, (float)val );
	}
	sprintf( buff, "%4.*f", digits_right, (float)val );
	return buff;
}

/*!
	draw the y axis (ticks, numbers, etc )left of x1
	and draw horizontal grid-lines across the main area to x2

	x1 is where the vertical line
*/
void axis_draw_y::draw_all( int x1, int x2 )
{
	int lft = x1 - 1;
	int rgt = x2;
	int x_tick2 = lft - tick2_width;
	int x_tick1 = lft - tick1_width;
	int x_text1 = lft - tick1_width - 2;

	//! y_val is the VAL of the current tick
	flt80 y_val = val_step1 * rint( scale.val_lo / val_step1 );

	// val_step1 is the major tick step (minor is 1/5 of that)
	y_val -= val_step1; // step back then step forwards in loop

	// y_no_label is the VAL point (at the top) when labels stop
	flt80 y_no_label = (scale.val_hi - val_step1 * 2 / 10 ); // LURK redo as pixels

	while ( y_val <= scale.val_hi )
	{
		y_val += val_step1;
		int y_tick = scale.scn_of_val( y_val );

		char * s = str_val( y_val );

		// draw the label
		if( y_val < y_no_label )
		{
			text.goto_y_tick( y_tick );
			text.draw_xright_str( x_text1, s );
		}

		// draw the tick1
		draw_line( gc, x_tick1, y_tick, lft, y_tick );

		// draw the main area dotted line
		draw_line( line_dotted, lft, y_tick, rgt, y_tick );

		// draw the tick2's at 1/5 of tick1 (1,2,3,4)/5
		for( int i = 1; i<tick2_per_tick1; i++ )
		{
			flt80 y_t2_val = y_val + i*val_step1/tick2_per_tick1 ;
			int y_t2 = scale.scn_of_val( y_t2_val );
			draw_line( gc, x_tick2, y_t2, lft, y_t2 );
		}
	}
}








