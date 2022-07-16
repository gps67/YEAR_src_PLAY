#ifndef X_Pixmap_H
#define X_Pixmap_H

#include "X_STUBS.h"
// free // #include <stdlib.h>
// strdup // #include <string.h>
#include <X11/Xlib.h>

namespace WAX {

struct X_Pixmap
{
	Pixmap pixmap;

	bool create( X_Window & win );

}; // struct
	

}; // namespace
#endif
