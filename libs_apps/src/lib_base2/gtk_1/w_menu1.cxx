#include "w_menu1.h"
#include "dgb.h"

/*!
*/
w_menu_item::w_menu_item(
	str0 s,
	SignalFunc func,
	obj_ref * app
) {
	w_set( gtk_menu_item_new_with_label ( s ));
	connect(
		"activate",
		func,
		app
	);
}
/*!
*/
void w_menu_item::right_justify()
{
	gtk_menu_item_right_justify( MENU_ITEM() );
}
/*!
*/
w_menu::w_menu( str0 s ) {
	w_set( gtk_menu_item_new_with_label ( s ));
	pop_up = (GtkMenu *) gtk_menu_new(); // never show
	gtk_menu_item_set_submenu(
		GTK_MENU_ITEM( w ),
		(GtkWidget *) pop_up
	);
	gtk_menu_set_title( pop_up, s );
	// this can look a bit odd
	if(1) {
		GtkWidget * tear = gtk_tearoff_menu_item_new();
		gtk_menu_append (GTK_MENU(pop_up), tear );
		gtk_widget_show ( tear );
	}
}

/*!
void w_menu::append( w_menu_item * item )
{
	ref_keeper.append( item );
	gtk_menu_append (GTK_MENU(pop_up), item->w );
	item->show();
}
*/

/*!
*/
void w_menu::append( w_widget0 * item )
{
	ref_keeper.append( item );
	gtk_menu_append (GTK_MENU(pop_up), item->w );
	item->show();
}

/*!
*/
w_menu_item * w_menu::mk_item(
	str0 s,
	SignalFunc func,
	obj_ref * app
)
{
	w_menu_item * item = new w_menu_item( s, func, app );
	append( item );
	return item ;
}

/*!
*/
w_menu * w_menu_bar::mk_menu_right( str0 s)
{
	w_menu * item = mk_menu( s );
	gtk_menu_item_right_justify( (GtkMenuItem *) item->w );
	return item ;
}

/*!
*/
w_menu_bar::w_menu_bar()
{
	w_set( gtk_menu_bar_new());
}
/*!
*/
void w_menu_bar::append( w_menu * root_menu )
{
	ref_keeper.append( root_menu );
	gtk_menu_bar_append (GTK_MENU_BAR (w), root_menu->w );
	root_menu->show();
}
/*!
*/
w_menu * w_menu_bar::mk_menu( str0 s)
{
	w_menu * m = new w_menu( s );
	append( m );
	return m;
}

/*!
*/
void w_menu_bar::call_destroy()
{
	STEP("we did not gtk-refincr so clear out w from each in ref_keeper[]");
	int n = ref_keeper.N();
	for( int i=0; i< n; i++ )
		ref_keeper[i]->w_set_NULL();
		// remove_sub( ref_keeper[i] );
}
