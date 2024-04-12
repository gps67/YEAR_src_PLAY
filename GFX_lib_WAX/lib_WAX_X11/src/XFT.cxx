#include "XFT.h"
#include <X11/Xft/Xft.h>
#include "X_Window.h"
#include "X_Display.h"

#include "A_matrix_2x2.h"

using namespace WAX;
// using namespace WAX::XFT;

	// see /tools/x86_64_src/tcl/t~7a5/unix/tkUnixRFont.c
	// also beyond ASCII
	// https://tronche.com/gui/x/xlib/display/display-macros.html


WAX::
Xft_Draw::
Xft_Draw()
: draw( NULL )
{
	INFO("requires late init Xft_DrawCreate(W)");
}

WAX::
Xft_Draw::
Xft_Draw( X_Window & W )
: draw( NULL )
{
	if( W.drawable )
	  if(!Xft_DrawCreate( W ) )
		FAIL("should throw");
}

WAX::
Xft_Draw::
~Xft_Draw()
{
	Xft_DrawDestroy();
}

bool
WAX::
Xft_Draw::
Xft_DrawCreate( X_Window & W )
{
	// before draw exists you may not call
	// get_display()
	// get_screen()
	// draw holds all those, in duplicate
	// and colors are also duplicated
	if(draw) {
		WARN("draw should be NULL");
		Xft_DrawDestroy(); // no leak
	}
	INFO("CALLED");

	Display  *display = W.slow_get_display();
	Drawable drawable = W.drawable;

	Visual   *visual;
	Colormap colormap;
	int screen_0 = X_Display_1:: SCREEN_0; // another alias for 0
//	cant call get_screen() here because that uses draw which holds display
//	screen_0 = get_screen();
	screen_0 = X_Display:: SCREEN_0; // another alias for 0
	INFO("screen_0 == %d", screen_0 );

	if( display ) {
		visual = DefaultVisual(display, screen_0 );
		colormap = DefaultColormap( display, screen_0 );
	} else {
		draw = NULL;
		return FAIL("NULL display");
	}

	draw = XftDrawCreate (display, drawable, visual, colormap);
	// get_display() is now a valid thing to call = draw.hidden_field

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
	XftColor & alloc_color,
	const char *name
)
{
	if(! XftColorAllocName (
		XftDrawDisplay(draw),
		XftDrawVisual(draw),
		XftDrawColormap(draw),
		name,
		& alloc_color 
	)) {
		return FAIL("XftColorAllocName( %s )", name );
	}
	return true;
}

bool
WAX::
Xft_Draw::
Xft_ColorFree (
	XftColor & color
)
{
	XftColorFree (
		XftDrawDisplay(draw),
		XftDrawVisual(draw),
		XftDrawColormap(draw),
		& color 	// C PTR to struct
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
	font = XftFontOpen(
		get_display(),
		get_screen(),
		XFT_FAMILY, XftTypeString, font_name,
		XFT_SIZE,   XftTypeDouble, font_size,
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
	font = XftFontOpen(
		get_display(),
		get_screen(),
		XFT_FAMILY, XftTypeString, font_name,
		XFT_SIZE,   XftTypeDouble, font_size,
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

	XftFontClose( get_display(), font );
	font = NULL;
	return true;
}

bool
WAX::
Xft_Pen::
Xft_TextExtents8 (
	// XftDrawDisplay(draw.draw),
	// pen.pen_font,
//  (FcChar8*)
	const char * string,
	int len
	// &pen.pen_extents
) 
{
	if( len < 1 ) 
		len = strlen(string);

	XftTextExtents8 (
		get_display(),
		pen_font,
	(FcChar8*)
		string,
		len,
		&pen_extents
	);
	return true;
}

bool
WAX::
Xft_Pen::
Xft_DrawString8 (
//	draw.draw,
//	pen.pen_color,
//	pen.pen_font,
	int x,
	int y,
//  (FcChar8*)
	const char * string,
	int len
)  {

	if( len < 1 ) 
		len = strlen(string);
	
	if(!pen_color) {
		return FAIL("NULL pen_color");
	}

	if(!pen_font) {
		return FAIL("NULL pen_font");
	}

	XftDrawString8 (
		xft_draw.draw,	// 
		pen_color,
		pen_font,
		x,
		y,
	  (FcChar8*)
		string,
		len
	);
	return true;
}

bool
WAX::	
Xft_Pen::
zero_XGlyphInfo( XGlyphInfo & e ) // init for debugging
{
	e.x = e.y = 0;
	e.width = e.height = 0;
	e.xOff = e.yOff = 0;
	return true;
}

bool		// type_retval
WAX::		// namespace
Xft_Pen::	// classname
show_XGlyphInfo( const XGlyphInfo & e ) // Xrender.h
{
	/*
		caller remembers initial P0 == START on screen
			that is the BASELINE
			that is the XLEFT of first char
			nb BBOX might be after P0.x
			nb BBOX might be before P0.x
			eg Y sloping upwards top starts before P0.x
			eg Y sloping upwards bottom starts above P0.y
		P2 == P0 + xyOff == START of next SP along baseline
		BBOX WH == size of BBOX - tightest fitting BBOX
		xy == BBOX_START
		xy == START relative to TOP_LEFT of BBOX
		BBOX XY == SCREEN_START - (xy)
	*/
	buffer1 buf;
	buf.print( "width %4d height %4d BBOX WH  \n", e.width, e.height );
	buf.print( "x     %4d y      %4d BASELINE START from top left\n", e.x    , e.y     );
	buf.print( "xOff  %4d yOff   %4d ADVANCE wrt START\n", e.xOff , e.yOff  );
	e_print( "%s", (STR0) buf );
	return true;
}


////// MY_XFT //// is the APP not the LIBR /// it owns all the bits

bool
WAX::
MY_XFT::
alloc_items() // color angle matrix font
{
	const char *color_purple_name = "purple";
	double angle = -22; // + up - down
	double font_size = 36; // pt
	const char * font_name = "charter";
	angle = +90;
	angle = +23; // any angle works + is up l2r
	angle = +45;
	font_size = 18;
	font_size =  8;
	font_size = 28;
	test_x = 100;
	test_y = 300;

	// color //
	if(! draw.Xft_ColorAllocName ( color_purple, color_purple_name )) {
		return FAIL_FAILED();
	}

	// angle //
	A_matrix_2x2  font_matrix; // rotation scale shear
	font_matrix.set_to_rotate( angle ); // degrees

	// font //
	if(! draw.Xft_FontOpen(
		  font_charter,	// VAR // & of PTR to //
		  font_name,
		  font_size,
		& font_matrix	// used not retained
	))
		return FAIL_FAILED();

	// assign pen_ pointers to allocated items
	pen.pen_font = font_charter;
	pen.pen_color = & color_purple;

	if(!draw.draw) return FAIL("NULL draw");
	return true;
}

bool
WAX::
MY_XFT::
release_items()
{
	// OK the pen is pointers to preallocated items
	// some thought required
	if(!draw.draw) return FAIL("NULL draw");
	if(! draw.Xft_ColorFree ( color_purple )) { // ? pen.pen_color ?
		return FAIL_FAILED();
	}
	if(! draw.Xft_FontClose( font_charter )) // ? pen.pen_font ?
		return FAIL_FAILED();
	return true;
}

bool		// type_retval
WAX::		// namespace
MY_XFT::	// classname
test_redraw()	// func(proto)
{

	// check init was OK
	if(!draw.draw) return FAIL("NULL draw");

	// app might keep things for a long time
	// errm this encodes angle into the font alloc
	alloc_items();

	// assign pen_ pointers to allocated items
	pen.pen_font = font_charter;
	pen.pen_color = & color_purple;

	// initial Y shows BBOX is BBOX
	//
	const char * string = "Y_XFT_String_XFT";
	int len = strlen(string);

	pen.Xft_TextExtents8 (
		string,
		len
	);
//	show_XGlyphInfo( pen_extents );
	pen.Xft_DrawString8 (
		test_x,
		test_y,
		string,
		len
	);


	release_items();
	return PASS("return");
	return true;
}

//////

// CTOR
WAX::		// namespace
MY_XFT::	// classname
MY_XFT( X_Window & W )
: draw( W )
, pen( draw )
{
}

WAX::
MY_XFT::
~MY_XFT()
{
	INFO("DTOR");
}

