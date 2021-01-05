#include "w_combo_plus.h"

/*!
	constructor
*/
w_combo_plus::w_combo_plus()
: w_combo_base()
, item_list()
{
}

/*!
	Search for the key value in the list of items
*/
combo_item * w_combo_plus::find_key( str2 * k1 )
{
	int n = item_list.N();
	for( int i=0; i<n; i++ )
	{
		combo_item * item = item_list[i];
		if( item->key->is_same_as( * k1 ) )
		{
			return item;
			break;
		}
	}
	if(1) e_print( "w_combo_plus:find_key( %s ) - NOT FOUND\n", (STR0) k1 );
	return NULL;
}

/*!
	select the value BY KEY (GTK needs this as the full pair text)
*/
void w_combo_plus::set_value( str2 * key )
{
	if(!key) return;
	if(!w) return;
	combo_item * item = find_key( key );
	if( ! item )
	{
		if(1) e_print( "w_combo_plus:set_value( %s ) - NOT FOUND\n", (STR0) key );
		return;
	}

	str0 text = * item->pair;
	w_combo_base::set_value( text );

	/*
		Gtk BUG Note:
		plain UP/DOWN works as expected
		but pressing the triangle -
			the input fields immediately becomes blank
			the item is not highlighted in the list
		However: 
			after a change, it does works as expeceted
	*/
}

/*!
	return "key" text for the current entry text
*/
str0 w_combo_plus::get_text()
{
	str0 text = w_combo_base::get_text();

	/*
		search for "key - desc" in list to return "key"
	*/
	int n = item_list.N();
	for( int i=0; i<n; i++ )
	{
		combo_item * item = item_list[i];
		if( item->pair->is_same_as( text ) )
		{
			return * item->key;
		}
	}
	return text;
}

/*!
	destructor
*/
w_combo_plus::~w_combo_plus()
{
}

/*!
	move this down to _base class
*/
void w_combo_plus::prep_done()
{
	int n = item_list.N();
	str_list list( 0 ); // init n fills it with NULLS!
	for( int i=0; i<n; i++)
	{
		list.append( item_list[i] -> pair );
	}
	set_strings( & list );
}

/*!
	move this down to _base class
*/
void w_combo_plus::prep_row( str2 * key, str2 * desc )
{
	combo_item * item = new combo_item( key, desc );
	item_list.append( item );
}

/*!
	move this down to _base class
*/
void w_combo_plus::prep_row( str2 * key, str2 * desc, str2 * pair )
{
	combo_item * item = new combo_item( key, desc, pair );
	item_list.append( item );
}

