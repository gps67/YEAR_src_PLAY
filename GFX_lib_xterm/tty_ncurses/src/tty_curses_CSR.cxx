#include "tty_curses_STUBS.h"
#include "tty_curses_UDLR.h"
#include "tty_curses_CSR.h"

#include <ncurses.h>
#include "gdb_invoke.h"
#include "dgb.h" // 

/*
	_CSR is a WINDOW maybe stdsrc

	_CSR _FGBD colour_pair attrs ACS_LINE ACS_UTFX

	_CSR POS_PUTS( XY_3,  str ); //
	_CSR clear_upto_eoln // subwin boundary / flags

	_SUB_WINDOW
	_SURFACE_SCROLL
	_SURFACE_PAGED_LINKED
	_SURFACE_SCREEN		// AREA = _SUBZONE_SCREEN & _SUBZONE_SURFACE
	_SUBZONE_FRAME		// widget frames tiles or overlapping maybe
	_SUBZONE_FRAME		// generic tree_node, already has lists _SUB_
*/

namespace TTY_CURSES {

	tty_curses_CSR::
	tty_curses_CSR( 
		tty_curses * _TTY_curses // PARENT
	)
	: tty_curses_WIN_BOX(_TTY_curses)
	{
	}

};
