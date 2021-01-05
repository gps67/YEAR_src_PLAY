#include "w_entry.h"

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

	/*!
		constructor - allocate new blank GTK entry
	*/
	w_entry:: w_entry( VARS::V_var_decl & _V  )
	{
		w_set( gtk_entry_new());
		V = & _V;
		if(V) set_text_from_V();
	}

	/*!
		constructor - allocate new blank GTK entry
	*/
	w_entry:: w_entry( VARS::V_var_decl * _V )
	{
		w_set( gtk_entry_new());
		V = _V;
		if(V) set_text_from_V();
	}

	/*!
		constructor from existing GtkEntry
	*/
	w_entry:: w_entry( GtkEntry * w2 )
	{
		w_set( (GtkWidget*) w2 );
		if(V) set_text_from_V();
	}

	/*!
		constructor with initial string value
	*/
	w_entry:: w_entry( str0 text )
	{
		w_set( gtk_entry_new());
		set_text( text );
		if(V) set_text_from_V();
	}

	void w_entry:: set_width( int chars ) {
		gtk_entry_set_width_chars( Entry(), chars );
	}

	/*!
		set text value
	*/
	void w_entry:: set_text( str0 str )
	{
		gtk_entry_set_text( Entry(), str );
		if(V) {
			if(V->validate( str )) {
				V->set_from_text( str );
			} else {
				FAIL("NOT VALID");
			}
		}
	}

	void w_entry:: set_text_from_V()
	{
		if(!V) {
			FAIL("NULL V");
			return;
		}
		str0 str = V->get_as_text();
		gtk_entry_set_text( Entry(), str );
		INFO("%20s = %s", (STR0) V->name_sto, (STR0) str );
		if(V->TEST_is_pass()) {
			INFO("IS PASS - should set is password");
		}
	}

	void w_entry:: set_text_into_V()
	{
		if(!V) {
			FAIL("NULL V");
			return;
		}
		str0 str = gtk_entry_get_text( Entry());
		V->set_from_text(str);
		if(V->TEST_is_pass()) {
			str = "SOMEPASSWORD";
		}
		INFO("%20s = %s", (STR0) V->name_sto, (STR0) str );
	}

	/*!
		set text value
	*/
	void w_entry:: set_text( const str2 * str )
	{
		if( str )
			set_text( (str0) *str );
		else
			set_text( (str0) "" );
	}

	/*!
		get text value
	*/
	bool w_entry:: get_text( str1 & txt )
	{
		txt = get_text();
		return true;
	}

	/*!
		get text value
	*/
	str0 w_entry:: get_text()
	{
		// must not be free stored or modifed // belongs to gtk
		return gtk_entry_get_text( Entry() );
	}

	int w_entry:: get_text_len()
	{
		str0 val = get_text();
		return val.str_len();
	}

	bool w_entry:: is_at_least_len(int len)
	{
		return get_text_len() >= len;
	}

	bool w_entry:: is_less_than_len(int len)
	{
		return ! is_at_least_len( len );
	}

	bool w_entry:: is_empty()
	{
		return get_text_len() == 0;
	}


	/*!
		get text value
	*/
	str0 w_entry:: get_edited_value_as_cstr()
	{
		return get_text();
	}

	/*!
		get text value (as new str2)
	*/
	str2 * w_entry:: get_str2()
	{
		return new str2( get_text() );
	}

	/*!
		get text value (as new str2)
	*/
	void w_entry:: set_visibility( bool vis )
	{
		gtk_entry_set_visibility( Entry(), vis );
	}

	/*!
		get text value (as new str2)
	*/
	void w_entry:: set_is_password()
	{
		set_visibility( false );
	}

	void w_entry:: zap()
	{
		buffer2 buf;
		const char * PGTK = gtk_entry_get_text( Entry() );
		char * wr_PGTK = (char *) PGTK;
		buf.set( PGTK );
		int N = buf.nbytes_used;
		uint i = 0;
		for( i = 0; i < buf.nbytes_used; i++) {
			buf.buff[i] = '-' ;
			wr_PGTK[i] = '!';
#warning erase GTKS memory is rude and might not be sufficient
		}
		set_text( (str0) buf );
		// that was the same length - avoid realloc, now stretch
		// of course if stretch DID reallocate, free would show len
		int N2 = 58; // a long password ? as background
		buf.get_space( N2-N );
		while( (int) i < N2 ) {
			i++;
			buf.put('-');
		}
		set_text( (str0) buf );
		set_text( "" );
	}

/////////////////////////////////////


w_entry_pass:: w_entry_pass( VARS::V_var_decl * _V )
: w_entry(_V)
{
	if(_V && _V->TEST_is_pass()) {
		// set_is_password();
	} else {
		// set_is_password();
	}
	// set_is_password();
}

w_entry_pass:: w_entry_pass( VARS::V_var_decl & _V )
: w_entry(&_V)
{
	set_is_password();
}

w_entry_pass:: w_entry_pass( str0 text ) 
: w_entry( text )
{
	set_is_password();
}

