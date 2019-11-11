#include "tty_curses_CSR.h"
#include <ncurses.h>

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
	: TTY_curses(_TTY_curses)
	, win(NULL)
	{
		win = stdscr;
	}

	void tty_curses_CSR:: set_stdscr( WINDOW * W  ) {
		if( ! W ) W = stdscr;
		win = W;
	}


	void tty_curses_CSR:: 
	get_yx_was() {
		getyx(
			win,
			y_was,
			x_was
		);
	}

	void tty_curses_CSR:: printf( const char * fmt, ... )
	{
		va_list args;
		va_start( args, fmt ); // fmt is the position on the stack
		vwprintw( win, fmt, args );
	}

	void tty_curses_CSR:: clear_to_eol()
	{
		wclrtoeol( win );
	}

	void tty_curses_CSR:: move_status_line() // and clear it
	{
		wmove( win, LINES - 1, 1 );
		box( win, 0, 0 );
		// wclrtoeol( win );
	}

	int tty_curses_CSR:: get_ch()
	{
		wgetch( win );
	}


};

