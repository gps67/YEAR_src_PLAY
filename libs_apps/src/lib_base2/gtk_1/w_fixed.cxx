#include "w_fixed.h" 
#include "dgb.h" 
#include "buffer1.h" 

w_fixed::~w_fixed()
{
	debug_track_pointer();
	remove_all();
}
w_fixed::w_fixed()
{
	w_set( gtk_fixed_new());
	set_has_window( true );
	INFO("Giving w_fixed a window of its own - helped the yellow-ness");
}
#if 0
w_fixed::w_fixed( str0 label )
{
	w_set( gtk_fixed_new());
	gtk_fixed_set_has_window( Fixed(), true );
}
#endif

void w_fixed::put( w_widget0 * child, int x, int y )
{
	GtkWidget * w2 = child->outer_packable();
	gtk_fixed_put( Fixed(), w2, x, y );
	child->show();
}

void w_fixed::move( w_widget0 * child, int x, int y )
{
	GtkWidget * w2 = child->outer_packable();
	gtk_fixed_move( Fixed(), w2, x, y );
}

bool w_fixed::has_window()
{
	bool has = false;
	if(w)
		has = gtk_fixed_get_has_window( Fixed() );
	else {
		WARN("NULL w");
	}
	INFO( "%d", has );
	return has;
}

void w_fixed::set_has_window( bool own )
{
	WARN("GTK widgets call this to explain that they do/dont, not to change");
	gtk_fixed_set_has_window( Fixed(), own );
	bool owns = gtk_fixed_get_has_window( Fixed() );
	const char * s = "false";
	if(owns) s = "true";
	buffer1 tn;
	INFO("%s.has_window is now %s", type_name(tn), s );
	
}
