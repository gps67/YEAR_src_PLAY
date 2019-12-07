#ifndef tty_curses_CSR_H
#define tty_curses_CSR_H

#include "tty_curses_STUBS.h"
#include <stdarg.h> // 
#include "tty_curses.h"
#include "tty_curses_UDLR.h"

#ifndef NULL
#define NULL 0L
#endif

#if 0
 struct screen;
 typedef struct screen SCREEN;
#endif

 // NO struct WINDOW; // ncurses stubs
 typedef struct _win_st WINDOW;
 typedef short i16;
 typedef unsigned char u8;

#include <ncurses.h> // chtype !!!


namespace TTY_CURSES {

 class tty_curses_CSR {
 public:
	tty_curses * TTY_curses; // PARENT
	WINDOW * win; // = stdscr;

	tty_curses_CSR( tty_curses * _TTY_curses ); // PARENT
	void set_stdscr( WINDOW * W = NULL );

	bool get_WH( WH_t & _WH );
	bool get_W_H( int & _W, int & _H ); // set into vars directly (api_var)

	XY_t XY_was;

	void get_XY( XY_t & XY) {
		get_yx_was();
		XY = XY_was;
	}

	void get_yx() {
		get_yx_was();
	}
	void get_yx_was();

	void move( const XY_t & XY ) {
		wmove(win,XY.Y, XY.X);
	}
	void move( int y, int x ) {
		XY_t XY;
		XY.X = x;
		XY.Y = y;
		move(XY);
	}

	void printf( const char * fmt, ... );
	void print( const char * fmt, ... );
	void puts( const char * str );
	void putc_byte( const u8 ch );
	void putc_acs( const chtype ch );
//	void put_c( const u8 ch );
	void add_ch( const chtype ch );

	// chtype comes from ... means ...

	// attr_on is a CURSES MACRO
	void fgbg_on( int attr ) { attron( attr ); }
	void fgbg_off( int attr ) { attroff( attr ); }

	void clear_to_eol();
	void move_status_line(); // and clear it

	int get_ch();

	void box_mode_start();
	void box_mode_end();
	void box_v_line( i16 x, i16 y1, i16 y2 );
	void box_h_line( i16 y, i16 x1, i16 x2 );
	void putc_box( enum_UDLR udlr );
	void putc_box( UDLR UDLR);


 };

}; // namespace

#endif

