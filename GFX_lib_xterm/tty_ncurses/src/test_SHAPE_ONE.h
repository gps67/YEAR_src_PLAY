#ifndef test_SHAPE_ONE_H
#define test_SHAPE_ONE_H
// too many for TOP API // edit as if for STUBS // TODO
#include <ncurses.h>
#include "tty_curses.h"
#include "tty_curses_CSR.h"
#include "dgb.h"
#include "e_print.h"
#include <stdio.h>
#include <locale.h>
#include "str1.h"

// namespace TTY_CURSES {
//	user of namespace INTENTIONALLY NOT IN namespace,
//	as if from api callers view, demo caller
//	DITTO Makefile can have users invokation + _include _link _load _call
// }; // namespace

 using namespace TTY_CURSES;

  struct SHAPE_ONE {

	// SURFACE is
	tty_curses_CSR & CSR; // shared with other users of window
  	i16 X0; 
  	i16 Y0; 

	str1 title;
	i16 glyph_width; // dpi=4

	i16 dx_to_box;
	i16 dx_of_frame;
	i16 dy_of_frame;

	SHAPE_ONE( tty_curses_CSR & _CSR )
	: CSR( _CSR )
	{
		// title = "TitleTwo";
		title = "AB";
	}



	bool set_X_Y_W_H(
	 i16 _X, 
	 i16 _Y, 
	 i16 _W, 
	 i16 _H
	) ; /*
		// EACH class will  add stuff like this

		X0 = _X;
		Y0 = _Y;
		dx_of_frame = _W;
		dy_of_frame = _H;

		dx_to_box = 2;
		dx_to_box = 7;

		glyph_width = title.str_len();
		return true;
	}
	*/
	bool draw();
}; // class
#endif
