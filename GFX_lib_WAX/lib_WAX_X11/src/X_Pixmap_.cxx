#include "X_Pixmap.h"
#include "X_Window.h"

#include "X_Display.h"

::WAX:: X_Pixmap::
X_Pixmap()
: pixmap(0) // it is an int
, WH()
{
}


bool
::WAX:: X_Pixmap::
create( X_Window & win, A_WH _WH)
// create( X_Window & win )
// create( X_Drawable & win_or_pixmap )
{
	Drawable drawable = win.drawable;
	Display * display = win.display;
	return create( display, drawable, _WH );
}

bool
::WAX:: X_Pixmap::
create( Display * display, Drawable drawable, A_WH _WH)
// create( X_Window & win )
// create( X_Drawable & win_or_pixmap )
{
	WH = _WH;

	int screen0 = 0;
	int depth = DefaultDepth(display, screen0);

	// create pixmap // errno not in value // error detect = TODO //
	pixmap = XCreatePixmap( display, drawable, WH.w, WH.h, depth );

	INFO("depth %d", depth ); // depth 24
	INFO("pixmap x%lX", pixmap ); // pixmap x3A00003

	return PASS("DONE");
}

