#ifndef test_SHAPE_BASE_H
#define test_SHAPE_BASE_H
// too many for TOP API // edit as if for STUBS // TODO
// #include <ncurses.h>
// #include "tty_curses_CSR.h"
// #include "e_print.h"
// #include <stdio.h>
// #include <locale.h>
// // #include "test_SHAPE_BASE.h"

#include "tty_curses_STUBS.h" // as all do
#include "XYWH_16.h" // 16 bit coord system
#include "str1.h" // title
// #include "tty_curses.h"
#include "dgb.h" // FAIL


// namespace TTY_CURSES {
//	user of namespace INTENTIONALLY NOT IN namespace,
//	as if from api callers view, demo caller
//	DITTO Makefile can have users invokation + _include _link _load _call
// }; // namespace

 using namespace XYWH_16;
 using namespace TTY_CURSES;

  struct SHAPE_BASE { 

  	// i16 X0, Y0;
	// i16 W, H;
	// XY_t X0Y0;
	// WH_t WH;

	XYWH_t XYWH;
	str1 title;
	i16 title_glyph_width; // CSR = SURFACE REQUIRED == GLOBAL isa_TTY

	SHAPE_BASE( STR0 _title = "SHAPE_BASE" )
	: title()
	, title_glyph_width(0)
	{
		set_title( _title ); // CSR // ?? // model -vs- view layers
	}

	bool set_title( str0 _title )
	{
		title = _title;
		title_glyph_width = title.str_len(); // not without SURFACE
		return true;
	}

	virtual
	bool Layout( tty_curses_CSR & CSR ) // SURFACE METRICS dpi etc
	{
		return FAIL("TODO");
	}

	virtual
	bool Layout() // NO CSR required to (re) compute Layout
	{
		return FAIL("TODO"); // subclass must define  
		return INFO("OK");
	}

	virtual bool Layout_required() {
		return Layout();
		// or keep a POINTER to CTRL that we ask to call
		// it adds Layour( _CSR )
	}

	virtual 
	bool set_XYWH( XYWH_t _XYWH );

	virtual
	bool draw( tty_curses_CSR & CSR ) = 0 ;
  	
}; // class
#endif

