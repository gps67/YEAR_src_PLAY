#ifndef gui_drawing_area_H
#define gui_drawing_area_H

#include "app_session_base.h"
#include "airbook_enrole_args.h"
#include "airbook_enrole_gtk.h"
#include "key_file.h"
#include "spawn_wget_enrole.h"
#include "spawn_ash_tar.h"
#include "spawn_vnc_connect.h"
#include "vpn_utils.h"
#include "dgb.h"
#include "dgb_fail.h"
#include "w_window.h"
#include "w_drawing_area.h"
#include "pix_holder.h"

class my_win;

struct gui_drawing_area : public w_drawing_area
{
	pix_holder pix_da_buffer;

	gui_drawing_area()
	: pix_da_buffer()
	{
	}

	bool call_new()
	{
		w = gtk_drawing_area_new();
		if(!w) {
			return FALSE_dgb_fail("NULL w - did you call gtk_init");
		}
		STEP("connect exp");
	//	connect_expose();
		connect_event_expose();
		return true;
	}


	bool call_new( pix_holder & pix )
	{
		if(!call_new()) return false;
		// w = gtk_drawing_area_new();
		// STEP("connect exp");
	// //	connect_expose();
		// connect_event_expose();

		// I dont know why the pixbuf is copied
		// when the first pixbuf is good
		// maybe its about setting the depths ?

		if(!pix_da_buffer.new_copy_in_format8( pix )) return false;

		return true;
	}

	void event_expose( GdkEventExpose *event )
	{
		STEP("expose");
		guchar *pixels;
		int bytes_per_row;
		int bytes_per_pixel = 3;

		bytes_per_row = gdk_pixbuf_get_rowstride (pix_da_buffer.pixbuf);

		pixels = gdk_pixbuf_get_pixels (pix_da_buffer.pixbuf)
		+ event->area.y * bytes_per_row
		+ event->area.x * bytes_per_pixel;

		gdk_draw_rgb_image_dithalign (
			w->window,		// DEST
			w->style->black_gc,
			event->area.x,
			event->area.y,
			event->area.width,
			event->area.height,
			GDK_RGB_DITHER_NORMAL,
			pixels,			// SOURCE
			bytes_per_row,
			event->area.x,
			event->area.y
		);
	}
};

#endif
