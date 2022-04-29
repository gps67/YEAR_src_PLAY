#ifndef XFT_H
#define XFT_H

// #include "X_STUBS.h"
#include "X_ret_err.h"

// XFT_STUBS
// struct XftDraw;
typedef struct _XftDraw XftDraw;


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

	bool Create();
};

// } // namespace
} // namespace

#endif


