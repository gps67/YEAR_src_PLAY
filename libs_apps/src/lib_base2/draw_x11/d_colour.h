#ifndef d_colour_H
#define d_colour_H

#include "ht2.h"
#include "w_widget0.h"

/*!
	a d_colour is a resolved R.G.B value for a surface

	By using common lookup tables, the same colour will USUALLY
	have the same pointer to a similar value.
*/
class d_colour : public obj_ref
{
 public:
	GdkColor colour;

	d_colour()
	{
	}

	~d_colour()
	{
	}
};

//! a hash table of name->olours
typedef ht2<str2,d_colour> ht_colour;

/*!
	a systems colourmap, and a lookup table
	of names and rules into it.
*/
class d_colour_map : public GRP_lib_base2_x11
{
 public:
	//! the systems global colourmap (should be pre surface?)
	GdkColormap * colormap;
	//! the application, X11, and formula names 
	ht_colour names;

	d_colour_map();
	~d_colour_map();

	d_colour * create_new_colour( str0 name, str0 rule );
	d_colour * create_new_colour( str0 rule );
	d_colour * lookup( str0 name );
};

//! a global colour map - needed by ... ?
extern d_colour_map * Colour_Map;



#endif
