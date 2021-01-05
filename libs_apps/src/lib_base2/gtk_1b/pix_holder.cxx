#include "pix_holder.h"
#include "dgb.h"


pix_holder::pix_holder()
{
	pixbuf = NULL;
}
pix_holder::~pix_holder()
{
	release_pixbuf();
}

void pix_holder::release_pixbuf()
{
	if( ! pixbuf ) return;
	STEP("TODO: write code");
	// ...
	// unlink
	// un_ref
	STEP("UNREF");
	g_object_unref( pixbuf );
	//
	// free
	// ...
	pixbuf = NULL;
	return;
}

bool pix_holder::new_copy_in_format8( pix_holder & rhs )
{
	release_pixbuf();
	// NEW
	pixbuf = gdk_pixbuf_new(
		GDK_COLORSPACE_RGB,
		FALSE,
		8,
		rhs.get_width(),
		rhs.get_height()
	);
	if(!pixbuf) {
		return FAIL("new returned null");
	}
	// COPY
	gdk_pixbuf_copy_area(
		rhs.pixbuf,	// SRC
		0, 0,
		rhs.get_width(),
		rhs.get_height(),
		pixbuf,		// DST
		0, 0
	);
	return true;
}

bool pix_holder::load_filename(
	const char * filename,
	G_rrr & g_rrr

)
{
	INFO("%s", filename );
	release_pixbuf();
	pixbuf = gdk_pixbuf_new_from_file (filename, g_rrr.grrr());
	if (!pixbuf) {
		FAIL("%s", filename );
		return FALSE;
	}
	return true;
}
//	int get_width()
//	{
//		return gdk_pixbuf_get_width(pixbuf);
//	}
//	int get_height()
//	{
//		return gdk_pixbuf_get_height(pixbuf);
//	}


