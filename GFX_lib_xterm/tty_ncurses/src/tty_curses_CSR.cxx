#include "tty_curses_UDLR.h"
#include "tty_curses_CSR.h"

#include <ncurses.h>
#include "gdb_invoke.h"

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
		vw_printw( win, fmt, args );
	}

	void tty_curses_CSR:: print( const char * fmt, ... )
	{
		va_list args;
		va_start( args, fmt ); // fmt is the position on the stack
		vw_printw( win, fmt, args );
	}

	void tty_curses_CSR:: puts( const char * str ) // not yet utf8
	{
		waddstr( win, str );
	}

	void tty_curses_CSR:: putc_byte( const u8 ch )
	{
		waddch( win, (chtype) (char) ch );
	}
	void tty_curses_CSR:: putc_acs( const u8 ch )
	{
		if(!ch)
		 gdb_break_point();
		if(!ch)
		 puts("0x00");
		waddch( win, (chtype) (char) ch );
	}

	void tty_curses_CSR:: add_ch( const chtype ch )
	{
		waddch( win, ch );
	}

	void tty_curses_CSR:: clear_to_eol()
	{
		wclrtoeol( win );
	}

	void tty_curses_CSR:: box_mode_start()
	{
	}

	void tty_curses_CSR:: box_mode_end()
	{
	}

	void tty_curses_CSR:: box_v_line( i16 x, i16 y1, i16 y2 )
	{
		 move( y1, x );
		 wvline( win, 0, (y2-y1) );
		 move( y2, x );
		 return;
	}

	void tty_curses_CSR:: box_h_line( i16 y, i16 x1, i16 x2 )
	{
		// man 3 curs_border
		move( y, x1 );
		whline( win, 0, (x2-x1) );
		move( y, x2 );
		return;
	}

	void tty_curses_CSR:: putc_box( enum_UDLR udlr )
	{
		UDLR UDLR(udlr);
		putc_acs( UDLR.get_acs_char() );
		if(udlr == UDLR_____)
		 puts("0x00");
	}

	void tty_curses_CSR:: putc_box( UDLR UDLR)
	{
		putc_acs( UDLR.get_acs_char() );
	}

	void tty_curses_CSR:: move_status_line() // and clear it
	{
		wmove( win, LINES - 1, 1 );
		box( win, 0, 0 );
		// wclrtoeol( win );
	}

	int tty_curses_CSR:: get_ch()
	{
		return
		wgetch( win );
	}


};

