#ifndef w_combo_base_H
#define w_combo_base_H

#include "obj_list.h"
#include "str_list.h"

/*!
	a combo is a pick list of strings (maybe other things?)
*/
class w_combo_base : public w_widget0
{
 public:

	/*!
		return the gtk widget
	*/
	GtkCombo * Combo() { return (GtkCombo *) w; }

	/*!
		return the gtk widget of the entry field
	*/
	GtkEntry * Entry() {
		if(!w) return NULL;
		return (GtkEntry *) Combo()->entry;
	}

	/*!
		constructor
	*/
	w_combo_base()
	{
		w_set( gtk_combo_new());
	}

	/*!
		set the list of strings (copied or not?)
	*/
	void set_strings( str_list * s_list )
	{
		GList * g_list = g_list_alloc();
		int n = s_list->N();
		for( int i = 0; i<n; i++ )
		{
			const char * s = s_list->get_str(i);
			g_list = g_list_append( g_list, (void *)s );
		}
		gtk_combo_set_popdown_strings( Combo(), g_list );
	}

	/*!
		require an exact match - nothing new
	*/
	void exact_only()
	{
		gtk_combo_set_value_in_list( Combo(), TRUE, FALSE );
	}

	/*!
		set the value to a string (copied?)
	*/
	void set_value( str0 s2 )
	{
		if(!s2) return;
		if(!w) return;
		gtk_entry_set_text( Entry(), s2 );
	}

	/*!
		set the value to a string (copied?)
	*/
	void set_value( str2 * s2 )
	{
		if(!s2) return;
		set_value( s2->sget() );
	}

	/*!
		get the edited text
	*/
	str0 get_text()
	{
		return gtk_entry_get_text( Entry() );
	}

	/*!
		get the edited text
	*/
	str0 get_edited_value_as_cstr()
	{
		return get_text();
	}
};

#endif
