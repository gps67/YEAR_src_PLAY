#include "X_Pixmap.h"
#include "X_Window.h"

#include "X_Display.h"

bool
::WAX:: X_Pixmap::
create( X_Window & W, int width, int height  )
// create( X_Window & win )
// create( X_Drawable & win_or_pixmap )
{

	int depth = DefaultDepth(W.display, 0);

	int d = W.window;
	pixmap = XCreatePixmap(W.display, d, width, height, depth);

	INFO("depth %d", depth ); // depth 24
	INFO("pixmap x%lX", pixmap ); // pixmap x3A00003

	return PASS("DONE");
	return FAIL("TODO");
}





