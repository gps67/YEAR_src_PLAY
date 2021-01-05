
#include "d_colour.h"

// static
d_colour_map * Colour_Map;

	/*!
		constructor - gets the systems global colourmap

		WHY doesnt it need the per-surface colourmap?

		LURK: ref counting?
	*/
	d_colour_map::d_colour_map()
	: names()
	{
		colormap = gdk_colormap_get_system();
	}

	/*!
		destructor
	*/
	d_colour_map::~d_colour_map()
	{
		// gdk_colourmap_unref( colourmap );
	}

	/*!
		find an existing colour or create a new own - by rule
		Repeat requests are cached in the table

		rule is interpreted by gdk_color_parse()
	*/
	d_colour * d_colour_map::create_new_colour( str0 rule )
	{
		d_colour * col = lookup( rule );
		if( col ) return col;
		col = new d_colour();
		gdk_color_parse( rule, & col->colour );
		gdk_colormap_alloc_color( colormap, &col->colour, FALSE, TRUE );
		names.add( new str2( rule ), col );
		return col;
	}

	/*!
		find an existing colour or create a new own - by name, rule
	*/
	d_colour * d_colour_map::create_new_colour( str0 name, str0 rule )
	{
		d_colour * col = lookup( rule );
		if( !col ) {
			col = create_new_colour( rule );
			if( lookup( name ) )
			{
				e_print("create new colour(%s,%s) Name exists but not rule", (STR0) name, (STR0) rule );
				return col;
			}
		}
		names.add( new str2( name ), col );
		return col;
	}

	/*!
		The colour will be remembered with TWO entries,
		so name and rule must not clash (same is OK).
		To change this, simply split "names" to "by_name", "by_rule"
		and split lookup.
	*/
	d_colour * d_colour_map::lookup( str0 name )
	{
		return names.lookup( name );
	}



