#include "axis_draw_xtime.h"
#include "tm_parts.h"

axis_draw_xtime::axis_draw_xtime( w_widget0 * graph )
: axis_draw_base( graph )
, line_dotted( graph )
, fmt_dow( fmt_dow_none )
, fmt_mon( fmt_mon_none )
{
}

flt80 axis_draw_xtime::calc_pix_per_second()
{
	return scale.pix_per_val;
}

flt80 axis_draw_xtime::calc_pix_per_hour()
{
	return scale.pix_per_val * 60 * 60;
}

flt80 axis_draw_xtime::calc_pix_per_day()
{
	return scale.pix_per_val * 60 * 60 * 24;
}

flt80 axis_draw_xtime::layout_height()
{
	return (flt80) y_btm;
}

void axis_draw_xtime::recalc_layout_tod()
{
	// caller must add tod_tick1_height to y_pos

	flt80 pix_per_hour = calc_pix_per_hour();
	// flt80 pix_per_day = pix_per_second * 60 * 60 * 24;
	// flt80 pix_per_month = pix_per_day * 30; // approx

	flt80 w1 = text.string_width("W");
	flt80 w0 = rint( (w1-3.0) / 3.0)+1.0; // threshold min width between ticks
	      w1 = w0 * 3.0;
	flt80 w3 = w1 * 3.0;

	flt80 h1 = text.font_ascent + text.font_descent; // tick size
	h1 = rint((h1-4)/4) + 1;

	// tod_ time of day
	tod_tick1_dt = 1;
	tod_tick1_height = 0;
	tod_tick2_dt = 1;
	tod_tick2_height = 0;
	tod_tick3_dt = 1;
	tod_tick3_height = 0;
	tod_tick4_dt = 1;
	tod_tick4_height = 0;

	/*
		If a day is smaller than a WWW, forget it
	*/
	if( (pix_per_hour * 24) < (w1*3) )
	{
		return;
	}

	/*
		If 1 hour is smaller than a W, forget it
	*/
	if( (pix_per_hour * 1) < (w1*1) )
	{
		return;
	}

	/*
		If 1 hour is smaller than a WW, use 6, 3, 1 hour ticks
	*/
	if( (pix_per_hour * 1) < (w1*2) )
	{
		tod_tick1_height = h1*3;
		tod_tick1_dt = 60 * 60 * 6;
	
		tod_tick2_height = h1*2;
		tod_tick2_dt = 60 * 60 * 3;
	
		tod_tick3_height = h1*1;
		tod_tick3_dt = 60 * 60 * 1;
	
		return;
	}

	/*
		If 1/4 hour is smaller than a WW, use 6, 3, 1, 0.25 hour ticks
	*/
	if( (pix_per_hour * 1) < (w1*2) )
	{
		tod_tick1_height = h1*4;
		tod_tick1_dt = 60 * 60 * 6;
	
		tod_tick2_height = h1*3;
		tod_tick2_dt = 60 * 60 * 3;
	
		tod_tick3_height = h1*2;
		tod_tick3_dt = 60 * 60 * 1;
	
		tod_tick4_height = h1*1;
		tod_tick4_dt = 60 * 30;
	
		return;
	}

	/*
		Zoomed into an hour or two, early ticks are bigger
	*/
	{
		tod_tick1_height = h1*4;
		tod_tick1_dt = 60 * 60;
	
		tod_tick2_height = h1*3;
		tod_tick2_dt = 60 * 30;
	
		tod_tick3_height = h1*2;
		tod_tick3_dt = 60 * 15;
	
		tod_tick4_height = h1*1;
		tod_tick4_dt = 60 *  5;
	
		return;
	}

}

/////////////////////////////////////////////////////////////////////////
	//  day of week
/////////////////////////////////////////////////////////////////////////

void axis_draw_xtime::recalc_layout_dow()
{
	int y_dow_top = y_pos;

	flt80 pix_per_day = calc_pix_per_day();
	pix_per_day *= 0.9; // slight extra gap // some bars closer by pixel

	fmt_dow = fmt_dow_none;
	/*
		day_of_week_format
	*/
	if( pix_per_day > text.string_width(" Wednesday "))
	{
		fmt_dow = fmt_dow_Wednesday;
	}
	else if( pix_per_day > text.string_width(" Wed "))
	{
		fmt_dow = fmt_dow_Wed;
	}
	else if( pix_per_day > text.string_width("W"))
	{
		fmt_dow = fmt_dow_W;
	}
	else if( pix_per_day >= 2 )
	{
		fmt_dow = fmt_dow_ticks;
	}
	else
	{
		fmt_dow = fmt_dow_none;
	}

	switch( fmt_dow ) {
	 case fmt_dow_none:
		y_dow_baseline = y_dow_top;
		y_dow_btm = y_dow_top;
	 break;
	 case fmt_dow_ticks:
		y_dow_baseline = y_pos - y_dow_top; // unused
		y_pos += 4; // 4 ticks
		y_dow_btm = y_pos - y_dow_top; 
	 break;
	 default:
		y_pos += text.font_descent; // typical gap
		y_pos += text.font_ascent; // Top of font to baseline
		y_dow_baseline = y_pos - y_dow_top;
		y_pos += text.font_descent; // baseline to bottom of font
		y_dow_btm = y_pos - y_dow_top;
	}

}


/////////////////////////////////////////////////////////////////////////
	//  month names
/////////////////////////////////////////////////////////////////////////

void axis_draw_xtime::recalc_layout_mon()
{
	fmt_mon = fmt_mon_none;
	flt80 pix_per_month = calc_pix_per_day() * 28; // APPROX
	if( pix_per_month > text.string_width(" February ") )
	{
		flt80 scn_width = scale.scn_hi - scale.scn_lo;
		flt80 ndays_shown = scn_width / calc_pix_per_day(); // approx
		if( ndays_shown < 5 )
			fmt_mon = fmt_mon_none;
		else
			fmt_mon = fmt_mon_January;
	}
	else if( pix_per_month > text.string_width(" Feb ") )
		fmt_mon = fmt_mon_Jan;
	else if( pix_per_month > text.string_width("F ") )
		fmt_mon = fmt_mon_J;
	else if( (pix_per_month *2) > text.string_width(" ") )
		fmt_mon = fmt_mon_ticks;
	/////	fmt_mon_Quarters
	else
		fmt_mon = fmt_mon_none;

	switch( fmt_mon ) {
	 case fmt_mon_none:
	 break;
	 case fmt_mon_ticks:
		y_pos += 4;
	 break;
	 default:
		y_pos += text.font_padded_single_line_height;
	 break;
	}

}

/*
	caller has called set_scale( scale )
*/
void axis_draw_xtime::recalc_layout()
{
	/*
		recalc_layout will change the amount of space available
		above the line, and the ypos where the line is placed
		(zero is not zero).

		This will need to be called twice, and also to
		shift the recorded ypos's 
	*/
	e_print("recalc_layout()\n" );

	y_pos = 0; // relative to axis for layout, absolute for draw

	// time of day
	recalc_layout_tod();
	y_pos += tod_tick1_height; // function expects caller to do this for it

	// day of week
	recalc_layout_dow();

	//  month
	recalc_layout_mon();

	// year
	// hopefully obvious ?

/////////////////////////////////////////////////////////////////////////

	y_pos += 10; // a gap
	y_btm = y_pos;

	return;
}

void axis_draw_xtime::draw_all( int y0, int y2 )
{
	// Y0 = y0;
	y_pos = y0; // this time y_pos is absolute not rel to 0

	draw_tod_line();
	draw_dow_line();
	draw_mon_line();

/////////////////////////////////////////////////////////////////////////
	// zero axis lines
/////////////////////////////////////////////////////////////////////////

#if 0
	/*
		this hline COVERS the y dotted line
	*/
	draw_horiz_line( y_pos );
#else

	int btm = y0;
	int top = y2; // + 1;
	// int btm0 = btm - 1; // slight drop for baseline
	btm ++;

	int lft = scale.scn_lo;
	int rgt = scale.scn_hi;

	d_gc & gc_black = gc;

	draw_line( gc_black, lft, btm,  rgt, btm );	// btm
	draw_line( gc_black, lft, top,  lft, btm );	// lft
	// move above line to yaxis draw
	if(0) {
	draw_line( gc_black, lft, top,  rgt, top );	// top
	draw_line( gc_black, rgt, top,  rgt, btm );	// rgt
	}
	if(0) {
	draw_line( gc_black, lft, top,  rgt, btm );	// \ #
	draw_line( gc_black, rgt, top,  lft, btm );	// /
	}

	if(0) {
	draw_line( gc_black, rgt+1, top-1,  lft-1, btm+1 );	// /
	draw_line( gc_black, lft-1, top-1,  rgt+1, btm+1 );	// \ #
	}
#endif

/////////////////////////////////////////////////////////////////////////
	// zero axis lines
/////////////////////////////////////////////////////////////////////////
}

void axis_draw_xtime::tick_each( flt80 y_lo, flt80 y_hi, flt80 tick_dt )
{
	if( y_lo == y_hi ) return;
	if( tick_dt < 1 ) return;

	int y_top = y_lo;
	int y_btm = y_hi;

	int t_lo = (time_t) scale.val_lo;
	int t_hi = (time_t) scale.val_hi;

	d_gc & gc_black = gc;

	int lft = scale.scn_lo;
	int rgt = scale.scn_hi - 1;

	// start off with a day aligned time (LURK later)
	tm_parts tm1;
	tm1.local_from_time( t_lo );
	tm1.hour( 0 );
	tm1.min( 0 );
	tm1.sec( 0 );
 if(!	tm1.mktime_from_local() ) FAIL_FAILED();
	time_t t_zero = tm1.tval;

	time_t t_next = t_zero; // becomes t_prev
	int x_next = scale.scn_of_val( t_next ); // becomes x_prev
	while( (t_next) < t_hi ) // LURK LAST
	{
		time_t t_prev = t_next;
		int    x_prev = x_next;
		t_next += tick_dt;
		x_next = scale.scn_of_val( t_next );

		if( t_prev < t_lo ) continue;
		if( t_prev > t_hi ) break;

		// vertical ticks
		draw_line( gc_black, x_prev, y_top, x_prev, y_btm );

	}
	// draw horiz line below day names // ODD +1
	// draw_line( gc_black, lft, y_btm, rgt, y_btm );
}

void axis_draw_xtime::draw_tod_line()
{
	if(! tod_tick1_height ) return;

	flt80 y_lo     = y_pos;
	flt80 y_hi_1 = y_lo + tod_tick1_height;
	flt80 y_hi_2 = y_lo + tod_tick2_height;
	flt80 y_hi_3 = y_lo + tod_tick3_height;
	flt80 y_hi_4 = y_lo + tod_tick4_height;

	tick_each( y_lo, y_hi_1, tod_tick1_dt );
	tick_each( y_lo, y_hi_2, tod_tick2_dt );
	tick_each( y_lo, y_hi_3, tod_tick3_dt );
	tick_each( y_lo, y_hi_4, tod_tick4_dt );
	draw_horiz_line( y_hi_1 );
	y_pos = y_hi_1;
}

void axis_draw_xtime::draw_dow_line()
{
	int y_row_top = y_pos;
	int y_baseline = y_pos + y_dow_baseline;
	int y_row_btm = y_pos + y_dow_btm;
	y_pos = y_row_btm;

	int t_lo = (time_t) scale.val_lo;
	int t_hi = (time_t) scale.val_hi;

	d_gc & gc_black = gc;

	int lft = scale.scn_lo;
	int rgt = scale.scn_hi - 1;

	text.goto_y_baseline( y_baseline ); // text is BELOW the 1 pix axis line
	switch( fmt_dow ) {
	case fmt_dow_Wednesday:
	case fmt_dow_Wed:
	case fmt_dow_W:
	break;
	case fmt_dow_ticks:
		// ignore mis-alignment over DST for week ticks
		tick_each( y_row_top, y_row_btm, 60*60*24*7 );
		draw_horiz_line( y_row_btm );
		return;
	break;
	case fmt_dow_none:
		return;
	}

	// start off with a day aligned time (LURK later)
	tm_parts tm1;
	tm1.local_from_time( t_lo );
	tm1.hour( 0 );
	tm1.min( 0 );
	tm1.sec( 0 );
 if(!	tm1.mktime_from_local() ) FAIL_FAILED();
	time_t t_zero = tm1.tval;
	tm1.local_from_time( t_zero );

	// draw each DAY on the X axis
	// wrong first time, next is first
	// strange 12 hour fixup -- BST ??
/*
	DST causes problems with graph alignment
	due to caller setting unaligned t0-t2
	Also if DST changes within era - the start is propagated
*/
	time_t t_next = t_zero; // becomes t_prev
	int x_next = scale.scn_of_val( t_next ); // becomes x_prev
	while( (t_next + 60*60*12) < t_hi )
	{
		time_t t_prev = t_next;
		int    x_prev = x_next;

		// get prev days name
		char buff[20];
		switch( fmt_dow ) {
		 case fmt_dow_Wednesday:
			sprintf( buff, "%s", tm1.str_Sunday() );
                 break;
		 case fmt_dow_Wed:
			sprintf( buff, "%s", tm1.str_Sun() );
                 break;
		 case fmt_dow_W:
			sprintf( buff, "%s", tm1.str_Sun() );
			if( tm1.is_weekend() ) {
				buff[0] = 0;
			}
			buff[1] = 0;
                 break;
		 case fmt_dow_ticks:
		 case fmt_dow_none:
		 default:
			buff[0] = 0;
		}

		// step to next day
		/*
			cannot step by 24 * 60 * 60
			because of the 25 hour day
			t_next += 60*60*24;
		*/
		t_next = tm1.step_to_00_of_next_day_local();
		x_next = scale.scn_of_val( t_next );

		if( fmt_dow != fmt_dow_none ) {
			mid_l1( x_prev, x_next, buff );
			// int xmid = (x_next + 0 + x_prev) / 2;
			// // text.goto_y_baseline( y_baseline );
			// text.draw_xmid_str( xmid, buff );
		}

		// vertical ticks
		draw_line( gc_black, x_prev, y_row_top, x_prev, y_row_btm );

	}
	// draw closing vertical tick // LURK << day
	draw_line( gc_black, x_next, y_row_top, x_next, y_row_btm );
if(0)	draw_line( gc_black, x_next, y_row_top, x_next, y_row_btm+10 );
	// draw horiz line below day names // ODD +1
	draw_horiz_line( y_row_btm );
}

void axis_draw_xtime::draw_horiz_line( int y )
{
	d_gc & gc_black = gc;
	draw_line( gc_black, scale.scn_lo, y, scale.scn_hi, y );
}

void axis_draw_xtime::draw_mon_line()
{
	int y_row_top = y_pos;
	int y_baseline = y_pos;
	int y_row_btm = y_pos;

	bool draw_text = false;

	switch( fmt_mon ) {
	 case fmt_mon_none:
		return;
	 case fmt_mon_ticks:
		draw_text = false;
	 break;
	 default:
		draw_text = true;
		y_pos += text.font_line_pre_gap + text.font_ascent;
		y_baseline = y_pos;
		y_pos += text.font_descent;
		y_row_btm = y_pos;
	}


	int t_lo = (time_t) scale.val_lo;
	int t_hi = (time_t) scale.val_hi;

	d_gc & gc_black = gc;

	int lft = scale.scn_lo;
	int rgt = scale.scn_hi - 1;

	text.goto_y_baseline( y_baseline ); // text is BELOW the 1 pix axis line

	// start off with a month aligned time
	tm_parts tm1;
	tm1.local_from_time( t_lo );
	tm1.dd( 1 );
	tm1.hour( 0 );
	tm1.min( 0 );
	tm1.sec( 0 );
 if(!	tm1.mktime_from_local() ) FAIL_FAILED();
	time_t t_next = tm1.tval;
	tm1.local_from_time( t_next );

	int x_next = scale.scn_of_val( t_next ); // becomes x_prev
	while( (t_next) <= t_hi )
	{
		time_t t_prev = t_next;
		int    x_prev = x_next;

		char buff[20];
		switch( fmt_mon ) {
		 case fmt_mon_January:
			sprintf( buff, "%s", tm1.str_January() );
                 break;
		 case fmt_mon_Jan:
			sprintf( buff, "%s", tm1.str_Jan() );
                 break;
		 case fmt_mon_J:
			sprintf( buff, "%s", tm1.str_Jan() );
			buff[1] = 0;
                 break;
		 case fmt_mon_ticks:
		 case fmt_mon_none:
		 default:
			buff[0] = 0;
		}

		// step
		t_next = tm1.step_to_first_of_next_month_local();
		x_next = scale.scn_of_val( t_next );

		if( fmt_mon != fmt_mon_none ) {
			mid_l1( x_prev, x_next, buff );
			// int xmid = (x_next + 0 + x_prev) / 2;
			// // text.goto_y_baseline( y_baseline );
			// text.draw_xmid_str( xmid, buff );
		}

		// vertical ticks
		if( (x_prev >= scale.scn_lo) && (x_prev <= scale.scn_hi ))
			draw_line( gc_black, x_prev, y_row_top, x_prev, y_row_btm );

	}
	// draw closing vertical tick // LURK << day
	if( ( x_next >= scale.scn_lo ) && ( x_next <= scale.scn_hi ))
		draw_line( gc_black, x_next, y_row_top, x_next, y_row_btm );

	// draw horiz line below day names // ODD +1
	draw_line( gc_black, lft, y_row_btm, rgt, y_row_btm );
}

void axis_draw_xtime::mid_l1( int x1, int x2,  char * str )
{
	int x_A;
	int x_B;

	mid_l_base( x1, x2, str, x_A, x_B );
	// save the area written
	text.draw_x_str( x_A, str );
}

void axis_draw_xtime::mid_l2( int x1, int x2,  char * str )
{
	int x_A;
	int x_B;

	mid_l_base( x1, x2, str, x_A, x_B );
	// avoid the saved written area
	text.draw_x_str( x_A, str );
}

void axis_draw_xtime::mid_l_base( int x1, int x2,  char * str, int & x_A, int & x_B )
{
	int x1b = x1;
	int x2b = x2;
	if( x1b < scale.scn_lo ) x1b = scale.scn_lo;
	if( x2b > scale.scn_hi ) x2b = scale.scn_hi;
	int w_avail = x2b - x1b;
	int w_str;
	while(1)
	{
		w_str = text.string_width( str );
		if( w_avail > w_str ) break;
		int len = strlen( str );
		if(!len) return;
		len--; // reduce by one
		// month names AND day names nicely reduce to 3
		if( len > 3 ) len = 3;
		str[ len ] = 0;
	}

	int x = (x1b + x2b - w_str) / 2;


	if(1) ; else // skip the following
	if( x1 != x1b )
	{
		x = x1b; // slam to left
	} else if( x2 != x2b )
	{
		x = x2b - w_str; // slam to right
	} else
		; // centralised

	x_A = x;
	x_B = x + w_str;
}

