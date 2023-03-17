#ifndef X_Pixmap_H
#define X_Pixmap_H

#include "X_STUBS.h"
#include "X_Window.h"

// free // #include <stdlib.h>
// strdup // #include <string.h>
#include <X11/Xlib.h>
#include "A_point_plus.h" // WH

namespace WAX {

struct X_Pixmap : public X_Drawable_Surface
{
//	Pixmap pixmap; // alias drawable
//	A_WH WH; // base class

	X_Pixmap(
                X_Display * _disp,
 //             Drawable _drawable, // late init create will create Pixmap
                A_WH _WH
	);
	X_Pixmap(
                X_Display * _display,
                Drawable _drawable,
                A_WH _WH
	);
//	X_Drawable_Surface( display, drawable, WH_ );


	bool create( X_Window & win, A_WH _WH);
	bool create( Drawable drawable, A_WH _WH);
// 	void get_WH( A_WH & _WH ) { _WH = WH; }

}; // struct
	

}; // namespace
#endif
