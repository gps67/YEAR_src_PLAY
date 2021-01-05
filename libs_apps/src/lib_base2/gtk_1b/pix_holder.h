#ifndef pix_holder_H
#define pix_holder_H

#include "w_widget0.h"
#include "g_rrr.h"

/*!
	offscreen pixbuf (but possibly also at core of widget)
*/
struct pix_holder
{
	GdkPixbuf * pixbuf;
	//
	pix_holder();
	~pix_holder();

	void release_pixbuf();

	bool new_copy_in_format8( pix_holder & rhs );

	bool load_filename(
		const char * filename,
		G_rrr & g_rrr
	);

	int get_width()
	{
		return gdk_pixbuf_get_width(pixbuf);
	}
	int get_height()
	{
		return gdk_pixbuf_get_height(pixbuf);
	}
} ;
#endif

