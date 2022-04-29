#include "XFT.h"
#include <X11/Xft/Xft.h>

using namespace WAX;
// using namespace WAX::XFT;

bool
// WAX::XFT::
WAX::
Xft_Draw::
Create()
{
	Display  *dpy;
	Drawable drawable;
	Visual   *visual;
	Colormap colormap;

	draw =
	XftDrawCreate (dpy, drawable, visual, colormap);


	return true;
}



