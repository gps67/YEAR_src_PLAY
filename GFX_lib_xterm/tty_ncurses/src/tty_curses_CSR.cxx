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
	: TTY_curses(_TTY_curses)
	, win(NULL)
	{
		win = stdscr;
	}

	void tty_curses_CSR:: set_stdscr( WINDOW * W  ) {
		if( ! W ) W = stdscr;
		win = W;
	}

	bool tty_curses_CSR:: get_WH( WH_t & _WH )
	{
		_WH.W = COLS;
		_WH.H = LINES;
		return true;
	}

	bool tty_curses_CSR:: get_W_H( int & _W, int & _H )
	{
		_W = COLS;
		_H = LINES;
		return true;
	}

	void tty_curses_CSR:: 
	get_yx_was() {
		getyx(
			win,
			XY_was.Y,
			XY_was.X
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

	void tty_curses_CSR:: putc_acs( const chtype ch )
	{
		if(!ch)
		 gdb_break_point();
		if(!ch)
		 puts("0x00");
		waddch( win, (chtype) ch );
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
		 wvline( win, 0, (y2-y1)+1 );
		 move( y2+1, x );
		 return;
	}

	void tty_curses_CSR:: box_h_line( i16 y, i16 x1, i16 x2 )
	{
		// man 3 curs_border
		move( y, x1 );
		whline( win, 0, (x2-x1)+1 );
		move( y, x2+1 );
		return;
	}

	bool tty_curses_CSR:: box_line_up( Y_t dy ) {
		return box_line_by( 0, -dy );
	}

	bool tty_curses_CSR:: box_line_down( Y_t dy ) {
		return box_line_by( 0, dy );
	}

	bool tty_curses_CSR:: box_line_right( X_t dx ) {
		return box_line_by( dx, 0 );
	}

	bool tty_curses_CSR:: box_line_left( X_t dx ) {
		return box_line_by( -dx, 0 );
	}

	bool tty_curses_CSR:: box_line_to_X( X_t X )
	{
		return box_line_right( X - JB1_xy.X );
	}
	bool tty_curses_CSR:: box_line_to_Y( Y_t Y )
	{
		return box_line_down( Y - JB1_xy.Y );
	}

	bool tty_curses_CSR:: box_line_by( X_t dx, Y_t dy )
	{
		XY_t XY = JB1_xy;
		XY.X += dx;
		XY.Y += dy;
		box_line_to( XY );
	}

	bool tty_curses_CSR:: box_line_end()
	{
		JB0_udlr.OR_VAL( JB1_udlr );
		move( JB0_xy );
		putc_box( JB0_udlr );
	}

	bool tty_curses_CSR:: box_line_start()
	{
		JB0_udlr.clear(); // the closing item
		get_XY( JB0_xy );

		JB1_udlr.clear(); // the recent current JB1
		get_XY( JB1_xy );
	}

	bool tty_curses_CSR:: box_line_to( XY_t _JB2_xy )
	{
		UDLR JB2_udlr;
		XY_t JB2_xy = _JB2_xy; // are always going to POS2

		if( JB1_xy.X == JB2_xy.X ) { // HORIZONTAL
			if( JB1_xy.Y == JB2_xy.Y )
			{ return FAIL("SAAME POINT");  }

			if( JB1_xy.X < JB2_xy.X ) {
			// left to right
				JB1_udlr.set_R();
				JB2_udlr.set_L();

				move( JB1_xy );
				putc_box( JB1_udlr );

				int len = JB2_xy.X - JB1_xy.X; 
				if( len > 0 ) {
				 whline( win, 0, len ); 
				}
				putc_box( JB2_udlr ); // early but OK

				JB1_udlr = JB2_udlr;
				JB1_xy   = JB2_xy  ;

				return true;

			} else {
			// right to left
				JB1_udlr.set_L();
				JB2_udlr.set_R();

				move( JB1_xy );
				putc_box( JB1_udlr );

				XY_t POS2 = JB1_xy;
				XY_t POS1 = JB2_xy;
				POS2.X ++;
				POS1.X --;

				int len = POS2.X - POS1.X ; 
				if( len > 0 ) {
				 move( POS1 );
				 whline( win, 0, len ); 
				}
				move( JB2_xy );
				putc_box( JB2_udlr );

				JB1_udlr = JB2_udlr;
				JB1_xy   = JB2_xy  ;

				return true;
			}
			return true;
		}
		if( JB1_xy.Y == JB2_xy.Y ) { // VERTICAL
			if( JB1_xy.Y < JB2_xy.Y ) {
			// top to bottom
				JB1_udlr.set_D();
				JB2_udlr.set_U();

				move( JB1_xy );
				putc_box( JB2_udlr );

				XY_t POS1 = JB1_xy;
				POS1.Y ++;

				XY_t POS2 = JB2_xy;
				POS1.Y --;

				int len = POS2.Y - POS1.Y ; 
				if( len > 0 ) {
				 move( POS1 );
				 wvline( win, 0, len ); 
				}
				move( JB2_xy );
				putc_box( JB2_udlr ); // early but OK

				JB1_udlr = JB2_udlr;
				JB1_xy   = JB2_xy  ;
			} else {
			// bottom to top
				JB1_udlr.set_U();
				JB2_udlr.set_D();

				move( JB1_xy );
				putc_box( JB1_udlr ); // early but OK

				XY_t POS1 = JB1_xy;
				POS1.Y --;

				XY_t POS2 = JB2_xy;
				POS2.Y ++;

				int len = POS2.Y - POS1.Y ; 
				if( len > 0 ) {
				 move( POS2 );
				 wvline( win, 0, len ); 
				}

				move( JB2_xy );
				putc_box( JB2_udlr );

				JB1_udlr = JB2_udlr;
				JB1_xy   = JB2_xy  ;
			}
			return true;
		}

		JB1_udlr = JB2_udlr;
		JB1_xy   = JB2_xy  ;
		return FAIL("not on same LINE COL ");
	}

	void tty_curses_CSR:: box_line_keep_JB( XY_t & JB_XY, UDLR & JB_udlr )
	{
		JB_XY = JB1_xy;
		JB_udlr = JB1_udlr;
	}

	void tty_curses_CSR:: box_line_jump_JB( XY_t & JB_XY, UDLR & JB_udlr )
	{
		JB1_xy = JB_XY;
		JB1_udlr = JB_udlr;
	}

	void tty_curses_CSR:: box_line_amat_JB( XY_t & JB_XY, UDLR & JB_udlr )
	{
		// JB1_xy = JB_XY;
		JB1_udlr .OR_VAL( JB_udlr );
		move( JB1_xy );
		putc_box( JB1_udlr );
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
		WH_t WH;
		get_WH( WH );

		XY_t POS( 1, WH.H-1 );
		move( POS );
		box( win, 0, 0 );
		// wclrtoeol( win );
	}

	int tty_curses_CSR:: get_ch()
	{
		return
		wgetch( win );
	}


};

