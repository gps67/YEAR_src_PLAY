#ifndef tty_curses_WIN_H
#define tty_curses_WIN_H

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

 /*!
 */
 class tty_curses_WIN {
 public:
	tty_curses * TTY_curses; // PARENT
	WINDOW * win; // = stdscr;

	tty_curses_WIN( tty_curses * _TTY_curses ); // PARENT
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

	void pre_MOVE( XY_t & XY ); // reports bad XY and clips a bit

	void move( const XY_t & XY ) ;
	void move( int y, int x ) {
		XY_t XY;
		XY.X = x;
		XY.Y = y;
		move(XY); // hello compiler
	}
	void move_to_X( X_t X );
	void move_to_Y( X_t Y );
	void move_right( X_t X );
	void move_down( X_t Y );
	void move_left( X_t X ) { move_right( -X ); }
	void move_up( X_t Y ) { move_down( -Y ); }

	void printf( const char * fmt, ... );
	void print( const char * fmt, ... );
	void puts( const char * str );
	void putc_byte( const u8 ch );
	void putc_acs( const chtype ch );
//	void put_c( const u8 ch );
	void add_ch( const chtype ch );
	void nnpp( u8 lbl);

	// chtype comes from ... means ...

	// attr_on is a CURSES MACRO
	void fgbg_on( int attr ) { attron( attr ); }
	void fgbg_off( int attr ) { attroff( attr ); }

	void clear_to_eol();
	void move_status_line(); // and clear it

	int get_ch();

	// in box, in general, we are at JB1, soon JB2

// OK ERR
// overshooting end of line by 1
// overwriting start of line by 1
// line_between_JB is itself -2 (on top of that)

// operator (XY_t) typecast => XY ;; permits use as parameter wrap
// use of C++ cast is worth formalising TYPE_CAST_HERE
// CAST_into CAST_from
// operator (&XY_t) use of wrap as var (as well as val)
// NB JB is by default UDLR, could have made it multi-base
// DETECT USAGE CAST to XY from JB needing 


 };

}; // namespace

#endif

