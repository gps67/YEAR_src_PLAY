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

 /*!
 */
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

	void box_mode_start(); // saves into JB_box_start
	void box_mode_end();
	void box_v_line( i16 x, i16 y1, i16 y2 );
	void box_h_line( i16 y, i16 x1, i16 x2 );
	bool box_line_start();
	bool box_line_end();
	bool box_line_to( XY_t XY_2 );
	 JB_t JB_box_start;
	 JB_t JB1;
	 JB_t*JB1_keep;
	void set_JB1( JB_t & JB_next );
	void JB1_udlr_changed()
	{
		if( JB1_keep )
		 JB1_keep->udlr = JB1.udlr;
	}
	bool box_line_by( X_t dx, Y_t dy );
	bool box_line_left( X_t dx );
	bool box_line_left_to( X_t x ) { return box_line_to_X( x ); }
	bool box_line_right( X_t dx );
	bool box_line_up( Y_t dy );
	bool box_line_down( Y_t dy );
	bool box_line_to_X( X_t x );
	bool box_line_to_X( const JB_t & JB_layout )
	{ return box_line_to_X( JB_layout.XY.X ); } // NO JOIN just XPOS

	bool box_line_to_Y( Y_t y );
	void putc_box( enum_UDLR udlr );
	void putc_box( UDLR UDLR);
	void putc_box( JB_t & jb );
	void box_line_keep_JB(  JB_t & jb );
	void box_line_jump_JB(  JB_t & jb );
	void box_line_amat_JB(  JB_t & jb );

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

