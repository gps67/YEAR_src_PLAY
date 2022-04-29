#include "XFT.h"
#include <X11/Xft/Xft.h>
#include "X_Window.h"
#include <math.h>

using namespace WAX;
// using namespace WAX::XFT;

WAX::
Xft_Draw::
Xft_Draw()
: draw( NULL )
{
	INFO("requires late init create(W)");
}

WAX::
Xft_Draw::
Xft_Draw( X_Window & W )
: draw( NULL )
{
	if( W.window )
	  if(!create( W ) )
		FAIL("should throw");
}

WAX::
Xft_Draw::
~Xft_Draw()
{
	destroy();
}

bool
WAX::
Xft_Draw::
create( X_Window & W )
{
	if(draw) {
		WARN("draw should be NULL");
		destroy(); // no leak
	}
	INFO("CALLED");

	Display  *display = W.display;
	Drawable drawable = W.window;

	Visual   *visual;
	Colormap colormap;

	if( display ) {
		visual = DefaultVisual(display, 0);
		colormap = DefaultColormap( display, 0);
	} else {
		draw = NULL;
		return FAIL("NULL displaY");
	}

	draw = XftDrawCreate (display, drawable, visual, colormap);

	if(!draw) return FAIL("XftDrawCreate(...)");
	return true;
}

bool
WAX::
Xft_Draw::
destroy()
{
	if(draw) {
		XftDrawDestroy(draw);
		draw = NULL;

		// should also destroy other resources
		// Destroy made them remotely destroyed
	}
	return true;
}


bool		// type_retval
WAX::		// namespace
Xft_Draw::	// classname
test()		// func(proto)
{

	if(!draw) return FAIL("NULL draw");
	INFO("CALLED");

	XftColor purple;
	const char *name = "purple";
	if(! XftColorAllocName (
		XftDrawDisplay(draw),
		XftDrawVisual(draw),
		XftDrawColormap(draw),
		name,
		&purple 
	)) {
		return FAIL("XftColorAllocName( %s )", name );
	}

#if 0
	// no rotation
	int screen = 0; // get screen from display ?
	XftFont * font = XftFontOpen(
		XftDrawDisplay(draw),
		screen,
		XFT_FAMILY, XftTypeString, "charter",
		XFT_SIZE, XftTypeDouble, 12.0,
		NULL
	);
#endif

#if 1
	double angle = -22; // + up - down
	double font_size = 36; // pt
	const char * font_name = "charter";


	double PI = 3.1415926;
	double s = sin(angle*PI/180.0);
	double c = cos(angle*PI/180.0);
	FcMatrix mat;
	mat.xx = c;
	mat.yy = c;
	mat.yx =  s;
	mat.xy = -s;

	int screen = 0; // get screen from display ?
	XftFont * font = XftFontOpen(
		XftDrawDisplay(draw),
		screen,
		XFT_FAMILY, XftTypeString, font_name,
		XFT_SIZE, XftTypeDouble, font_size,
		XFT_MATRIX, FcTypeMatrix, &mat,
		NULL
	);
#endif

	if(!font) return FAIL("XftFontOpen(...)");

	const char * string = "XFT_String_XFT";
	int len = strlen(string);
	XGlyphInfo extents;
	XftTextExtents8 (
		XftDrawDisplay(draw),
		font,
	  (FcChar8*)
		string,
		len,
		&extents
	);

	XftColor * color = & purple;
	int x = 20;
	int y = 40;
	XftDrawString8 (
		draw,
		color,
		font,
		x,
		y,
	  (FcChar8*)
		string,
		len
	);


	return PASS("return");
	return true;
}
