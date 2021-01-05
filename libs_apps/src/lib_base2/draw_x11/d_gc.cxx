#include "d_gc.h"
#include "dgb_fail.h"
#include "d_colour.h"


/*!
	constructor - allocate a new GC
*/
d_gc::d_gc( w_widget0 * wid )
{
	GdkDrawable * drawable = wid->get_drawable();
	if(!drawable)
	{
		gc = NULL;
		return_VOID_dgb_fail("new d_gc from NULL drawable");
	}
	gc = gdk_gc_new( wid->get_drawable() ); // get correct depth (?and map)
	gdk_gc_ref( gc );
}

/*!
	constructor - allocate a new GC
*/
d_gc::d_gc( GdkDrawable * drawable )
{
	if(!drawable)
	{
		gc = NULL;
		return_VOID_dgb_fail("new d_gc from NULL drawable");
	}
	gc = gdk_gc_new( drawable );
	gdk_gc_ref( gc );
}

/*!
	destructor - de-allocate the GC
*/
d_gc::~d_gc()
{
	clear_gc();
}

void d_gc::clear_gc()
{
	if( gc ) gdk_gc_unref( gc );
	gc = NULL;
}

/*!
	copy preset values from one d_gc to another (ready to adjust)
*/
void d_gc::copy_from( d_gc & gc2 )
{
	/*
		I think that gc must already exist
		and this copies the fields - not the refcount
		Hence no clear_gc() called
	*/
	gdk_gc_copy( gc, gc2.gc );
}

/*!
	set XOR mode
*/
void d_gc::set_xor()
{
	gdk_gc_set_function( gc, GDK_XOR );
}

/*!
	allocate and set foreground colour from parsable name
*/
void d_gc::set_fg( str0 colour_name )
{
	d_colour * colr = Colour_Map->create_new_colour( colour_name );
	if(colr)
		gdk_gc_set_foreground( gc, & colr->colour );
}

/*!
	allocate and set background colour from parsable name
*/
void d_gc::set_bg( str0 colour_name )
{
	d_colour * colr = Colour_Map->create_new_colour( colour_name );
	if(colr)
		gdk_gc_set_background( gc, & colr->colour );
}

/*!
	set line dotted mode (to defaults that graph2 wants)
*/
void d_gc::set_line_dotted()
{
	gdk_gc_set_line_attributes(
		gc,
		1, // line width
		GDK_LINE_ON_OFF_DASH,
		GDK_CAP_BUTT,
		GDK_JOIN_BEVEL
	);
	gint8 lengths[] = { 2, 2 };
	gdk_gc_set_dashes( gc, 0, lengths, sizeof( lengths ) );
}

