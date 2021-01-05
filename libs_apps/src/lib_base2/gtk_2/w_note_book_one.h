#ifndef w_note_book_one_H
#define w_note_book_one_H

#include "w_note_book_base.h"
#include "str2.h"
#include "obj_list.h"
// #include "ht2.h"


/*!
	a notebook that keeps refences to its named pages
*/
class w_note_book_one : public w_note_book_base
{

 public:
	obj_list<str2>		list_names;

        ~w_note_book_one();
        w_note_book_one();
	str2 * get_name_of_idx( int idx );
	bool get_idx_of_name( str2 * name, int & idx );
	bool get_idx_of_widget( w_widget0 * wid, int & idx );
	str2 * get_name_at_idx( int idx );

	void page_replace( int idx, w_widget0 * sub );
	void page_new( str2 * name, w_widget0 * sub );
	void page_del( str2 * name );
	void page_del( int idx );
	void remove_all_pages();

	w_widget0 * current_page_widget();

	void reload_page();	// table changed
// VIRTUAL
	void event_switch_page( int pos );
// VIRTUAL
	void remove_all();

};

#endif
