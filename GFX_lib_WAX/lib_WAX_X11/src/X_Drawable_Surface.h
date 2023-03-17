#ifndef X_Drawable_Surface_H
#define X_Drawable_Surface_H

#include "X_STUBS.h"
#include "A_point_plus.h"

#include <X11/Xlib.h>

#include "XFT.h"
// PROBLEM IS
// we include X_Display.h
// it includes X_Window.h
// it includes X_Drawable_Surface.h // and is based on it
// so move that code to .cxx to use disp->fields
//
//#include "X_Display.h"

// SO NO // #include "X_Display.h"
// DIALECT "{ %s }" VALUE_1 = "X_Display.h"

// SO use LIBR // X_Display & disp // route to { Display * display }
// DIALECT Cident * cident = VALUE_FROM_GOT // as in GETTER not GOT // BIND VAR
// cident == EXPR // data stored
// EXPR == "VALUE" // MATCH EXPR
// EXPR == "EXPR" // MATCH VALUE
// EXPR == "{ PARSED SOURCE }" // tokenised and MATCH and GEN // SCRIPT VALS //
// VALS += VARNAME VAGUE_TYPE SPECIFIC_API 

// so use STUBS.h // X_Display * PTR // API 

namespace WAX {

struct X_Drawable_Surface { // base of X_Window X_Pixmap // own spin

	X_Display * disp; // refactored as X_Display -> get_display()
	Display * slow_get_display();

		// RETVAL is SCRIPT { SCRIPT }
		// PSG recognises ABOVE line //
		// MATCH MODULE SESS FILTER DETECT //
		// EXPR "{ RETVAL STR0 }" // TEXT MUX STR0 AUTO_UTF8 
		// AUTO_ASCII // AUTO_UTF8 // AUTO_LATIN // CSET_USED  //
		// NAME_cident_varient //
		// https://youtu.be/7kxUjkenn9E?t=80
		// "{ 't' 'tokenised' 'args_expr args' }" 
// "{ a r g s }" // extra info // VAR_POOL SPEC 
		// "{ t 18:00 }" FOLD FOUND  " += VAR TYPE=SPEC NAME SESS"
		// 
	
	Drawable drawable; // caller creates drawable then this holds it
	A_WH WH; // FRAME XY WH // WINDOW // PANEL // 

	#if 1 // permit simple prototype sketch design
	X_Drawable_Surface(): disp(NULL), drawable(0) {}
	#endif
	X_Drawable_Surface( int temp_skim ) {}

/*
	X_Drawable_Surface(
		Display * _display,
	//	X_Display * _disp,	// move to keeping shared PTR 
		Drawable _drawable,
		A_WH _WH
	)
	:	display( _display )
	,	drawable( _drawable )
	,	WH( _WH )
	{
	}
*/

	X_Drawable_Surface(
		X_Display * _disp,	// move to keeping shared PTR 
		Drawable _drawable
	)
	:	disp( _disp )
	,	drawable( _drawable )
	,	WH( 0,0 )
	{
	}

	X_Drawable_Surface(
		X_Display * _disp,	// GEN at FULL_LIST // AUTO check FULL
		Drawable _drawable,
		A_WH _WH
	)
	:	disp( _disp )
	,	drawable( _drawable )
	,	WH( _WH )
	{
	}

	bool check_drawable()
	{
		if(drawable) {
			return true;
		} else {
			WARN("ZERO drawable");
			DEBUG_print_stack();
			return false;
		}
	}

	/*!
		create a GC for this drawable
	*/
	GC CreateGC()
	{
		if(!check_drawable()) return 0;

		unsigned long valuemask = 0;
		XGCValues * values = NULL;
		return ::XCreateGC( slow_get_display(), drawable, valuemask, values );
	}



	void get_WH( A_WH & _WH ) { _WH = WH; }


};

}; // namespace
#endif
