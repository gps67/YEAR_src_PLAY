#ifndef tty_curses_WIN_BOX_H
#define tty_curses_WIN_BOX_H

#include "tty_curses_WIN.h"

namespace TTY_CURSES {

class tty_curses_WIN_BOX : public tty_curses_WIN {
 public:

	tty_curses_WIN_BOX( tty_curses * _TTY_curses )
	: tty_curses_WIN( _TTY_curses )
	, JB1_keep(NULL)
	{}

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

 };
}; // namespace

#endif

