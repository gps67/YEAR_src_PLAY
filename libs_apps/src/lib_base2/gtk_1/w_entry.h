#ifndef w_entry_H
#define w_entry_H

#include "w_widget0.h"
#include "str1.h"
#include "buffer2.h"
#include "dgb.h"

namespace VARS {
	class V_var_decl;
};
#include "var_tree.h"

/*!
	A text field entry widget
*/
class w_entry : public w_widget0
{
 public:
	/*!
		return GTK object
	*/
	GtkEntry * Entry() { return (GtkEntry *) w; }

	obj_hold<VARS::V_var_decl> V;

	/*!
		constructor - allocate new blank GTK entry
	*/
	w_entry( VARS::V_var_decl & _V  );

	/*!
		constructor - allocate new blank GTK entry
	*/
	w_entry( VARS::V_var_decl * _V = NULL );

	/*!
		constructor from existing GtkEntry
	*/
	w_entry( GtkEntry * w2 );

	/*!
		constructor with initial string value
	*/
	w_entry( str0 text );

	void set_width( int chars = -1 );

	/*!
		set text value
	*/
	void set_text( str0 str );

	void set_text_from_V();
	void set_text_into_V();

	/*!
		set text value
	*/
	void set_text( const str2 * str );

	/*!
		get text value
	*/
	bool get_text( str1 & txt );

	/*!
		get text value
	*/
	str0 get_text();

	/*!
		get text value
	*/
	str0 get_edited_value_as_cstr();

	/*!
		get text value (as new str2)
	*/
	str2 * get_str2();

	/*!
		get text value (as new str2)
	*/
	void set_visibility( bool vis = false );

	/*!
		get text value (as new str2)
	*/
	void set_is_password();

	/*!
		used to validate fields that are empty
	*/
	int get_text_len();

	/*!
		used to validate fields
	*/
	bool is_empty();

	/*!
		used to validate fields
	*/
	bool is_at_least_len(int len);

	/*!
		used to validate fields
	*/
	bool is_less_than_len(int len);

	void zap();

	
};

class w_entry_pass : public w_entry
{
 public:
	w_entry_pass( VARS::V_var_decl * _V = NULL );
	w_entry_pass( VARS::V_var_decl & _V );
	w_entry_pass( str0 text ) ;
};

#endif
