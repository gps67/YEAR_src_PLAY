#ifndef w_note_book_H
#define w_note_book_H
#include "str2.h" 
// #include "w_win_scrolled.h" 
#include "w_container_.h" 

/*!
	a basic notebook - see note_book_one

	The page are numbered, but note_book_one manages to track
	its use of GTK, and NAME the pages.

	If the page names are not the (dynamic) table names (LANG), 
	or if you want pretty labels, you will want to replace add_page.

	LURK ALERT - just like container_ there is a keeper/dispose issue.
*/
class w_note_book_base : public w_container_N
{
 	static void C_switch_page(
		GtkNotebook * tree,
		GtkNotebookPage * gtk_page,
		gint _page_num,
		w_note_book_base * _app
	);
 public:
	void connect_event_switch();
	virtual void event_switch_page( int idx ); // signalled 

	// obj_list<w_widget0>	list_subs;
	GtkNotebook * Notebook() { return (GtkNotebook *) w ; }

	w_note_book_base();

	int current_page_num(); // -1 means none
	void remove_page_num( int num );

	void set_tab_pos( GtkPositionType pos );

	// widget will PROBABLY be forgotten but sub->w kept
	// else CALLER does ref_incr and on_destroy

	void add_page( str0 name, w_widget0 * sub );
	void add_page( int pos, str0 name, w_widget0 * sub );
	void replace_page( int pos, str0 name, w_widget0 * sub );
	void add_page( str2 * name, w_widget0 * sub );
	void set_current_page( int pos );

	void remove_all_pages();
	virtual void remove_all();

};

#endif
