#include "XFT.h"
#include <X11/Xft/Xft.h>
#include "X_Window.h"

#include "A_matrix_2x2.h"

using namespace WAX;
// using namespace WAX::XFT;

WAX::
Xft_Draw::
Xft_Draw()
: draw( NULL )
, pen_font( NULL )
{
	INFO("requires late init Xft_DrawCreate(W)");
}

WAX::
Xft_Draw::
Xft_Draw( X_Window & W )
: draw( NULL )
, pen_font( NULL )
{
	if( W.window )
	  if(!Xft_DrawCreate( W ) )
		FAIL("should throw");
}

WAX::
Xft_Draw::
~Xft_Draw()
{
	if(pen_font) Xft_FontClose(pen_font);
	Xft_DrawDestroy();
}

bool
WAX::
Xft_Draw::
Xft_DrawCreate( X_Window & W )
{
	if(draw) {
		WARN("draw should be NULL");
		Xft_DrawDestroy(); // no leak
	}
	INFO("CALLED");

	Display  *display = W.display;
	Drawable drawable = W.window;

	Visual   *visual;
	Colormap colormap;
	int screen_0 = 0; // I think this is OK

	if( display ) {
		visual = DefaultVisual(display, screen_0 );
		colormap = DefaultColormap( display, screen_0 );
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
Xft_DrawDestroy()
{
	if(draw) {
		XftDrawDestroy(draw);
		draw = NULL;

		// should also destroy other resources
		// Destroy made them remotely destroyed
	}
	return true;
}


bool
WAX::
Xft_Draw::
Xft_ColorAllocName (
	XftColor & colour_purple,
	const char *name
)
{
	if(! XftColorAllocName (
		XftDrawDisplay(draw),
		XftDrawVisual(draw),
		XftDrawColormap(draw),
		name,
		& colour_purple 
	)) {
		return FAIL("XftColorAllocName( %s )", name );
	}
	return true;
}

bool
WAX::
Xft_Draw::
Xft_ColorFree (
	XftColor & colour
)
{
	XftColorFree (
		XftDrawDisplay(draw),
		XftDrawVisual(draw),
		XftDrawColormap(draw),
		& colour 	// C PTR to struct
	);
	// return FAIL("XftColorFree()" );
	return true;
}

// TODO: BOLD ITALIC // maybe use this class as attribute collection

bool
WAX::
Xft_Draw::
Xft_FontOpen( 
	XftFont *& font,	// ret_var is a pointer 
	const char * font_name,
	double font_size,
	A_matrix_2x2 * matrix
) {
	if(font) Xft_FontClose(font);
	Display * display = XftDrawDisplay(draw);
	int screen = DefaultScreen( display );	// options for ...
	font = XftFontOpen(
		display,
		screen,
		XFT_FAMILY, XftTypeString, font_name,
		XFT_SIZE, XftTypeDouble, font_size,
		XFT_MATRIX, FcTypeMatrix, &matrix->matrix,
		NULL
	);
	if(!font) return FAIL("font_name %s size %4.1f", font_name, font_size);
	return true;
}

bool
WAX::
Xft_Draw::
Xft_FontOpen( 
	XftFont *& font,	// ret_var is a pointer 
	const char * font_name,
	double font_size
) {
	if(font) Xft_FontClose(font);
	Display * display = XftDrawDisplay(draw);
	int screen = DefaultScreen( display );	// options for ...
	font = XftFontOpen(
		display,
		screen,
		XFT_FAMILY, XftTypeString, font_name,
		XFT_SIZE, XftTypeDouble, font_size,
		NULL
	);
	if(!font) return FAIL("font_name %s size %4.1f", font_name, font_size);
	return true;
}

bool
WAX::
Xft_Draw::
Xft_FontClose( 
	XftFont *& font	// ret_var is a pointer 
) {
	if(!font) return PASS("font was already NULL");

	Display * display = XftDrawDisplay(draw);
	XftFontClose( display, font );
	font = NULL;
	return true;
}

bool		// type_retval
WAX::		// namespace
Xft_Draw::	// classname
test()		// func(proto)
{

	if(!draw) return FAIL("NULL draw");
	INFO("CALLED");

	XftColor colour_purple;
	const char *name = "purple";
	if(! Xft_ColorAllocName ( colour_purple, name )) {
		return FAIL_FAILED();
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

	// see /tools/x86_64_src/tcl/t~7a5/unix/tkUnixRFont.c
	// also beyond ASCII

	double angle = -22; // + up - down
	double font_size = 36; // pt
	const char * font_name = "charter";



	A_matrix_2x2 matrix;
	matrix.set_to_rotate( angle ); // degrees

	// https://tronche.com/gui/x/xlib/display/display-macros.html
//	Display * display = XftDrawDisplay(draw);
//	int screen = DefaultScreen( display );	// options for ...

	XftFont * font = NULL;
	if(! Xft_FontOpen( font, font_name, font_size, & matrix ))
		return FAIL_FAILED();

	const char * string = "XFT_String_XFT";
	int len = strlen(string);

//	XGlyphInfo extents;
	XftTextExtents8 (
		XftDrawDisplay(draw),
		font,
	  (FcChar8*)
		string,
		len,
		&pen_extents
	);
//	show_XGlyphInfo( pen_extents );

	XftColor * color = & colour_purple;
	int x = 100;
	int y = 100;
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

//////

bool		// type_retval
WAX::		// namespace
Xft_Draw::	// classname
show_XGlyphInfo( const XGlyphInfo & e ) // Xrender.h
{
	buffer1 buf;
	buf.print( "width %4d height %4d\n", e.width, e.height );
	buf.print( "x     %4d y      %4d\n", e.x    , e.y     );
	buf.print( "xOff  %4d yOff   %4d\n", e.xOff , e.yOff  );
	e_print( "%s", (STR0) buf );

	return true;
}