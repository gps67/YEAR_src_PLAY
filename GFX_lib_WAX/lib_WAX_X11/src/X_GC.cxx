#include "dgb.h"
#include "X_GC.h"

using namespace WAX;

bool X_GC:: Create(
	Display * display,
	Drawable drawable,
	int valuemask,
	XGCValues * values	// struct of all style attrs
)
{
	if(!display) return FAIL("NULL display");
	if(gc) WARN("gc not previously ZERO");
	gc = XCreateGC( display, drawable, valuemask, values );
	return true;
}

bool X_GC:: Free(
	Display * display
)
{
	if(gc) {
		XFreeGC( display, gc );
		gc = 0;
	}
	return true;
}




