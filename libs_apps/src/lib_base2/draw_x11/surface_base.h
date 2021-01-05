#ifndef sarface_base_H
#define sarface_base_H

#include "d_gc.h"
#include "d_text.h"
#include "ht2.h"

typedef ht2<str2,d_gc>		ht2_gc;
typedef ht2<str2,d_text>	ht2_text;

/*!
	unused untested new code - to hold lots of named GC's

	surface_base surface( drawable );

	if(!surface.create_d_gc( "ticks" )) return FALSE_dgb_fail( "...");
	surface.set_fg( "purple" ); // permanant setting for "ticks"

	if(!surface.create_text("text y values")) return FALSE_dgb_fail( "...");
	surface.set_font("courier 12");
	
	...

	if(!with_gc( "ticks" )) return FALSE_dgb_fail("code error");
	surface.line( p1, p2 );

	if(!with_text("text y values")) return false;
	surface.text->go_y_tickline( p1.y );
	surface.text->str_x_mid( p1.x - 2 );
	
	
	
*/
class surface_base : public obj_ref
{
	GdkDrawable * drawable;

	ht2_gc		ht_gc;
	ht2_text	ht_text;

	d_gc *		gc;
	d_text *	text;

 public:

	surface_base();
	~surface_base();

	d_gc * create_d_gc( str2 * name );
	d_text * create_d_text( str2 * name );
	d_gc * get_d_gc( str0 name );
	d_text * get_d_text( str0 name );

};

#endif
