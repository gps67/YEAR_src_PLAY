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
	, JB1_keep(NULL)
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

	void tty_curses_CSR:: get_yx_was()
	{
		getyx( // ncurses macro
			win,
			XY_was.Y,
			XY_was.X
		);
	}

	// reports bad XY and clips a bit
	void tty_curses_CSR:: pre_MOVE( XY_t & XY )
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
	
	void tty_curses_CSR:: move( const XY_t & XY )
	{
		XY_t XY_adj = XY;
		pre_MOVE( XY_adj );
		wmove( win, XY_adj.Y, XY_adj.X );
		XY_was = XY; // lost by printf or addch or ...
	}

	void tty_curses_CSR:: move_to_X( X_t X )
	{
		get_yx_was();
		XY_t XY;
		XY.Y = XY_was.Y;
		XY.X = X;
		move(XY);
	}

	void tty_curses_CSR:: move_to_Y( Y_t Y )
	{
		get_yx_was();
		XY_t XY;
		XY.X = XY_was.X;
		XY.Y = Y;
		move(XY);
	}

	void tty_curses_CSR:: move_right( X_t X ) {
		get_yx_was();
		XY_t XY = XY_was;
		XY.X += X;
		move(XY);
	}

	void tty_curses_CSR:: move_down( Y_t Y ) {
		get_yx_was();
		XY_t XY = XY_was;
		XY.Y += Y;
		move(XY);
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

	void tty_curses_CSR:: putc_box( JB_t & jb )
	{
		move( jb.XY );
		putc_box( (UDLR) jb );
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
		// no rmacs required
	}

	void tty_curses_CSR:: box_mode_end()
	{
		// no rmacs required
	}

	void tty_curses_CSR:: box_v_line( i16 x, i16 y1, i16 y2 )
	{
		// wvline dy==1 draws one char, at y1 not y2
		 move( y1, x );
		 wvline( win, 0, (y2-y1)+1 );
		 move( y2, x );
		 return;
	}

	void tty_curses_CSR:: box_h_line( i16 y, i16 x1, i16 x2 )
	{
		// man 3 curs_border
		move( y, x1 );
		whline( win, 0, (x2-x1)+1 );
		move( y, x2 );
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
		return box_line_right( X - JB1.XY.X );
	}
	bool tty_curses_CSR:: box_line_to_Y( Y_t Y )
	{
		return box_line_down( Y - JB1.XY.Y );
	}

	bool tty_curses_CSR:: box_line_by( X_t dx, Y_t dy )
	{
		XY_t XY = JB1.XY;
		XY.X += dx;
		XY.Y += dy;
		box_line_to( XY );
		return true;
	}

	bool tty_curses_CSR:: box_line_end()
	{
		JB1_keep = NULL; // maybe
		// expect curr JB1 .XY == //
		JB_box_start.OR_VAL( JB1 );
		putc_box( JB_box_start );
		JB1 = JB_box_start; // copy udlr as well as copy same XY
		return true;
	}

	bool tty_curses_CSR:: box_line_start()
	{
		JB1_keep = NULL; 
		get_yx();
		JB1.XY = XY_was;
		JB1.udlr_clear();
		if(0) JB1.OR_VAL( UDLR_UDLR );
		box_line_keep_JB( JB_box_start );
		// JB_box_start = JB1;
		return true;
	}
	void tty_curses_CSR:: nnpp(u8 lbl)
	{
		return; // silent
		static int nn = 0;
		nn++;
		if(nn>26) nn=0;
		putc_byte( lbl );
		return; // only show LBL
		putc_byte( '@' + nn );
	}

	void tty_curses_CSR:: set_JB1( JB_t & JB_next )
	{
		JB1 = JB_next;
		JB1_keep = NULL; // lose track of previous JB1
	}

	bool tty_curses_CSR:: box_line_to( XY_t _JB2_xy )
	{
		// LINE is from JB1 to JB2
		// JB2 is INCL
		// but as a corner it will be updated
		// JB2 written twice, on way in, on way out
		JB_t JB2 = _JB2_xy; // from XY to JB

		if( JB1.XY.Y == JB2.XY.Y ) { // HORIZONTAL same Y
			if( JB1.XY.X == JB2.XY.X )
			{ putc_byte( 'X'); return FAIL("SAAME POINT");  }

			if( JB1.XY.X < JB2.XY.X ) {
			// left to right
				JB1.set_R(); // .udlr
				JB2.set_L(); // .udlr
				JB1_udlr_changed();

				putc_box( JB1 );
				nnpp( 'a' ); //

				// -1 because already placed JB1
				int len = JB2.XY.X - JB1.XY.X - 1 + 1 ; 
				if( len > 0 ) {
				 whline( win, 0, len ); 
				}
				// XY should be asis
				putc_box( JB2 ); // early but OK
				nnpp( 'b' ); //

				JB1 = JB2;
				JB1_keep = NULL;

				return true;

			} else {
			// JB1 right to left JB2
				JB1.set_L();
				JB2.set_R();
				JB1_udlr_changed();

				putc_box( JB1 );
				nnpp( 'c' ); //

				XY_t POS1 = JB2; // left
				XY_t POS2 = JB1; // right
				POS1.X ++;
				POS2.X --;

				int len = POS2.X - POS1.X + 1 ; 
				if( len > 0 ) {
				 move( POS1 );
				 whline( win, 0, len ); 
				}
				putc_box( JB2 );
				nnpp( 'd' ); //

				JB1 = JB2;
				JB1_keep = NULL;

				return true;
			}
			return true;
		}
		if( JB1.XY.X == JB2.XY.X ) { // VERTICAL same X
			if( JB1.XY.Y < JB2.XY.Y ) {
			// top to bottom
				JB1.set_D();
				JB2.set_U();
				JB1_udlr_changed();

				putc_box( JB1 );
				nnpp( 'e' ); //

				XY_t POS1 = JB1.XY;
				XY_t POS2 = JB2.XY;
				POS1.Y ++;
				POS2.Y --;

				int len = POS2.Y - POS1.Y + 1 ; 
				if( len > 0 ) {
				 move( POS1 );
				 wvline( win, 0, len ); 
				}
				move( JB2 );
				nnpp( 'f' ); //

				JB1   = JB2  ;
				JB1_keep = NULL;
			} else {
			// bottom to top
				JB1.set_U();
				JB2.set_D();
				JB1_udlr_changed();

				putc_box( JB1 ); //
				nnpp( 'g' ); //

				XY_t POS1 = JB2.XY; // top
				XY_t POS2 = JB1.XY; // bottom
				POS1.Y ++;
				POS2.Y --;

				int len = POS2.Y - POS1.Y + 1 ; 
				if( len > 0 ) {
				 move( POS1 ); // one more than JB1
				 wvline( win, 0, len ); 
				}

				move( JB2 );
				nnpp( 'h' ); //

				JB1 = JB2;
				JB1_keep = NULL;
			}
			return true;
		}

		JB1 = JB2;
		return FAIL("not on same LINE COL ");
	}

	void tty_curses_CSR:: box_line_keep_JB( JB_t & JB_tee )
	{
		// SAVE corner or TEE
		JB_tee = JB1;
		JB1_keep = & JB_tee; // keep for exit route
	}

	void tty_curses_CSR:: box_line_jump_JB( JB_t & JB_tee )
	{
		// calling code is resuming from jb_tee
		JB1 = JB_tee;
		JB1_keep = NULL;
		JB1_keep = & JB_tee; // keep for exit route
		// any updated UDLR values get written back to // _STO _CACHE
	}

extern "C" void exit( int status );
	void tty_curses_CSR:: box_line_amat_JB( JB_t & jb )
	{
		// claims to have arrived at a JB with state from back then
		if( jb.XY != JB1.XY ) {
			if(1) WARN("(%d,%d) !=(%d,%d)",
			 jb.XY.X,
			 jb.XY.Y,
			 JB1.XY.X,
			 JB1.XY.Y
			);
			if(0) printf("(%d,%d %d,%d)",
			 jb.XY.X,
			 jb.XY.Y,
			 JB1.XY.X,
			 JB1.XY.Y
			);
		//	puts("( jb.XY != JB1.XY )");
		//	exit(1);
		//	return;
		}
		// OK caller wrong math , but trust the approx corner shape
		JB1.OR_VAL( jb );
		putc_box( JB1 );
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

