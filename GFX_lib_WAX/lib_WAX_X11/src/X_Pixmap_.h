#ifndef X_Pixmap_H
#define X_Pixmap_H

#include "X_STUBS.h"
// free // #include <stdlib.h>
// strdup // #include <string.h>
#include <X11/Xlib.h>
#include "A_point_plus.h" // WH

namespace WAX {

struct X_Pixmap
{
	Pixmap pixmap;
	A_WH WH;

	X_Pixmap();

//	bool create( X_Window & win );
	bool create( X_Window & win, A_WH _WH);
	bool create( Display * display, Drawable drawable, A_WH _WH);
	// you also need { GC gc } to draw on it
//	int get_width() { return WH.w; }
//	int get_height() { return WH.h; }
	void get_WH( A_WH & _WH ) { _WH = WH; }

}; // struct
	

}; // namespace
#endif
