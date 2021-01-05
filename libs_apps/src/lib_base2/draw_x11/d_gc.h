#ifndef d_gc_H
#define d_gc_H

#include "obj_hold.h"
#include "w_widget0.h"

/*!
	d_gc holds an allocated gc. You create several.

	Its not efficient for switching between colours and dottedness,
	but its good for setting them.

	Memory rich PC's should not have a problem with several GC's
	allocated (one per constant style).

	NB The initialisation must already have a valid drawable,
	but that can be re-allocated within the same screen
	(eg pixmap realloc on resize)
*/
class d_gc : public obj_ref
{
	void clear_gc();
 public:
	GdkGC * gc;

	d_gc( GdkDrawable * drawable );
	d_gc( w_widget0 * wid );
	~d_gc();
	void copy_from( d_gc & gc2 );
	void set_xor();
	void set_fg( str0 colour_name );
	void set_bg( str0 colour_name );
	void set_line_dotted();
};

#endif
