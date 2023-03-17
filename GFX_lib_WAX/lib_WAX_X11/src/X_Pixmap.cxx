#include "X_Pixmap.h"
#include "X_Window.h"

#include "X_Display.h"
#include "X_Pixmap.h"

/*
::WAX:: X_Pixmap::
X_Pixmap(int d)
: X_Drawable_Surface(d) // FAKE
{
}
*/

::WAX:: X_Pixmap:: X_Pixmap(
	X_Display * _disp,
//      Drawable _drawable, // late_init create will create Pixmap
	A_WH _WH
)
: X_Drawable_Surface( _disp, -1, _WH )
{
	INFO("late init drawable set to -1");
	FAIL("late init drawable set to -1");
}

::WAX:: X_Pixmap:: X_Pixmap(
	X_Display * _disp,
	Pixmap _drawable,
	A_WH _WH
) : X_Drawable_Surface( _disp, _drawable, _WH )
{
}


bool
::WAX:: X_Pixmap::
create( X_Window & win, A_WH _WH)
{
	X_Display * _disp = win.disp;
	Drawable _drawable = win.drawable;
	return create( _drawable, _WH );
}

bool
::WAX:: X_Pixmap::
create( Drawable _drawable, A_WH _WH)
{
	WH = _WH; // should match == // already ?

	int screen0 = 0;
	int depth = DefaultDepth(slow_get_display(), screen0);

	// late_init pixmap = drawable = XCreatePixmap( ... ) // was -1 //
	// create pixmap // errno not in value // error detect = TODO //
	drawable = XCreatePixmap( slow_get_display(), _drawable, WH.w, WH.h, depth );

	INFO("depth %d", depth ); // depth 24
	INFO("pixmap x%lX", drawable ); // pixmap x3A00003

	return PASS("DONE");
}

