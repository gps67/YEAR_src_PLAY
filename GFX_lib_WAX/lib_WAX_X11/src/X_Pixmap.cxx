#include "X_Pixmap.h"
#include "X_Window.h"

bool
::WAX:: X_Pixmap::
create( X_Window & win )
// create( X_Drawable & win_or_pixmap )
{
	int w = win.window.width;
	// pixmap = Pixmap XCreatePixmap(display, d, width, height, depth)

	return FAIL("TODO");
}





