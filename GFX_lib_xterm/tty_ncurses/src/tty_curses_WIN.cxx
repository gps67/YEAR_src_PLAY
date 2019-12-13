#include "tty_curses_STUBS.h"
// #include "tty_curses_UDLR.h"
#include "tty_curses_WIN.h"

#include <ncurses.h>
#include "gdb_invoke.h"
#include "dgb.h" // 


namespace TTY_CURSES {


	tty_curses_WIN::
	tty_curses_WIN( 
		tty_curses * _TTY_curses // PARENT
	)
	: TTY_curses(_TTY_curses)
	, win(NULL)
	{
		win = stdscr;
	}

	void tty_curses_WIN:: set_stdscr( WINDOW * W  ) {
		if( ! W ) W = stdscr;
		win = W;
	}

	bool tty_curses_WIN:: get_WH( WH_t & _WH )
	{
		_WH.W = COLS;
		_WH.H = LINES;
		return true;
	}

	bool tty_curses_WIN:: get_W_H( int & _W, int & _H )
	{
		_W = COLS;
		_H = LINES;
		return true;
	}

	void tty_curses_WIN:: get_yx_was()
	{
		getyx( // ncurses macro
			win,
			XY_was.Y,
			XY_was.X
		);
	}

	// reports bad XY and clips a bit
	void tty_curses_WIN:: pre_MOVE( XY_t & XY )
	{
	return;
		WH_t WH;
		get_WH( WH );

		XY_t XY_bad( 3, 15 );
			wmove( win, XY_bad.Y, XY_bad.X );
			printf("(%d,%d) ", XY.X, XY.Y );

		if( XY.Y < 0 ) {
			wmove( win, XY_bad.Y+1, XY_bad.X );
			printf("(%d,%d) ", XY.X, XY.Y );
			XY.Y = 0;
		}
		if( XY.Y > WH.H ) {
			wmove( win, XY_bad.Y, XY_bad.X );
			printf("(%d,%d) ", XY.X, XY.Y );
			XY.Y = WH.H;
		}
	}
	
	void tty_curses_WIN:: move( const XY_t & XY )
	{
		XY_t XY_adj = XY;
		pre_MOVE( XY_adj );
		wmove( win, XY_adj.Y, XY_adj.X );
		XY_was = XY; // lost by printf or addch or ...
	}

	void tty_curses_WIN:: move_to_X( X_t X )
	{
		get_yx_was();
		XY_t XY;
		XY.Y = XY_was.Y;
		XY.X = X;
		move(XY);
	}

	void tty_curses_WIN:: move_to_Y( Y_t Y )
	{
		get_yx_was();
		XY_t XY;
		XY.X = XY_was.X;
		XY.Y = Y;
		move(XY);
	}

	void tty_curses_WIN:: move_right( X_t X ) {
		get_yx_was();
		XY_t XY = XY_was;
		XY.X += X;
		move(XY);
	}

	void tty_curses_WIN:: move_down( Y_t Y ) {
		get_yx_was();
		XY_t XY = XY_was;
		XY.Y += Y;
		move(XY);
	}


	void tty_curses_WIN:: printf( const char * fmt, ... )
	{
		va_list args;
		va_start( args, fmt ); // fmt is the position on the stack
		vw_printw( win, fmt, args );
	}

	void tty_curses_WIN:: print( const char * fmt, ... )
	{
		va_list args;
		va_start( args, fmt ); // fmt is the position on the stack
		vw_printw( win, fmt, args );
	}

	void tty_curses_WIN:: puts( const char * str ) // not yet utf8
	{
		waddstr( win, str );
	}

	void tty_curses_WIN:: putc_byte( const u8 ch )
	{
		waddch( win, (chtype) (char) ch );
	}

	void tty_curses_WIN:: putc_acs( const chtype ch )
	{
		if(!ch)
		 gdb_break_point();
		if(!ch)
		 puts("0x00");
		waddch( win, (chtype) ch );
	}


	void tty_curses_WIN:: add_ch( const chtype ch )
	{
		waddch( win, ch );
	}

	void tty_curses_WIN:: clear_to_eol()
	{
		wclrtoeol( win );
	}

	void tty_curses_WIN:: move_status_line() // and clear it
	{
		WH_t WH;
		get_WH( WH );

		XY_t POS( 1, WH.H-1 );
		move( POS );
		if(0) box( win, 0, 0 );
		// wclrtoeol( win );
	}

	int tty_curses_WIN:: get_ch()
	{
		return
		wgetch( win );
	}


	void tty_curses_WIN:: nnpp(u8 lbl)
	{
		return; // silent
		static int nn = 0;
		nn++;
		if(nn>26) nn=0;
		putc_byte( lbl );
		return; // only show LBL
		putc_byte( '@' + nn );
	}


};
