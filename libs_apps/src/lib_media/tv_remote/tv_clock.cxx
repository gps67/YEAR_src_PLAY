
#include "tv_clock.h"

#include "w_vbox.h"
#include "w_label.h"
#include "tm_parts.h"
#include <time.h>
#include "tv_panel.h"

using namespace TV_REM;

// class tv_clock;
static gint C_clock_step( tv_clock * clock );

//! update labels each second
void tv_clock::get_time_now()
{
	time_t t = time(0);
	T.local_from_time( t );
	// set_text( T.str_fmt1() );
	l1->set_text( T.str_year_mm_dd() );
	l2->set_text( T.str_hh_mm_ss() );
	char buf[80];
	sprintf( buf, "%s %d %s!",
		T.str_Sunday(),
		T.dd(),
		T.str_January()
	);
	l3->set_text( buf );
}

//! constructor - 3 labels in parent (base class) w_vbox
tv_clock::tv_clock( tv_panel * panel )
: w_vbox()
{
	Panel = panel;
	l1 = new w_label((STR0)NULL);
	l2 = new w_label((STR0)NULL);
	l3 = new w_label((STR0)NULL);
	l4 = new w_label((STR0)NULL);

	// tv_rem is using 5% for 1 sec updates!!
	int ms = 1000;
	// ms = 2000;
	call_back_id = gtk_timeout_add( ms, (GtkFunction) C_clock_step, this );

#ifdef WITH_GTK2_and_some_more_reading_xxx
	// none of the following works!
	PangoAttrList * attrs;
	attrs = gtk_label_get_attributes( l2->Label() );
	if(!attrs) {
		e_print("null attrs\n");
		attrs = pango_attr_list_new();
	}
	PangoAttribute* attr;
	attr = pango_attr_size_new( 18 * 1000 );
	pango_attr_list_insert ( attrs, attr );

	attr = pango_attr_background_new( 0xFFFF, 0, 0 );
	pango_attr_list_insert ( attrs, attr );

	gtk_label_set_attributes( l2->Label(), attrs );
if(0)	pango_attr_list_unref( attrs );
#endif

	pack_asis( l1 );
	pack_asis( l2 );
	pack_asis( l3 );
	pack_asis( l4 );
}

//! destructor - de-register callback
tv_clock::~tv_clock()
{
	gtk_timeout_remove( call_back_id );
}

//! C callback that calls C++ member function
static gint C_clock_step( tv_clock * clock )
{
	clock->get_time_now();
	if(clock->Panel)
	{
		clock->Panel->poll_for_external_change();

		if( clock->Panel->cd_music )
			clock->set_extra_line( clock->Panel->cd_music->pos_str() );
	}
	return 1; // repeat the callback
}

void tv_clock::set_extra_line( str0 msg )
{
	
	l4->set_text( msg );
}
