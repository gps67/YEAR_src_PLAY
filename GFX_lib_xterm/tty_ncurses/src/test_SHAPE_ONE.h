#ifndef test_SHAPE_ONE_H
#define test_SHAPE_ONE_H
// too many for TOP API // edit as if for STUBS // TODO
// #include <ncurses.h>
// #include "tty_curses.h"
// #include "tty_curses_CSR.h"
// #include "dgb.h"
// #include "e_print.h"
// #include <stdio.h>
// #include <locale.h>

#include "test_SHAPE_BASE.h"
// #include "str1.h"

// namespace TTY_CURSES {
//	user of namespace INTENTIONALLY NOT IN namespace,
//	as if from api callers view, demo caller
//	DITTO Makefile can have users invokation + _include _link _load _call
// }; // namespace

 using namespace TTY_CURSES;

  struct SHAPE_ONE : public SHAPE_BASE {

	// SURFACE is CSR
	tty_curses_CSR & CSR; // shared with other users of window

	SHAPE_ONE( tty_curses_CSR & _CSR,  STR0 title= "SHAPE_ONE" )
	: SHAPE_BASE( title )
	, CSR( _CSR )
	{
	}

	// needs missing metrics // bool recalc_dx_to_box();

	bool Layout( tty_curses_CSR & _CSR )
	{
		WARN("attempt to draw on external CSR");
		return Layout( CSR );
	}

	bool Layout()
	{
		return Layout( CSR );
	}


	virtual 
	bool set_XYWH( XYWH_t & _XYWH );

	i16 frame_W() { return XYWH.WH.W; }
	i16 frame_H() { return XYWH.WH.H; }

	virtual
	bool draw( tty_curses_CSR & CSR ); // shared with other users of window
}; // class
#endif
