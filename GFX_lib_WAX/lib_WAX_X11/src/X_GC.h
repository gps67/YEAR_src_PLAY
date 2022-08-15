#ifndef X_GC_H
#define X_GC_H

#include "X_STUBS.h"
// X_STUBS fails to define a STUB for XGCValues // struct XGCValues; // so Xlib
#include <X11/Xlib.h>

namespace WAX {

	/*!
		Hold an allocated GC
	*/
	struct X_GC {
		GC gc;

		X_GC()
		: gc(0)
		{
		}

		operator GC() { return gc; }

		bool is_UNSET() { return gc == 0; }

		bool Create(
			Display * display,
			Drawable drawable,
			int valuemask,
			XGCValues * values
		);

		bool Free(
			Display * display
		);
	};

}; // namespace
#endif

