#ifndef test_SHAPE_ONE_H
#define test_SHAPE_ONE_H
// too many for TOP API // edit as if for STUBS // TODO
#include "test_SHAPE_BASE.h"
// #include <ncurses.h>
// #include "tty_curses.h"
// #include "tty_curses_CSR.h"
// #include "dgb.h"
// #include "e_print.h"
// #include <stdio.h>
// #include <locale.h>
// #include "str1.h"

// namespace TTY_CURSES {
//	user of namespace INTENTIONALLY NOT IN namespace,
//	as if from api callers view, demo caller
//	DITTO Makefile can have users invokation + _include _link _load _call
// }; // namespace

 using namespace TTY_CURSES;

  struct SHAPE_ONE : public SHAPE_BASE {

	// SURFACE is
	tty_curses_CSR & CSR; // shared with other users of window

	i16 dx_to_box;
	i16 dx_of_frame;
	i16 dy_of_frame;

	SHAPE_ONE( tty_curses_CSR & _CSR )
	: CSR( _CSR )
	{
		// title = "TitleTwo";
		title = "AB";
	}



	virtual
	bool set_XYWH( XYWH_t & _XYWH );
	virtual
	bool draw( tty_curses_CSR & CSR );
}; // class
#endif
