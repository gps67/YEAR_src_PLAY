#include "rep_pdf_0.h"

#include "rep_pdf_Reports.h" // for single table
#include "dgb_fail.h"

namespace ACCTS {

//////// util class

/*!
	header str is multiline, calc its width and height max
*/
void Layout_Hdrs::layout_a_b_c( pdf_pen_text * pen, str0 str )
{
	buffer2 buff;
	uchar * s = str;
	height = 0;
	width = 0;
	flt80 h = pen->get_line_height_v();
	height += pen->get_descender_v(); // gap above
	while( *s )
	{
		buff.clear();
		while( uchar c = *s )
		{
			s++;
			if( c == '\r' ) continue;
			if( c == '\n' ) break;
			buff.put( c );
		}
		flt80 w = pen->calc_string_width( str0(buff) );
		height += h;
		if( width < w ) width = w;
	}
	width += pen->calc_string_width( " " ); // gap
}

/*!
	header str is multiline, draw it centralised in the box
*/
void Layout_Hdrs::draw_a_b_c(
	pdf_pen_text * pen,
	flt80 x_l,
	flt80 x_r,
	flt80 y_t,
	flt80 y_b,
	str0 str
)
{
	int lines=1;
	uchar * s = str;
	while( *s )
	{
		if( *s == '\n' ) lines++;
		s++;
	}
	if(!lines) return;
	flt80 h = pen->get_line_height_v();
//	flt80 h1 = - pen->get_ascender_v();
	flt80 h2 = + pen->get_descender_v();
	flt80 x1 = - pen->calc_string_width(" ") / 2.0;
	flt80 residue = (y_b - y_t) - (lines * h + h2);
	// +h is the leading gap + drop from ascender to baseline
	// but thats already in the residue ...
	flt80 y = y_t + residue/2.0 +h;
	flt80 x = x1 + (x_l+x_r)/2.0;
	// x1 adjustment should really be done by xy_mid ??
	buffer2 buff;
	height = 0;
	width = 0;
	s = str;
	while( *s )
	{
		buff.clear();
		while( uchar c = *s )
		{
			s++;
			if( c == '\r' ) continue;
			if( c == '\n' ) break;
			buff.put( c );
		}
		pen->xy_mid( x, y, str0(buff) );
		y += h;
	}
}



////////


/*!
	constructor 

	the instance is not reusable, but the draw_table() is out of the
	constructor to factor the class into functions
*/
rep_pdf_0::rep_pdf_0(
	pdf_ctxt & _pdf,
	data_set * _dset,
	str0 _filenamepdf
)
: pdf_base( _pdf )
, filenamepdf( _filenamepdf )
, dset( _dset )
// defaults
, tbl( NULL )
, spec_amnt( NULL )
, tree( new sum_tree_build( NULL )) // col_spec
// surely NULL comes from memory init
, s1_tree( NULL )
, s1_desc( NULL )
, s2_tree( NULL )
, s2_desc( NULL )
, ncol(0)
, layout_hdrs()
, show_hidden_cols( false )
, show_hidden_tbls( false )
{
	landscape = false;
	titles_height = 0;
	if(1) {
		e_print("# ODD # showing hidden cols\n");
		// MESSES UP subtotals alignment // show_hidden_cols = true;
		landscape = true;
	}

	/*
		the following was absent
		the first object was initialised to NUL --> 0.0
		the SECOND was initialised to NAN --> fail
	*/
	// because y_pos is assigned below - it isnt initialised to ZERO
	// e_print( "# INFO # rep_pdf_0::() # y_pos was %lf\n", y_pos );
	y_page_top = 0.1; // pen_normal->get_line_height_v() * 2 ;
	y_page_end = 0.97;
	y_page_min_break = 0.5 ;
	y_min_orphan = 0.2;
	y_pos = y_page_top;
}

/*!
	VTL callback on new page open, set y_pos = y_page_top
*/
void rep_pdf_0::on_page_top()
{
	y_pos = y_page_top;
}

/*!
	debug: cross out entire page (indicate landscape portrait area)
*/
void rep_pdf_0::x_out()
{
	return;
	pdf_pen_draw * pen = line_borders;
	if(!pen) return;
	float O = 0.01;
	float I = 0.99;
	pen->line( O,O, O,I );
	pen->line( O,I, I,I );
	pen->line( I,I, I,O );
	pen->line( I,O, O,O );
	pen->line( O,O, I,I );
	pen->line( O,I, I,O );
}

/*!
	Build the default list of columns for tbl
*/
void rep_pdf_0::build_col_list()
{
	col_list.clear();
	widths.clear();
	just_align.clear();

	ncol = tbl->col_specs.N(); // is_hidden reduces this

	col_list.get_space( ncol );
	widths.get_space( ncol );
	just_align.get_space( ncol );

	for( int j=0; j<ncol; j++ )
	{
		col_spec * spec = tbl->col_specs[j];
		if( !spec ) continue;
		if( spec->is_hidden )
			if( !show_hidden_cols )
				 continue;

		align_val align = spec -> get_align()
;
		col_list.append( spec );
		just_align.append( align );
		widths.append( 0.000001 ); // 0.01 is 1/100 of page width
	}
	ncol = col_list.N();
}

/*!
	widths[col] is the widest - include a measurement
*/
void rep_pdf_0::size_col_request( int vis_col, flt80 w )
{
	if( vis_col < 0 ) return; // invis
	if( widths[vis_col] < w ) widths[vis_col] = w;
}

/*!
	calc size of each col of this row
*/
void rep_pdf_0::size_row( row_inst * r )
{
#if 1
	// debugging
	// b ACCTS::rep_pdf_0::size_row
	// c
	str0 s1 = col_list[0]->get_str2_for_display(r)->str;
#endif
	pdf_pen_text * pen = pen_normal;
	for(int i=0; i< ncol; i++ )	// vis_col
	{
		obj_hold<str2> s2 = col_list[i]->get_str2_for_display(r);
		str0 s = s2->str;
		flt80 w = pen->calc_string_width( s );
#if 0
		// shows that LAVES only are being sized - not nodes (in Categorised)
		if(i==0) e_print("# row_size # %s # %5.3f ", (STR0) s, (float) w );
		if(i!=0) e_print(" # %s", (STR0) s );
#endif
		size_col_request( i, w );
	}
#if 0
	e_print("\n" );
#endif
}

/*!
	do all X layout - after calculating the required field widths

	small_titles are often wider than the field, so pad it on both sides

*/
void rep_pdf_0::set_x_left()
{
	x_left.clear();
	x_title_left.clear();
	x_title_right.clear();
	x_right.clear();
	y_horiz_lines.clear();
	x_vert_lines.clear();

	flt80 x = 0;
	x_vert_lines.append( x );
	for(int i=0; i< ncol; i++ )
	{
		x += x_gap_left;

		// widest data from ... size_row
		flt80 wd = widths[i];

		// width of small title
		str0 s = col_list[i]->get_name_hdr()->str;
		layout_hdrs.layout_a_b_c( pen_titles_small, s );
		if( titles_height < layout_hdrs.height ) titles_height = layout_hdrs.height;
		flt80 wt = layout_hdrs.width;

		flt80 xs = ( wt - wd );
		flt80 xtl = 0;
		flt80 xtr = 0;
		if( xs > 0 ) // wt title is wider than wd widest data
		{
			flt80 xs_left = xs / 2.0;
			flt80 xs_right = xs - xs_left;

			xtl = x;
			x += xs_left;
			x_left.append( x );
			x += wd;
			x_right.append( x );
			x += xs_right;
			xtr = x;
			widths[i] = wt; // unused
		} else {
			xs = - xs;
			flt80 xs_left = xs / 2.0;
			flt80 xs_right = xs - xs_left;

			x_left.append( x );
			x += xs_left;
			xtl = x;
			x += wt;
			xtr = x;
			x += xs_right;
			x_right.append( x );
			widths[i] = wd; // unused
		}

		x_title_left.append( xtl );
		x_title_right.append( xtr );

		x += x_gap_right;
		x_vert_lines.append( x );
	}
	x_margin_left = ( 1 - x ) / 2.0;
	x_side_right = x + x_margin_left;
	x_side_left = x_margin_left;
	add_each( x_left, x_margin_left );
	add_each( x_right, x_margin_left );
	add_each( x_title_left, x_margin_left );
	add_each( x_title_right, x_margin_left );
	add_each( x_vert_lines, x_margin_left );
}

/*!
	when centralising everything on the page, add 'adj' to each in array
*/
void rep_pdf_0::add_each( dyn_array<flt80> & ary, flt80 adj )
{
	int n = ary.N();
	for( int i=0; i<n; i++ )
	{
		ary[i] += adj;
	}
}

/*!
	when centralising everything on the page, sub 'adj' to each in array
*/
void rep_pdf_0::sub_each( dyn_array<flt80> & ary, flt80 adj )
{
	int n = ary.N();
	for( int i=0; i<n; i++ )
	{
		ary[i] -= adj;
	}
}

/*!
	add each table-row to tree, calc tree subtotals and size most things

	Theres a serious problem with interior nodes not being sized!

	Each LEAF row is added and sized, but the interior NODES arent.
	One answer is to tree walk, like draw_ does, along with its code
	to fetch and hande each item.

	One answer is to make 'draw_' take a two-pass-parameter,
	to not actually draw.

	One answer is to add a fake row with a key length long enough.
*/
void rep_pdf_0::build_tree()
{
	/*
		the input rows are in date-sorted order (original input seq)
		add each to the tree builder
	
		measure each cell width - presuming single font ?
	*/
	int nr = tbl->rows_list.N();
	for( int i=0; i<nr; i++ )
	{
		row_inst * row = tbl->rows_list[i];
		tree->add_one_row( row );
		size_row( row );
	}

	tree->add_all_subtotals();
	tree->sort_interior();

	size_subtotals( tree->root );
	size_subtotals( tree->null_branch );

}

/*!
	draw the column header titles (col_list) using pre-calculated layout
*/
void rep_pdf_0::draw_titles_small()
{
	flt80 y1 = y_pos;

	pdf_pen_text * pen = pen_titles_small;
	// add a gap similar to the one which APPEARS to be beneath, to the top
	y_pos += titles_height;
#if 0
	y_pos += pen->get_descender_v();
	y_pos += - pen->get_ascender_v(); // minus
	// flt80 y_baseline = y_pos;
	y_pos += pen->get_descender_v();
#endif
	flt80 y2 = y_pos;

	// all titles share a background colour
	fill_titles_small->fill( x_side_left, y1, x_side_right, y2 );

	// draw each title
	for( int i=0; i<ncol; i++ )
	{
		str0 s = col_list[i]->get_name_hdr()->str;
		flt80 x_l = x_title_left[i];
		flt80 x_r = x_title_right[i];
		layout_hdrs.draw_a_b_c( pen, x_l, x_r, y1, y2, s );
	}

	// NONE // y_pos += y_gap_below;
}

/*!
	draw the column header titles (col_list) using pre-calculated layout
	but only draw the columns which are summable, key, desc, as these
	are the interior tree ones.
*/
void rep_pdf_0::draw_tree_titles_small()
{
	flt80 y1 = y_pos;

	pdf_pen_text * pen = pen_titles_small;
	// add a gap similar to the one which APPEARS to be beneath, to the top
	y_pos += titles_height;
	flt80 y2 = y_pos;

	// all titles share a background colour
	// fill_titles_small->fill( x_side_left, y1, x_side_right, y2 );
	fill_totals->fill( x_side_left, y1, x_side_right, y2 );

	// draw each title
	for( int i=0; i<ncol; i++ )
	{
		str0 s;
		s = col_list[i]->get_name_hdr()->str;
		col_spec * spec = col_list[i];
		if( spec->is_hidden ) if(!show_hidden_cols) continue;
		if( !spec->is_summable )
		{
			if( tree->spec_is_tree_spec( spec )
			||  tree->spec_is_desc( spec ) )
				; // s is already OK
			else 
				s = "-";
/*
			if( spec == s1_tree ) 
			{
				// reuse same
				// s = tree->tree_spec->name->str;
			} else
			if( spec == s1_desc )
			{
				// reuse same
				// s = tree->tree_spec->name->str;
			} else
				s = "-";
				// continue;
*/
		}
		flt80 x_l = x_title_left[i];
		flt80 x_r = x_title_right[i];
		layout_hdrs.draw_a_b_c( pen, x_l, x_r, y1, y2, s );
	}

	// NONE // y_pos += y_gap_below;
}

void rep_pdf_0::mid_title_gap( int gap_lines )
{
	pdf_pen_text * pen = pen_titles;
	flt80 h = pen->get_line_height_v();

	y_pos += h * gap_lines;
}

/*!
	draw a title
*/
void rep_pdf_0::mid_title( str0 title, int gap_pre, int gap_post )
{
	pdf_pen_text * pen = pen_titles;
	flt80 h = pen->get_line_height_v();

	y_pos += h * gap_pre;
	pen->xy_mid( 0.5, y_pos, title );
	y_pos += h;
	y_pos += h * gap_post;
}

/*!
	The internal tree has locations of locations (not locations of leaves).
	Draw anything that is_summable (and is_visible) in the same column
	as the leaf.
*/
void rep_pdf_0::draw_row_node( tree_node * _locn )
{
	sum_tree_node * locn = (sum_tree_node *) _locn;

	// The ROOT node should not appear in a grid 
	if( !locn->group )
		return;

	pdf_pen_text * pen = pen_normal;

	// gap between line and text top
	y_pos += y_gap_above;
	y_pos += - pen->get_ascender_v(); // minus
	amnt_t val;
	for( int i=0; i<ncol; i++ )
	{
		col_spec * spec = col_list[i];
		str0 s = "";
		if( spec->is_summable && tree->get_total( locn, spec, val ) )
		{
			s = (STR0) val.get_nocurr_str();
		}

		else if( spec == s1_tree ) 
		{
			if( s2_tree && locn->group )
			s = s2_tree->get_str2_for_display( locn->group )->str;
		} else if( spec == s1_desc )
		{
			if( s2_desc && locn->group )
			s = s2_desc->get_str2_for_display( locn->group )->str;
		} else
		{
			// s = "-";
			continue;
		}

		switch( just_align[i].val ) {
		 case align_none:
		 case align_left:
			pen->xy_left( x_left[i], y_pos, s );
		 break;
		 case align_right:
			pen->xy_right( x_right[i], y_pos, s );
		 break;
		 case align_center: {
			flt80 x = ( x_right[i] + x_left[i] ) / 2.0;
			pen->xy_mid( x, y_pos, s );
		 } break;
		}

	}
	// e_print("\n");
	y_pos += pen->get_descender_v();
	y_pos += y_gap_below;
}

/*!
	see size_row
*/
void rep_pdf_0::draw_row( row_inst * row )
{

	pdf_pen_text * pen = pen_normal;

	// draw N2 rows with background fill (sum values already placed)
	// SEE: report2.cxx  row->row_flags |= ROW_show_sums;
	if( row->row_flags & ROW_show_sums )
	{
		pdf_pen_draw * draw = fill_totals;
		flt80 y1 = y_pos;
		flt80 y2 = y_pos // copy following code (not 2 pass)
		+ y_gap_above
		- pen->get_ascender_v()
		+ pen->get_descender_v()
		+ y_gap_below
		;
		flt80 x1 = x_side_left;
		flt80 x2 = x_side_right;
		draw->fill( x1, y1, x2, y2 );
	}

	// gap between line and text top
	y_pos += y_gap_above;
	y_pos += - pen->get_ascender_v(); // minus
	for( int i=0; i<ncol; i++ )
	{
		col_spec * spec = col_list[i];
		obj_hold<str2> s = spec->get_str2_for_display( row );
		switch( just_align[i].val ) {
		 case align_none:
		 case align_left:
			pen->xy_left( x_left[i], y_pos, s->str );
		 break;
		 case align_right:
			pen->xy_right( x_right[i], y_pos, s->str );
		 break;
		 case align_center: {
			flt80 x = ( x_right[i] + x_left[i] ) / 2.0;
			pen->xy_mid( x, y_pos, s->str );
		 } break;
		}
	}
	// e_print("\n");
	y_pos += pen->get_descender_v();
	y_pos += y_gap_below;
}

/*!
*/
void rep_pdf_0::do_page_break()
{
	next_page();
	x_out();
}


/*!
	here are the fonts and colours
*/
void rep_pdf_0::create_pens()
{
	line_borders       = new pdf_pen_draw( this );
	fill_titles        = new pdf_pen_draw( this );
	fill_titles_small  = new pdf_pen_draw( this );
	fill_totals        = new pdf_pen_draw( this );

	pen_titles       = new pdf_pen_text( this );
	pen_normal       = new pdf_pen_text( this );
	pen_subtots      = new pdf_pen_text( this );
	pen_titles_small = new pdf_pen_text( this );

	int pt_24 = 20;
	int pt_12 = 10; // cashbook report is wide
	int pt_8 = 8;

 if(1) {
	pt_24 = 16;
	pt_12 =  9; // cashbook report is wide
	pt_8 = 7;
 }

	pen_titles ->       init_font( "Times-Bold",     pt_24 );
	pen_normal ->       init_font( "Helvetica",      pt_12 );
	pen_subtots ->      init_font( "Helvetica-Bold", pt_8 );
	pen_titles_small -> init_font( "Helvetica",      pt_8 );

// these could be shared -- its possible ;-)
	pen_titles       -> init_line_rgb( 0.1, 0.0, 0.0 );
	pen_titles       -> init_fill_rgb( 0.1, 0.0, 0.0 );
	pen_normal       -> init_line_rgb( 0.0, 0.1, 0.0 );
	pen_normal       -> init_fill_rgb( 0.0, 0.1, 0.0 ); // BUG needed to reset
	pen_titles_small -> init_line_rgb( 0.0, 0.0, 0.1 );
	pen_titles_small -> init_fill_rgb( 0.0, 0.0, 0.1 ); // BUG (same)
	pen_subtots      -> init_line_rgb( 0.1, 0.1, 0.1 );
	pen_subtots      -> init_fill_rgb( 0.1, 0.1, 0.1 );

	line_borders     -> init_fill_rgb( 0.0, 0.0, 0.0 );
	fill_titles      -> init_fill_rgb( 0.8, 0.8, 1.0 );
	fill_titles_small-> init_fill_rgb( 0.9, 0.9, 1.0 );
	fill_totals      -> init_fill_rgb( 0.6, 0.8, 0.6 );

	// change of mind

	pen_subtots = pen_normal;
}

/*!
	The group title is a single line that introduces a grid 

	This also shows the parent group keys and selected spec_amnt total
*/
void rep_pdf_0::draw_group_title( tree_node * node )
{
	pdf_pen_text * pen = pen_titles;
	flt80 x = x_left[0];
	y_pos += y_gap_above;
	y_pos -= pen->get_ascender_v(); // MINUS
	flt80 y_baseline = y_pos;
	y_pos += pen->get_descender_v();

	buffer2 buff;
	if( full_key_path )
	{

		str0 gap = "";
		for( int d = ((sum_tree_node*)node)->depth; d>0; d-- )
		{
			buff.put( gap );
			gap = "/";
			tree_node * n = node;
			for( int i=0; i<d; i++ )
			{
				n = n->parent;
			}
			// sum_tree_node * n2 = (sum_tree_node *) n;
			buff.put( tree->get_key( n ) );
		}
		buff.put( gap );
	}
	buff.put( tree->get_key( node ) );
	buff.put( "   " );
	buff.put( tree->get_desc( node ) );
	pen->xy_left( x, y_baseline, str0(buff) );

	amnt_t sum;
	if( spec_amnt && tree->get_total( node, spec_amnt, sum ))
	{
	} else {
		tree->get_total_of_visible_row( node, sum );
	}
	if( sum.is_non_zero() )
	{
		buff.clear();
		buff.print( "%s", (STR0) sum.get_curr_str() );
		pen->xy_right( x_side_right, y_baseline, str0(buff) );
	}
}

/*!
	All tree_nodes are sum_tree_nodes, but casts are a hassle
*/
void rep_pdf_0::size_subtotals( tree_node * node )
{
	size_subtotals( (sum_tree_node *) node );
}

/*!
	Calc the width of each subtotal (already calculated)
*/
void rep_pdf_0::size_subtotals( sum_tree_node * node )
{
	if(!node ) return;
	int n = tree->subtotal_specs.N();
	if(!n) return;
	// what about y_pos += height of subtots if absent ?

	pdf_pen_text * pen = pen_subtots;
	flt80 w_min = pen->calc_string_width( "0" );
	for(int i=0; i<n; i++ )
	{
		subtot_spec & sub = tree->subtotal_specs[i];
		flt80 w = pen->calc_string_width( (STR0) node->subtotal[i].get_nocurr_str() );
		if( w < w_min ) w = w_min;
		size_col_request( sub.vis_col_pos, w );
		// sub.sum.set_zero();
	}
}

/*!
	subtotals appear beneath the group grid
*/
void rep_pdf_0::draw_subtotals( sum_tree_node * node )
{
	int n = tree->subtotal_specs.N();
	if(!n) return;

	pdf_pen_text * pen = pen_subtots;
	// pen = pen_normal; // cols not wide enough for extra digit
	y_pos += pen->get_descender_v(); // extra gap
	y_pos += pen->get_descender_v(); // extra gap
	y_pos -= pen->get_ascender_v(); // baseline
	flt80 y_baseline = y_pos;
	y_pos += pen->get_descender_v(); // next line // no extra gap

	for(int i=0; i<n; i++ )
	{
		subtot_spec & sub = tree->subtotal_specs[i];
		if( sub.vis_col_pos < 0 ) continue;
		flt80 x = x_right[ sub.vis_col_pos ];
		str0 str = node->subtotal[i].get_nocurr_str();
		pen->xy_right( x, y_baseline, (STR0) str );
		if(0)
		{
			e_print("subtot %d %d %f %s %s\n",
				i,
				sub.vis_col_pos,
				(float)x,
				(STR0) str,
				(STR0) sub.spec->name->str
			);
		}
	}
}

void rep_pdf_0::draw_grid_lines()
{
	// horizontal line end points
	flt80 x0 = x_vert_lines[0];
	flt80 x2 = x_vert_lines[ x_vert_lines.N()-1 ];

	flt80 y0 = y_horiz_lines[0];
	flt80 y2 = y_horiz_lines[ y_horiz_lines.N()-1 ];

	// lots of horizontal lines
	int nlines = y_horiz_lines.N();
	for( int i=0; i<nlines; i++ )
	{
		flt80 y = y_horiz_lines[i];
		line_borders->line( x0, y, x2, y);
	}

	// vertical lines
	nlines = x_vert_lines.N();
	for( int i=0; i<nlines; i++ )
	{
		flt80 x = x_vert_lines[i];
		line_borders->line( x, y0, x, y2 );
	}
}


/*!
	draw a node which isnt leaves but nodes
*/
void rep_pdf_0::draw_tree_node( sum_tree_node * node )
{
	int nch = node->children_locn.N();
	if(!nch) return;

	if( node == tree->root.get_ptr() )
	{
		if( nch == 1 ) return; // skip single root repeat
		if( nch == 2 ) return; // BIG BUG SOMEWHERE false fix
	}

	// not quite regular //if( nch == 1 ) return; // already have it

	flt80 expected_height = 0 
	+ pen_titles->get_line_height_v() // group
	+ titles_height
	+ (nch * y_normal_line_height)
	;

	// THIS ONLY WORKS FOR A SINGLE BREAK - NOT A MULTI PAGE LAYOUT!
	flt80 y_break = y_page_end;
	if( ( y_pos + expected_height ) > y_break ) {
		if( ( y_pos + y_min_orphan ) > y_break ) {
			do_page_break();
		} else
		if( ( expected_height < y_break ) // will fit on next page
		 && ( y_pos >= y_page_min_break) ) // and some on this page
			do_page_break();
		else {
			flt80 y_next_page = 
				y_pos + expected_height - y_break;
			if( y_next_page < y_min_orphan ) 
			{
				y_next_page = expected_height / 2.0 ;
				y_break = y_pos + y_next_page;
				if( y_break > y_page_end )
					y_break = y_page_end;
				// new y_break will cause early split
				
			}
			// else leave it
		}
	}

	draw_group_title( node );
	y_horiz_lines.clear();

	// The grid has column titles, between 2 lines
	y_horiz_lines.append( y_pos ); // horiz line beneath each row
	draw_tree_titles_small();
	y_horiz_lines.append( y_pos ); // horiz line beneath each row

	// lots of rows and lines (child nodes)
	for( int ch=0; ch<nch; ch++ )
	{
		if( ( y_pos + y_normal_line_height ) > y_break )
		{
			draw_grid_lines();
			y_break = y_page_end;
			do_page_break();
			draw_group_title( node );
			y_horiz_lines.clear();
			// no lines above headers - show the break
			// y_horiz_lines.append( y_pos );
			draw_titles_small();
			y_horiz_lines.append( y_pos );
		}
		draw_row_node( node->children_locn[ch] );
		y_horiz_lines.append( y_pos ); // horiz line beneath each row
	}

	draw_grid_lines();

	// subtitles appear beneath the boxed grid
	draw_subtotals( node );

	// gap beneath node
	y_pos += y_gap_between_nodes;
}

/*!
	draw one line of the summary sections (when not draw_tree_node)
*/
void rep_pdf_0::draw_summary_line( sum_tree_node * node )
{
	if(!node) return;
	if(!node->group ) return;

	obj_hold<str2> s;
	flt80 x = x_side_left;
	buffer2 buff;
	for( int i=0; i<node->depth; i++ )
	{
		buff.put( "|   " );
	}
	buff.put( "+-----    " );
	
	pdf_pen_text * pen = pen_normal; // pen_titles;
	flt80 dent = pen->calc_string_width(" £ -1,234.56 ");
	flt80 y_baseline = 0; // overwritten
	if( node->group )
	{
		pen = pen_normal;
		s = tree->get_key_desc( node );
		x = x_left[0] * 1.1;
	} else {
		pen = pen_titles;
		s = new str2( "Tree" );
		x = x_side_left;
	}
	y_pos += y_gap_above;
	y_pos -= pen->get_ascender_v(); // MINUS
	y_baseline = y_pos;
	y_pos += pen->get_descender_v();

	pen->xy_left( x, y_baseline, buff );
	x += pen->calc_string_width( buff );
	pen->xy_left( x, y_baseline, s->str );

	if( ! node->group )
	{
		return;
	}

	amnt_t sum;
	if( spec_amnt && tree->get_total( node, spec_amnt, sum ))
	{
		x = x_side_left + 0.5 + dent*(node->depth-1);
		buffer2 buf2;
		buf2.print( "%s", (STR0) sum.get_curr_str() );
		pen->xy_right( x, y_baseline, str0(buf2) );
	}
}

/*!
	draw a heading, a grid of rows and a subtotal row
*/
void rep_pdf_0::draw_node( sum_tree_node * node )
{
	int nch = node->children_leaf.N();
	// e_print("-NODE- %d \n", nch );
	if(!nch) return;

	flt80 expected_height = 0 
	+ pen_titles->get_line_height_v() // group
	+ titles_height
	+ (nch * y_normal_line_height)
	;

	// THIS ONLY WORKS FOR A SINGLE BREAK - NOT A MULTI PAGE LAYOUT!
	flt80 y_break = y_page_end;
	if( ( y_pos + expected_height ) > y_break ) {
		if( ( y_pos + y_min_orphan ) > y_break ) {
			do_page_break();
		} else
		if( ( expected_height < y_break ) // will fit on next page
		 && ( y_pos >= y_page_min_break) ) // and some on this page
			do_page_break();
		else {
			flt80 y_next_page = 
				y_pos + expected_height - y_break;
			if( y_next_page < y_min_orphan ) 
			{
				y_next_page = expected_height / 2.0 ;
				y_break = y_pos + y_next_page;
				if( y_break > y_page_end )
					y_break = y_page_end;
				// new y_break will cause early split
				
			}
			// else leave it
		}
	}

	draw_group_title( node );
	y_horiz_lines.clear();

	// The grid has column titles, between 2 lines
	y_horiz_lines.append( y_pos ); // horiz line beneath each row
	draw_titles_small();
	y_horiz_lines.append( y_pos ); // horiz line beneath each row

	// lots of rows and lines
	for( int ch=0; ch<nch; ch++ )
	{
		if( ( y_pos + y_normal_line_height ) > y_break )
		{
			draw_grid_lines();
			y_break = y_page_end;
			do_page_break();
			draw_group_title( node );
			y_horiz_lines.clear();
			// no lines above headers - show the break
			// y_horiz_lines.append( y_pos );
			draw_titles_small();
			y_horiz_lines.append( y_pos );
		}
		draw_row( node->children_leaf[ch] );
		y_horiz_lines.append( y_pos ); // horiz line beneath each row
	}

	draw_grid_lines();

	// subtitles appear beneath the boxed grid
	draw_subtotals( node );

	// gap beneath node
	y_pos += y_gap_between_nodes;

}


/*!
	open the PDF file with a title, creator
*/
bool rep_pdf_0::open_up( str0 _filenamepdf, str0 title, bool _landscape )
{
	landscape = _landscape;
	filenamepdf = _filenamepdf;
	str0 author = "Information-Casade";
	str0 creator = "libs_apps";
	open_file(
		(STR0) filenamepdf,
		(STR0) title,
		(STR0) author,
		(STR0) creator
	); // THROWS
	begin_page();
	x_out();

	create_pens();
	return true;
}

/*!
	almost generic - print any table, needs more flag options
*/
bool rep_pdf_0::draw_table(
	str0 tablename,
	str0 treecol,
	str0 totcol,
	bool _landscape,
	bool _full_key_path,
	int interior_depth,
	bool interleave,
	bool show_leaves,
	str0 sum_tablename
)
{
	// e_print("# OK # draw_table( %s ) START \n", (STR0) tablename );

	tbl = dset->get_table( tablename );
	if(!tbl)
	{
		return FALSE_dgb_fail(" rep_pdf_0::draw_table() NULL tbl\n");
	}

	col_spec * col_spec_tree_1;
	col_spec_tree_1 = tbl->get_col_spec( treecol );
	tree->re_init_2( col_spec_tree_1 ); // late_init - constructor


	table * t1 = tbl;
	table * t2 = tree->tree_table;

	s1_tree = t1->col_spec_tree;
	s1_desc = t1->desc_spec;
	s2_tree = s1_tree; // LURK
	s2_desc = s1_desc; // LURK
	if(t2) s2_tree = t2->key_spec; // t2->col_spec_tree;
	if(t2) s2_desc = t2->desc_spec;

	spec_amnt = NULL;
	col_spec * spec_totcol = tbl->get_col_spec( totcol );
	if( spec_totcol )
	{
		if( spec_totcol->get_field_type() == T_amnt )
			spec_amnt = ( col_spec_amnt * ) spec_totcol;
	}

	now_need_landscape( _landscape );
	// begin_page();
	x_out();

	// ZERO means TOP only // if( interior_depth == 0 ) interior_depth = 1000;
	if( interior_depth == -1 ) interior_depth = 1000;
	full_key_path = _full_key_path;

	flt80 x_gap = pen_normal->calc_string_width(" ");

	y_page_top = pen_normal->get_line_height_v() * 2 ;
	y_page_end = 0.97;
	y_page_min_break = 0.5 ;
	y_min_orphan = 0.2;

	x_margin_left = x_gap * 5;

	// y_pos = y_page_top;

	x_gap_left = (x_gap * 3) / 4;
	x_gap_right = (x_gap * 1) / 4;
	flt80 y_gap = pen_normal->get_line_height_v() / 4;
	y_gap = 0;
	y_gap_above = y_gap * 3 / 4;
	y_gap_below = y_gap * 1 / 4;
	// Digits are like caps, so adding descender over the top looks right
	y_gap_above = pen_normal->get_descender_v();
	y_gap_below = 0;
	y_gap_between_nodes = pen_normal->get_line_height_v(); // leave a gap
	y_normal_line_height
	= pen_normal->get_line_height_v()
	+ y_gap_above
	+ y_gap_below;

	// tree->add_all_rows();
	build_col_list();
	build_tree(); // calls size_row() which requires AFTER begin_page_a4_landscape
	set_x_left();

	pdf_pen_text * pen = pen_titles;
	y_pos -= pen->get_ascender_v();
	pen->xy_left( x_side_left, y_pos, tbl->name->str );
	y_pos += pen->get_descender_v();

	obj_list<tree_node> nodelist;
	/*
		The root node holds a list of top-level nodes,
		but somehow the vat year has TWO at top!
	*/
	if( tree->root->children_locn.N() > 1 )
		nodelist.append( tree->root );
	bool depthwise = false;
	if( interleave ) depthwise = true;
	tree->root->flat_list_locn( nodelist, depthwise ); // LURK NULL ROOT PATH

	// the summary block // no calcs needed as spec_amnt must be summable
	// the summary block - a list of summary lines

	if(!( show_leaves || interleave ) )
	{
		int nn = nodelist.N();
		for( int i=0; i<nn; i++ )
		{
			tree_node * n1 = nodelist[i];
			sum_tree_node * n2 = ( sum_tree_node * ) n1;
			if( n2->depth < interior_depth )
			{
				// draw_summary_line( n2 );
				draw_tree_node( n2 );
			}
		}
		// gap
		y_pos += pen_titles->get_line_height_v();
	}

	// lots of blocks
	if( show_leaves || interleave )
	{
		int nn = nodelist.N();
		for( int i=0; i<nn; i++ )
		{
			tree_node * n1 = nodelist[i];
			sum_tree_node * n2 = ( sum_tree_node * ) n1;
			if( interleave && n2->children_locn.N()  && ( n2->depth < interior_depth ))
				draw_tree_node( n2 );
			if( show_leaves && n2->children_leaf.N() )
				draw_node( n2 );
		}
	}

	// end_page();
	// THIS MUST HAPPEN AFTER build_tree();
	// // //
	if( sum_tablename ) {	//  "CAT_sum"
		sum_table(
			tbl,
			spec_totcol,
			sum_tablename
		);
		if(0) // drawn elsewhere
		draw_table(
			sum_tablename,
			"key",	// str0 treecol,
			totcol,
			_landscape,
	// these default - not necc parents
			full_key_path,
			interior_depth,
			interleave,
			show_leaves,
			(STR0) NULL
		);
	}


	// e_print("# OK # draw_table( %s ) DONE \n", (STR0) tablename );
	return false;
}

bool rep_pdf_0::sum_table(
//		str0 tablename,
		table * tbl,
		col_spec * spec_totcol,
//		str0 treecol,
//		bool _landscape,
//		bool full_key_path = true, // looks shabby for era keys
//		int interior_depth = -1,
//		bool interleave = true,
//		bool show_leaves = true,
		str0 sum_tablename
)
{

 // gdb_invoke( true );

	// THIS MUST HAPPEN AFTER build_tree();

	int nc = tree->subtotal_specs.N();

	obj_hold<table> sum_tbl = tbl->dset->find_add_fake_table( sum_tablename );
	if( !sum_tbl ) return FALSE_dgb_fail( "failed to create table" );

	/*
		key is (probably) a linked row
	*/
	col_spec * spec1_key = tbl -> key_spec;
	if(!spec1_key) return FALSE_dgb_fail( "spec1_key NULL" );
	col_spec * spec2_key = spec1_key -> col_spec_clone( sum_tbl ); 
	sum_tbl -> add_col( spec2_key ); // tbl is needed twice

	/*
		each column that was summable,
		becomes a colmumn in the new table
		(of type amnt!)
	*/
	obj_list<col_spec_amnt> specs2;
	for( int c= 0; c < nc; c++ )
	{
		col_spec * spec1 = tree->subtotal_specs[c].spec;
		col_spec * spec2 = sum_tbl->add_type_col( "amount", (str0) spec1->name );
		col_spec_amnt * spec2a = (col_spec_amnt *) spec2;
		// this makes non/vat duplication a bit clearer
		if(1) spec2a -> show_zero_as_empty = true;

		specs2 . append( spec2a );
	}

	col_spec * desc1 = tbl -> desc_spec;
	col_spec * desc2 = NULL;
	if( desc1 ) {
		desc2 = desc1 -> col_spec_clone( sum_tbl ); 
		sum_tbl -> add_col( desc2 ); // 
	}

	/*
		dont bother with leaves, only do (linked) locn's
		print key and sum
	*/
	obj_list<tree_node> list1;
	bool depthwise = false;
	tree->flat_list_locn( list1, depthwise );
	int n = list1.N();
	int totcol_spec_idx = tree->find_idx_spec( spec_totcol );
	// dont iterate over all summable columns
	// pick the non-vat-turnover (totcol)
	int t = totcol_spec_idx;
	if( t >= 0 )
	for( int i=0; i< n; i++ )
	{
		tree_node * locn1 = list1[i];
		sum_tree_node * locn = (sum_tree_node *) locn1;
		if(!locn) {
			e_print("NULL locn1\n");
			continue;
		}
		str2 * key = tree->get_key( locn );
		if(!key) {
			e_print("NULL key\n");
			continue;
		}
		// e_print("key: %s\n", (STR0) (str0) key );
		row_inst * r2 = sum_tbl -> find_row_add_fake( key);
		for( int j=0; j<nc; j++ )
		{
			amnt_t & amnt = locn->subtotal[j];
			specs2[j]->set_amnt( r2, amnt );
		}
		if( desc2 ) {
			str2 * desc = tree->get_desc( locn );
			desc2 -> set_obj( r2, desc );
		}
	}
	return true;
}


}; // NAMESPACE
