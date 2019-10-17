#ifndef X_FontStruct_H
#define X_FontStruct_H

// We rely on this having been included already ?
// #include "X_STUBS.h"

namespace WAX {

 struct X_FontStruct :

	Display * display;
	XFontStruct * font;

// COPY CTOR
	X_FontStruct( X_FontStruct & rhs ) 
	: display( rhs.display )
	, font( rhs.font )
	{
		// home it wasnt NULL
	}

// init0 CTOR
	X_FontStruct() : display(NULL), font(NULL) {}

/*
	X_FontStruct( Display * display, XFontStruct * F )
	: display( display )
	: font( F )
	{
	}

	X_FontStruct( Display * display, XFontStruct * F )
	: display( display )
	: font( F )
	{
	}
*/

	bool XLoadQueryFont(
		Display * disp,
		const char * str 
	)
	{
		font = ::XLoadQueryFont( disp, str );
		
		
	}

	

 };

};

#endif

