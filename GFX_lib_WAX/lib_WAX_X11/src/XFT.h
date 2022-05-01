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
struct A_matrix_2x2;
}

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

	bool Xft_DrawCreate( X_Window & W );
	bool Xft_DrawDestroy();

	// parameter order - moving RETVAR to first

	bool Xft_ColorAllocName (
		XftColor & colour, // retval is struct initialised
		const char *name // "purple"
	);
	bool Xft_ColorFree (
		XftColor & colour // retval
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

#if 0
	// also I think the logic is wrong
	// nothing is allocated as PTR
	// caller owns memory, INIT by func

	XftColor colour; // a struct as the default ?

	bool Xft_ColorAllocName (
		const char *name // "purple"
	) { return Xft_ColorAllocName ( colour, name ); }
#endif

	// one Draw can have many fonts, then many XftColors
	// but we only really want one rotated text pen

	A_matrix_2x2 pen_matrix; // rotation scale shear
	str1 pen_font_name;
	double pen_font_size;
	XftFont * pen_font;	// struct belongs to Xft ??

	XGlyphInfo pen_extents; // unused avail for any temp use


	bool test();

	bool show_XGlyphInfo( const XGlyphInfo & extents );

};

// } // namespace
} // namespace

#endif


