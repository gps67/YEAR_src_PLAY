#include "w_note_book_base.h"

/*!
	convert C callback to virtual function
*/
void w_note_book_base::C_switch_page(
	GtkNotebook * tree,
	GtkNotebookPage * gtk_page,
	gint _page_num,
	w_note_book_base * _app
) {
	_app->event_switch_page( _page_num );
}

/*!
	notebook switched itself, app does have to do anything
*/
void w_note_book_base::event_switch_page( int idx )
{
	// str2 * name = get_name_at_idx( idx );
	if(1) e_print("w_note_book_base.event_switch_page(%d=%s)\n", idx, (STR0) "no-name-unknown" );
}

/*!
	called by constructor
*/
void w_note_book_base::connect_event_switch()
{
//	void * self = this;
	connect(
		"switch-page",
		(SignalFunc) C_switch_page
	);
}

//////////////////////////////////////////


/*!
*/
w_note_book_base::w_note_book_base()
: w_container_N()
{
	w = gtk_notebook_new();
	gtk_notebook_set_scrollable(Notebook(), TRUE );
	gtk_notebook_popup_enable(Notebook());
}

/*!
*/
int w_note_book_base::current_page_num() // -1 means none
{
	return gtk_notebook_get_current_page( Notebook() );
}

/*!
*/
void w_note_book_base::remove_page_num( int num )
{
	if( num < 0 )
	{
		int n = keeper.N();
		if(!n) return;
		num = n-1;
	}
	gtk_notebook_remove_page( Notebook(), num ); // -1 is last
	keeper.del( num ); // delete after event_destroyed() called
}

/*!
	the TABS will be drawn LEFT RIGHT TOP BOTTOM
*/
void w_note_book_base::set_tab_pos( GtkPositionType pos )
{
	// GTK_POS_LEFT _RIGHT _TOP _BOTTOM
	gtk_notebook_set_tab_pos( Notebook(), pos );
}

/*!
*/
void w_note_book_base::add_page( str0 name, w_widget0 * sub )
{
	int pos =  keeper.N();
	add_page( pos, name, sub );
	return;

	GtkWidget * lbl = gtk_label_new( name );
	gtk_notebook_append_page( Notebook(), sub->w, lbl );
	keeper.append( sub );
	sub->show();
	if(1) sub->connect_event_destroyed();
}

/*!
*/
void w_note_book_base::add_page( int pos, str0 name, w_widget0 * sub )
{
	GtkWidget * lbl = gtk_label_new( name );
	gtk_notebook_insert_page( Notebook(), sub->w, lbl, pos );
	keeper.ins( pos, sub );
	sub->show();
	if(1) sub->connect_event_destroyed();
}

/*!
*/
void w_note_book_base::replace_page( int pos, str0 name, w_widget0 * sub )
{

	remove_page_num( pos );
	add_page( pos, name, sub );
	gtk_notebook_set_current_page( Notebook(), pos );
	return;

	/*
	   keep widget alive until gtk actually gets the newone
	   and gtk disposes of the old one, some callback trashes the heap
	   Not that it took 2 days to find that out
	*/
	obj_hold<w_widget0> prev;
	prev = keeper[ pos ];
	keeper[ pos ] = sub;

	// flickers a bit
	GtkWidget * lbl = gtk_label_new( name );
	gtk_notebook_remove_page( Notebook(), pos );
	gtk_notebook_insert_page( Notebook(), sub->w, lbl, pos );
	// must show page before focussing it
	sub->show();
	gtk_notebook_set_current_page( Notebook(), pos );
	// gtk_notebook_set_page( Notebook(), pos );

	sub->connect_event_destroyed();
}

/*!
	create a plain-text label "name" and attach its sub-page
*/
void w_note_book_base::add_page( str2 * name, w_widget0 * sub )
{
	// GtkWidget * lbl = gtk_label_new( name->get() );
	GtkWidget * lbl = gtk_label_new( *name );
	gtk_notebook_append_page( Notebook(), sub->w, lbl );
	keeper.append( sub );
	sub->show();
	/*
		This is crucial, sub must be told
	*/
	sub->connect_event_destroyed();
}

/*!
	tell gtk to switch the current page to POS
*/
void w_note_book_base::set_current_page( int pos )
{
	gtk_notebook_set_current_page( Notebook(), pos );
}

/*!
	Remove the pages in reverse order, to make it easier to
	keep the names array in the same order, and the ground here

	maybe page_del could be virtual ...
*/
void w_note_book_base::remove_all_pages()
{
	if(0) e_print("remove_all_pages\n");
	int n = keeper.N();
	for( int i = n-1; i>=0; i-- )
	{
		if(0) e_print("	remove_all_pages\n");
		remove_page_num( i );
	}
}

// VIRTUAL
void w_note_book_base::remove_all()
{
	remove_all_pages();
}

