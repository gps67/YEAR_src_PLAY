#include "w_note_book_one.h"


/*!
	constructor - all work is done within the pages, not here
*/
w_note_book_one::w_note_book_one()
{
	connect_event_switch(); // no need ;-)
}

/*!
*/
w_note_book_one::~w_note_book_one()
{
	debug_track_pointer();
	// remove_all_pages();
}

/*!
	notebook switched itself, app does have to do anything
*/
void w_note_book_one::event_switch_page( int idx )
{
	// This confirms that name - idx is correct
	str2 * name = get_name_at_idx( idx );
	if(0) e_print("w_note_book_one.event_switch_page(%d=%s)\n", idx, (STR0) *name );
}

/*!
	get the (internal) name of the tab
*/
str2 * w_note_book_one::get_name_at_idx( int idx )
{
	return list_names[ idx ];
}

/*!
	find idx of widget in own managed list
*/
bool w_note_book_one::get_idx_of_widget( w_widget0 * wid, int & idx )
{
	return keeper.find_ptr( wid , idx );
}

/*!
	convert name to index, via own managed list
*/
bool w_note_book_one::get_idx_of_name( str2 * _name, int & idx )
{
	// LIKE: list_names.find_ptr() but _name doesnt have to be the same ptr
	str0 name = *_name;
	int n = list_names.N();
	for( int i = 0; i<n; i++ )
	{
		if( list_names[i]->is_same_as( name ) ) {
			idx = i;
			return TRUE;
		}
	}
	if(1) e_print("get_idx_of_name(%s) not found (ok iff adding)\n", (STR0) name );
	return FALSE;
}

/*!
	append name/page to notebook
*/
void w_note_book_one::page_new( str2 * name, w_widget0 * sub )
{
	int idx = 0;
	if( get_idx_of_name( name, idx ) )
	{
		// not new name or idx, but new sub
		page_replace( idx, sub );
		return;
	}
	// must append name before page - as switch_page gets called
	list_names.append( name );
	add_page( name, sub );

	// if you dont set the page, there is a wierd gtk bug
	// can only switch PROPERLY after one DECREASING select
	set_current_page( 0 );
}

/*!
	delete by name
*/
void w_note_book_one::page_del( str2 * name )
{
	int idx;
	if( get_idx_of_name( name, idx ) )
	{
		page_del( idx );
	} else {
		e_print("w_note_book_one::page_del(%s) - MISSING\n", (STR0) * name );
	}
}

/*!
	delete by idx, manage the names_list
*/
void w_note_book_one::page_del( int idx )
{
	if(1) e_print("page_del(%d)\n", idx );
	obj_hold<w_widget0> page; page = (w_widget0 *) keeper[idx];
	list_names.del( idx );
	remove_page_num( idx );
}

/*!
	replace page with prepared widget, keeping same name

	if you upgrade to more advanced tab widget, or internationalise
	the external name, some extra funcs might confuse the prototypes
*/
void w_note_book_one::page_replace( int idx, w_widget0 * sub )
{
	str2 * name = get_name_at_idx( idx );
	w_note_book_base::replace_page( idx, (STR0) *name, sub );
}

/*!
	Remove the pages in reverse order, to make it easier to
	keep the names array in the same order, and the ground here
*/
void w_note_book_one::remove_all_pages()
{
	if(0) e_print("remove_all_pages\n");
	int n = keeper.N();
	for( int i = n-1; i>=0; i-- )
	{
		if(0) e_print("	remove_all_pages\n");
		page_del( i );
	}
}

/*!
	get current page as a widget
*/
w_widget0 * w_note_book_one::current_page_widget()
{
	int idx = current_page_num();
	if( idx == -1 ) {
		e_print("current page is NULL\n");
		return NULL;
	}
	return keeper[idx];
}

/*!
*/
// VIRTUAL
void w_note_book_one::remove_all()
{
	remove_all_pages();
}





