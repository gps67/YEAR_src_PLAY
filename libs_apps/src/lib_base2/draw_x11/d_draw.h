#ifndef d_draw_H
#define d_draw_H

#include "d_colour.h"
#include "d_gc.h"
#include "d_text.h"
#include "obj_hold.h"
#include "w_widget0.h"

/*!
	this is a fledgling class, that generalises X11, much as gdk did
*/
class d_draw : public GRP_lib_base2_x11
{
 public:
	GdkDrawable * drawable;
	obj_hold<d_gc> gc;
//	d_text text;

	~d_draw()
	{
	}

	d_draw( w_widget0 * w )
//	: text( w )
	{
		set_drawable( w->get_drawable() );
		gc = NULL;
	}

	void set_drawable( GdkDrawable * _drawable )
	{
		drawable = _drawable;
//		text.set_drawable( drawable );
		// gc must only use compat display.depth
	}

//	void check_text_gc()
//	{
//		if(!text.gc)
//			text.set_gc( gc->gc );
//	}

	bool check_gc()
	{
		if(gc)
		{
//			check_text_gc();
			return true;
		}
		if( !drawable ) 
		{
			e_print("# FAIL # check_gc() NULL drawable \n");
			return false;
		}
		gc = new d_gc( drawable );
		if(!gc) return false;
//		check_text_gc();
		return true;
	}

	void line( int x1, int y1, int x2, int y2 )
	{
		if( !check_gc() ) return;
		gdk_draw_line( drawable, gc->gc, x1, y1, x2, y2 );
	}

	void rectangle_xywh( bool filled, int x, int y, int w, int h )
	{
		if( !check_gc() ) return;
		gdk_draw_rectangle( drawable, gc->gc, filled, x, y, w, h );
	}

	void rectangle_edge( bool filled, int x1, int y1, int x2, int y2 )
	{
		if( !check_gc() ) return;
		gdk_draw_rectangle( drawable, gc->gc, filled, x1, y1, x2-x1, y2-y1 );
	}

};

#endif
