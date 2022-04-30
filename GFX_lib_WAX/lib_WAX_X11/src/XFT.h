#ifndef XFT_H
#define XFT_H

#include "X_STUBS.h"
#include "X_ret_err.h"

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

	bool create( X_Window & W );
	bool destroy();

	// parameter order - moving RETVAR to first

	bool Xft_ColorAllocName (
		XftColor * colour_purple,
		const char *name // "purple"
	);

	bool Xft_FontOpen( 
		XftFont ** font,
		const char * font_name,
		double font_size,	// pt
		A_matrix_2x2 * matrix	// rotation scale shear
	);

	bool Xft_FontOpen( 		// no matrix
		XftFont ** font,
		const char * font_name,
		double font_size	// pt
	);



	bool test();

	XGlyphInfo extents;
	bool show_XGlyphInfo( const XGlyphInfo & extents );

};

// } // namespace
} // namespace

#endif


