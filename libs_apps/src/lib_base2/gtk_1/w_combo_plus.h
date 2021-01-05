#ifndef w_combo_plus_H
#define w_combo_plus_H

#include "obj_list.h"
#include "str_list.h"
#include "w_widget0.h"
#include "str_builder.h"
#include "w_combo_base.h"

/*!
	combo_item is a custom item in a w_combo_plus

	a "pair" = "key - desc" 
*/
struct combo_item : public obj_ref
{
	obj_hold<str2>		key;	//!< abbreviation
	obj_hold<str2>		desc;	//!< full text
	obj_hold<str2>		pair;	//!< combined with a middle dash

	/*!
		constructor - caller provides third string
	*/
	combo_item( str2 * k, str2 * d, str2 * k_d )
	{
		key = k;
		desc = d;
		pair = k_d;
	}
	/*!
		constructor - third string is "key -- desc"
	*/
	combo_item( str2 * k, str2 * d )
	{
		key = k;
		desc = d;
		str_builder b;
		b.append( k );
		b.append( " -- " );
		b.append( d );
		pair = b.get_str2();
	}

};

/*!
	w_combo_plus offers a list of abbreviations and full text.

	Thats slightly contrived, because the key has to be searched for
	in the list. It almost works, but new items need an external
	add-new button.

	w_combo_plus is NOT related to combo_base, its similar from scratch
*/
class w_combo_plus : public w_combo_base
{
	/*!
		a list of key - desc triplets
	*/
	obj_list<combo_item> item_list;
 public:

	//! destructor
	~w_combo_plus();

	//! constructor
	w_combo_plus();

	//! add a pair
	void prep_row( str2 * key, str2 * desc );

	//! add a pair
	void prep_row( str2 * key, str2 * desc, str2 * pair );

	//! complete the construction
	void prep_done();

	//! set the value
	void set_value( str2 * s2 );

	//! get the edited or selected text
	str0 get_text();

	//! search for the row in the list
	combo_item * find_key( str2 * s2 );
};

#endif
