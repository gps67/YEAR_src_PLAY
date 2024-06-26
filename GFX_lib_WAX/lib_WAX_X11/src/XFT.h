#ifndef XFT_H
#define XFT_H

#include "X_STUBS.h"
#include "X_ret_err.h"
#include "A_matrix_2x2.h"

#include <X11/Xft/Xft.h> // XGlyphInfo

// XFT_STUBS
// struct XftDraw;
typedef struct _XftDraw XftDraw;
typedef struct _XGlyphInfo XGlyphInfo;
typedef struct _XftColor XftColor;
typedef struct _XftFont XftFont;
namespace WAX {
struct A_matrix_2x2;	// matches Xft matrix to be used ASIS
}

/*
	XFT
#include <X11/Xft/Xft.h> // XGlyphInfo

	Release (AllocatedItem) requires Display * 
	free(mem) needs TheHeap * (as global vars)
	Either each Item must store Display (etc) 
	OR a resource manager must release them
	
	It is simpler to create independent Items
	that also hold their own Display (etc)

	Ditto for Items that need parameters to DRAW

	but it is smaller to not do that

	The complexity has already been chosen by the X11 people,
	and they chose lots of scattered parameters,
	joining them as parameters to the same functions.

	The complexity of writing independent Items everywhere
	does not make sense (without going completely independent),
	and then it makes sense to not even start to try.

	Instead, I am putting everything in MY_XFT
	which holds the main vars that allocates items.
	MY_XFT also holds lots of individual Items,
	and it has to know to release them.

	The X11 Xft module seems to repeat stuff, eg colours. WHY ?
	IT makes more sense to have all the non-xft items (eg colours)
	manage them in the usual way, and use them in Xft calls

*/

namespace WAX {

// namespace XFT {
/*!
	X_Draw is the basic drawing context (with GC)

	Xft_Draw is specific to Xft

	I'm not happy with this layout, as it creates a GC,
	maybe it should hold a GC created elsewhere.

		GC contains clipping ? - not sure, it can do, does it?
		GC can be shared within display - yes - bit clipping!
		GC fragments and combinations
*/
struct Xft_Draw
{
	XftDraw * draw;

	Xft_Draw();
	Xft_Draw( X_Window & W );
	~Xft_Draw();

	Display * get_display() {
		return XftDrawDisplay(draw);
	}
	int get_screen() {
		return DefaultScreen( get_display() );
	}

	bool Xft_DrawCreate( X_Window & W );
	bool Xft_DrawDestroy();

	// parameter order - moving RETVAR to first

	bool Xft_ColorAllocName (
		XftColor & color, // retval is struct initialised
		const char *name // "purple"
	);
	bool Xft_ColorFree (
		XftColor & color // retval
	);

	bool Xft_FontOpen( 
		XftFont *& font,
		const char * font_name,
		double font_size,	// pt
		A_matrix_2x2 * matrix	// rotation scale shear
	);

	bool Xft_FontOpen( 		// no matrix
		XftFont *& font,
		const char * font_name,
		double font_size	// pt
	);

	bool Xft_FontClose( 
		XftFont *& font		// ret_var is cleared //  pointer 
	);

	// now THIS acts as a PEN GC with ink
	// Draw + Font //

	// one Draw can have many fonts, then many XftColors
	// but we only really want one rotated text pen

};

struct Xft_Pen { // a bit like a GC or a PrintHead

	Xft_Draw & xft_draw;

	Display * get_display() { return xft_draw.get_display(); }
	// this needs the full declatation of Xft_Draw
	//	return XftDrawDisplay(xft_draw.draw);

//	A_matrix_2x2  pen_matrix; // rotation scale shear
//	str1 pen_font_name;
//	double pen_font_size;

	// owner of pen owns loans sets these // must do so
	XftColor * pen_color; //  pointer to color_purple
	XftFont * pen_font;

	// measure instead of drawing
	XGlyphInfo pen_extents; // unused avail for any temp use

	bool show_XGlyphInfo( const XGlyphInfo & extents );
	bool zero_XGlyphInfo( XGlyphInfo & e );

	bool Xft_TextExtents8 (
	//  (FcChar8*)
		const char * string,
		int len
	);

	bool Xft_DrawString8 (
		int x,
		int y,
	//  (FcChar8*)
		const char * string,
		int len
	);

	bool measure_str( const char * str, int len = -1 ) {
		return Xft_TextExtents8( str, len );
	}
	bool xy_str( int x, int y, const char * str, int len = -1 ) {
		return Xft_DrawString8( x, y, str, len ); // calc strlen
	}

	Xft_Pen(
		Xft_Draw & _xft_draw
	)
	: xft_draw(  _xft_draw )
	, pen_color( NULL )
	, pen_font( NULL )
	{
		zero_XGlyphInfo( pen_extents );
	}

	~Xft_Pen()
	{
		// pen does not hold much
		// caller provides the components
		// and holds them whilst pen is in use
	}

};

#if 0
struct XXX_Xft_Font // just use raw XftFont * font
#endif

struct MY_XFT {	// owns all the resources // loans them to Pen

	// Pallette

	// HMMM // MY_XFT should also hold the ALLOC RELEASE DATA
	// base class holds display colormap etc
	// derived class adds as many fonts as it needs

	MY_XFT( X_Window & W );
	~MY_XFT();

	Xft_Draw draw;
	Xft_Pen pen;	// there is no such thing , a collection of items
	// on each text draw you might set:
	// pen.pen_color
	// pen.pen_font;

	bool alloc_items();
	bool release_items();

	// this owns these 
	XftFont * font_charter; // specifically the angled one
	XftColor color_purple; // 

	int test_x; // = 100;
	int test_y; // = 100;
	bool test_redraw(); // Xft_Pen & pen );
};

// } // namespace
} // namespace

#endif


