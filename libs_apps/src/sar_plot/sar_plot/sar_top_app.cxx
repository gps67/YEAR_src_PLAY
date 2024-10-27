#include "sar_top_app.h"
// #include "top.h"
#include "w_menu1.h"
#include "c_tree_tbl_two.h"
#include "w_vpaned.h"
#include "w_table.h"
#include "w_entry.h"
// #include "nb_page_tbl.h"
// #include "note_book.h"
// #include "tbl_col_spec.h"
#include "w_dialog_msg.h"

#include "time_calc_zone.h"
#include "sar_set_buttons.h"

#include "dset_api_MySQL.h"
#include "dset_api_postgres.h"

/*

void C_menu_help_about( GtkWidget * abtn, sar_top_app * app )
{
	w_dialog::msg_failed(
		app->main_window,
		"About sar_plot",
		"\n"
		"sar_plot\n"
		"\n"
		"This is Free Software,\n"
		"Licensed under the GPL and LGPL \n"
		"http://www.information-cascade.co.uk\n"
		"http://freshmeat.net\n"
		"http://www.gnu.org/licenses/lgpl.html\n"
	);
}

*/

void sar_top_app::pre_fast_exit()
{
	e_print("pre_fast_exit();\n");
	if( dset) dset->disconnect();
}


sar_top_app::~sar_top_app()
{
	// main_window->destroy(); // HIDE 
	gtk_widget_hide_all( main_window->w ); // HIDE 
	e_print("~sar_top_app();\n");

	pre_fast_exit();

	// hbox1 = NULL;
	// layout is retained in hbox ...
	layout->dismantle();
	e_print("~sar_top_app(); SUB-fields\n");
}

void sar_top_app::file_open( str2 * _filename )
{
	/*
		should only open a pdb file
		but maybe the file is an SQL connecton spec or hardcoded
	*/
	switch(1) {
	case 1:
#warning "This just broke, editing something else"
	//	dset = new dset_api_pdb( _filename->sget() );
	break;
#ifdef WITH_MYSQL
	case 2:
		dset = new dset_api_MySQL();
	break;
#else
#warning MYSQL not enabled
#endif
#ifdef WITH_PGSQL
	case 3:
		dset = new dset_api_postgres();
#else
#warning PGSQL not enabled
#endif
	}

	layout->set_line_tree_from_dset( dset );
}

bool sar_top_app::fill_menu_bar()
{
	STEP("sar_top_spp");
#warning RENAME VAR menu_bar
	obj_hold<w_menu> m_file = new w_menu( "File" );
	menu_bar->append( m_file );
	// similar but condensed
	obj_hold<w_menu> m_edit = menu_bar->mk_menu( "Edit" );
	obj_hold<w_menu> m_help = menu_bar->mk_menu_right( "Help" );

	obj_hold<w_menu_item> item = new w_menu_item(
		"Open ...",
		(SignalFunc) C_menu_file_open,
		(obj_ref *) this
	);
	m_file->append( item );

	m_file->mk_item(
		"Save",
		(SignalFunc) C_menu_file_save,
		(obj_ref *) this
	);

	m_file->mk_item(
		"Save As ...",
		(SignalFunc) C_menu_file_save_as,
		(obj_ref *) this
	);

	m_file->mk_item(
		"Exit",
		(SignalFunc) gtk_main_quit,
		(obj_ref *) NULL
	);

	m_help->mk_item(
		"About",
		(SignalFunc) C_menu_help_about,
		(obj_ref *) this
	);

	obj_hold<w_menu_item> gdb_item = new w_menu_item(
		(char *) "Enable gdb_break_point",
		(SignalFunc) C_enable_gdb_break_point,
		(obj_ref *) this
	);
	m_file->append( gdb_item );

//	obj_hold<menu_item> mi_copy = m_edit->mk_item( "copy" );
//	obj_hold<menu_item> mi_paste = m_edit->mk_item( "Paste" );

	return true;
}

// zero size causes no call to set usize

#if 1
// the point is - bench_top_app is producing an error message
// we can make the same message by if 0 dropping this CODE
//
// libs_apps/src/apps/main/sar_plot.cxx:34:
//  undefined reference to
//  `sar_top_app::sar_top_app(APPS::TOPAPP_Holder_gtk*, char const*)'
//
// collect2: error: ld returned 1 exit status
//
// basically says this function was not provided or defined
// but it was referenced, eg FILE_LINE sar_plot.cxx:34:
// expected prototype is 
//
//  `sar_top_app::sar_top_app(
//	APPS::TOPAPP_Holder_gtk*,
//	char const*
//  )'
//
// that confirms that "APPS::" is AUTO_NEST LOCN RESOLVED_TYPENAME
// it says it was resolved, and got "APPS::" from "{ using namespace APPS; }"
//
// 	FUTURE CODE STYEL USAGE
// 	FUTURE CODE STYEL USAGE
//
//	using namespace ZONE; // let G++ resolve name in namespace // HOW
//	// then avoid using namespaces wherever possible // always using DECL
//
// 	FUTURE CODE STYEL USAGE
// 	FUTURE CODE STYEL USAGE
//
// the SOURCE CODE prototype in the .cxx code_provider
// CXX_HEADER_FUNCTION_with_BODY
// actual prototype is

sar_top_app::sar_top_app(
	TOPAPP_Holder_gtk * holder,
	const char * first_file
)
: TOPAPP_Task_Window_Major( holder, "Sar Plot", 0, 0, first_file, false )
{
	use_standard_layout();

	t_lo = 1;
	t_hi = t_lo + 28 * 24 * 60 * 60;

	//! the row of columns of buttons (its an hbox)
	obj_hold<w_hbox> hbox1;
	obj_hold<w_vbox> curr_vals;

 // ROW1 (top half of window - lots of buttons sliders ... )

	hbox1 = new w_hbox();
//	app_area->pack( hbox1 );

 // BUTTONS - line selectors
	layout = new sar_top_layout( this );
	if(0)
		hbox1->pack_asis( layout );
	else
		hbox1->pack( layout );

  // INFO Labels
	curr_vals = new w_vbox();
	hbox1->pack( curr_vals );

	curr_date  = new W_LABELS( "curr_date" );
	curr_date2 = new W_LABELS( "curr_date2" );
	curr_date3 = new W_LABELS( "curr_date3" );
	curr_hhmm  = new W_LABELS( "curr_hhmm" );
	curr_yval  = new W_LABELS( "curr_yval" );

	curr_vals->pack_asis( curr_date );
	curr_vals->pack_asis( curr_date2 );
	curr_vals->pack_asis( curr_date3 );
	curr_vals->pack_asis( curr_hhmm );
	curr_vals->pack_asis( curr_yval );

  // TIME SPAN buttons

	float daylist[] = { 0.125, 0.5, 1, 2, 3, 4, 5, 6, 7, 14, 21, 28, 90, 365 };
	int daylist_N = sizeof( daylist ) / sizeof( daylist[0] );

	w_table * spans = new w_table( 3, daylist_N );
	hbox1->pack( spans );

	for( int i=0; i< daylist_N; i++ )
	{
		float d = daylist[i];
		int s = (int)(d * 60 * 60 * 24);
		char buff[20];
		if( d >= 1 )
			sprintf( buff, "%1.0f day", d );
		else
			sprintf( buff, "%1.1f hour", d*24 );
		
		sar_adj_time_pos * lft = new sar_adj_time_pos( this, "-", -s );
		sar_set_time_span * mid = new sar_set_time_span( this, buff, s );
		sar_adj_time_pos * rgt = new sar_adj_time_pos( this, "+", +s );
		spans->add( 1, i, lft );
		spans->add( 2, i, mid );
		spans->add( 3, i, rgt );
	}

/*
	w_vbox * span_vals = new w_vbox();
	hbox1->pack( span_vals );
	span_vals->pack_asis( new sar_set_time_span( this, "1 day", 1 ) );
	span_vals->pack_asis( new sar_set_time_span( this, "2 day", 2 ) );
	span_vals->pack_asis( new sar_set_time_span( this, "3 day", 3 ) );
	span_vals->pack_asis( new sar_set_time_span( this, "4 day", 4 ) );
	span_vals->pack_asis( new sar_set_time_span( this, "5 day", 5 ) );
	span_vals->pack_asis( new sar_set_time_span( this, "6 day", 6 ) );
	span_vals->pack_asis( new sar_set_time_span( this, "7 day", 7 ) );
	span_vals->pack_asis( new sar_set_time_span( this, "14 day", 14 ) );
	span_vals->pack_asis( new sar_set_time_span( this, "21 day", 21 ) );
	span_vals->pack_asis( new sar_set_time_span( this, "28 day", 28 ) );
	span_vals->pack_asis( new sar_set_time_span( this, "90 day", 90 ) );
	span_vals->pack_asis( new sar_set_time_span( this, "365 day", 365 ) );
*/

 // GRAPH
	graph_2 = new w_graph3( this );

	obj_hold<w_vpaned> pair = new w_vpaned( hbox1, graph_2, false );
	app_area->pack_big( pair );

 // YEAR-MAP
	years_map = new w_segs_map( this );
	app_area->pack_asis( years_map );

  // OPEN-FILE
	if( first_file )
	{
		filename = new str2( first_file );
		file_open( filename );
		// so why does it still flicker
		main_window->show();	// main_window has no pack to call show()
	} else {
		filename = new str2( "" );
		main_window->show();	// main_window has no pack to call show()
		C_menu_file_open( NULL, this );
		// C_menu_file_save_as( NULL, this ); // this KILLS files!
	}

 // done       main_window->show();

 // Start program with the About BOX
	if(0)  C_menu_help_about( NULL, this );
}
#endif

void sar_top_app::layout_selected_time( time_t t1 )
{
	int dt = t_hi - t_lo;
//	time_calc_zone era( t1-(dt/2), 24*60*60 );
	time_calc_zone era( t1       , 24*60*60 );
	layout_selected_time_range( era.T0, era.T0 + dt );
}

void sar_top_app::layout_selected_time_adj( int secs )
{
	e_print( "layout_selected_time_adj( %d )\n", secs );
	layout_selected_time_range( t_lo + secs, t_hi + secs );
}

void sar_top_app::layout_selected_time_range( time_t t0, time_t t2 )
{
	// align to day boundry
	int alignment_1min = 60;
	int alignment_1hour = alignment_1min * 60;
	int alignment_1day = alignment_1hour * 24;
	int alignment_1week = alignment_1day * 7;
	int alignment_4week = alignment_1week * 4;
	int alignment = (t2 - t0);
	if( alignment > alignment_4week )
		alignment = alignment_1week;
	else if( alignment > alignment_1week )
		alignment = alignment_1day;
	else if( alignment > alignment_1day )
		alignment = alignment_1hour;
	else
		alignment = alignment_1min;
	time_calc_zone era( t0, alignment );
	int shift1 = era.T0 - t0;
	t_lo = t0 + shift1;
	t_hi = t2 + shift1;
	// if mad, reset to 1 week span
	if(( t_hi - t_lo ) < 50 )
	{
		t_hi = t_lo + 7 * 24 * 60 * 60;
	}
	years_map->highlight_set( t_lo, t_hi );
	if(selected_spec)
		graph_2->plot_line( dset, selected_spec, t_lo, t_hi );
}

void sar_top_app::layout_selected_time_span_days( int ndays )
{
	layout_selected_time_span_secs( ndays * 60 * 60 * 24 );
}

void sar_top_app::layout_selected_time_span_secs( int dt_new )
{
	int dt_old = t_hi - t_lo;
	int adj = (dt_new - dt_old ) / 2;
	time_t t1  = t_lo - adj;
	layout_selected_time_range( t1, t1+dt_new );
}


void sar_top_app::layout_selected_leaf( mem_line_spec * spec )
{
	if(0) {
		e_print("layout_selected_leaf() ");
		spec->print1(stderr);
	}
	graph_2->plot_line( dset, spec, t_lo, t_hi );
	years_map->get_map( dset, spec ); //

	if (selected_spec == NULL)
	{
		/*
			when initially opened, try to show the
			most recent data for 'spec'

			Of course 'spec' could be a discontinued line,
			and leave the user 1 year back!

			But it displays _SOMETHING_
		*/
		time_t TMAX = ((unsigned)-1)>>1;
		dset_api_segs_map map1;
		dset->list_segs_to_dset( & map1, spec, 0, TMAX );
		t_hi = map1.highest(); // start of highest ?
		// 2 week before
		t_lo = t_hi - 2 * 7 * 24 * 60 * 60;
		t_hi = t_hi + 1 * 7 * 24 * 60 * 60;
		// t_lo = map1.lowest();
		selected_spec = spec;
		layout_selected_time_range( t_lo, t_hi );
	}
	selected_spec = spec;

	// graph2 sends this fake to set initial values
	// cross_hairs_at( graph_2->t_lo, 0.0 );
}

void sar_top_app::cross_hairs_at( time_t x_time, flt80 y_val )
{
	char buff[80];
	tm_parts tm;
	tm.local_from_time( x_time );

	sprintf( buff, "%s", tm.str_year_mm_dd() );
	curr_date->set_text( buff );

	sprintf( buff, "%s",
		tm.str_January()
	);
	curr_date2->set_text( buff );

	sprintf( buff, "%s",
		tm.str_Sunday()
	);
	curr_date3->set_text( buff );

	sprintf( buff, "%s",
		tm.str_hh_mm()
	);
	curr_hhmm->set_text( buff ); 

	sprintf( buff, "%.3f", (float)y_val );
	curr_yval->set_text( buff );
}
