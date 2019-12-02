#include <ncurses.h>
#include "tty_curses.h"
#include "tty_curses_CSR.h"
#include "dgb.h"
#include "e_print.h"
#include <stdio.h>
#include <locale.h>
#include "str1.h"

extern "C" void exit(int);

// namespace TTY_CURSES {
//	INTENTIONALLY NOT IN namespace, as if from api callers view
//	DITTO Makefile can have users invokation + _include _link _load _call
// }; // namespace

	using namespace TTY_CURSES;

  struct SHAPE_ONE {

	// SURFACE is
	tty_curses_CSR & CSR; // shared with other users of window
  	i16 X0; 
  	i16 Y0; 

	str1 title;
	i16 glyph_width; // dpi=4

	i16 dx_to_box;
	i16 dx_of_frame;
	i16 dy_of_frame;

	SHAPE_ONE( tty_curses_CSR & _CSR )
	: CSR( _CSR )
	{
		title = "Title";
	}


	bool set_X_Y_W_H(
	 i16 _X, 
	 i16 _Y, 
	 i16 _W, 
	 i16 _H
	) {
		X0 = _X;
		Y0 = _Y;
		dx_of_frame = _W;
		dy_of_frame = _H;

		dx_to_box = 2;
		dx_to_box = 7;

		glyph_width = title.str_len();
		return true;
	}

	bool draw()
	{
		i16 x0 = X0 + 0; // inside line
		i16 x1 = X0 + 0; // inside line
		i16 x2 = x1 + dx_to_box;
		i16 x3 = x2 + 1 + glyph_width + 1;
		i16 x4 = X0 + dx_of_frame - 1; // -1 thing

		i16 y0 = Y0;		// GAP TOP LINE 
		i16 y1 = y0 + 1;	// TEXT line
		i16 y2 = y1 + 1;	// LINE

		i16 y4 = Y0 + dy_of_frame;

		CSR.move( Y0, X0 );
		CSR.puts("/");

		CSR.move( y0, x2 );
		CSR.putc_box( false, true, false, true );
		CSR.box_h_line( y0, x2+1, x3-1 );		// over
		CSR.putc_box( false, true, true , false);

		CSR.move( y2, x2 );
		CSR.putc_box( false, true, true , false);
		CSR.box_h_line( y2, x2, x3 );			// below
		CSR.putc_box( false, true, false, true );

		CSR.move( y1, x0 );				// left
		CSR.putc_box( true, true, true, false );
		CSR.box_h_line( y1, x1, x2-1 );			// below
		CSR.puts(" "); // SP before Title
		CSR.puts( title ); // str1 Title
		CSR.puts(" "); // SP before Title
		CSR.putc_box( true, true, false, true );

		CSR.box_h_line( y1, x3, x4 );
		CSR.putc_box( false, true, true, false );
	// end title line

		CSR.box_v_line( x0, y2+1, y4 ); // left
		CSR.box_v_line( x4, y2, y4 ); // right
		CSR.move( y4, x0 );
		CSR.putc_box( true, false, true, false );
		CSR.box_h_line( y4, x1, x4 ); // bottom
		CSR.putc_box( true, false, false , true);

	}

	// no fg_bg yet

  };


  bool main_test1()
  {       
	tty_curses tty;
	tty.setup();

	TTY_CURSES::tty_curses_CSR csr( & tty );
	csr.set_stdscr();

	// WHEN THIS is derived from _CSR
	// TTY_CURSES::tty_curses_CSR & csr = *this;

	// _CSR ?
	int y = 2;
	int X1 = 4;  // then some INDENT system tables of numbers varnames

	// CSR_t csr // local tool to attach variables to
	// FIELD csr.FieldName // GEN adds it to the derived-from-CSR_t
	// DIALECT guesses_then_pretends_it_always_knew_by_adding_item_to_list
	// DIALECT csr.fieldname // fields just spring to life on mention
	// BENCH retro-fits DECLARATIONS of PREBUILT for SCRIPT api

	int A1 = A_BOLD;
	int A2 = A_UNDERLINE;

	csr.move( y++, X1 );
	csr.fgbg_on( A1 );
	csr.fgbg_on( A2 );
        csr.printf("Hello World!");
	csr.fgbg_off( A2 );
	csr.fgbg_off( A1 );

	// SPUD_HEAD detects generic
	// csr.attr( A1 );
	// csr.move( Y1, X1 );
        // csr.put(nice_text);
        // csr.printf(fmt, args);
        // buf.printf(fmt, args);
        // csr.put( buf ); // SPUD HEAD has accumulated text upto somewhere
	// SCRIPT wants to know more about VA_ARGS 

	csr.get_yx();
	int X2 = csr.x_was;
	csr.move( y, X1 );
      //  csr.printf("--------------------------------------------------------");
	y--;
	csr.move( y++, X2 );
        csr.printf(".. . .  .   .    .     .        .           .             .");

	csr.move( y++, X1 );
        csr.printf("--------------------------------------------------------");

	// 
        csr.move_status_line();
	csr.print( "TODO GIT Branch 01_");
	csr.fgbg_on( A2 );
	csr.print( "_Hello_World");
	csr.fgbg_off( A2 );
	// NOTE the screen cursor has been all over
	// but y is where we want it to be

	y += 2; // Two Blank Lines // OMITTED_not_EMITTED // XY_JUMP
	csr.move( y++, X1 );
        csr.printf("TODO GIT Branch 01_Hello_World");
        csr.printf("");
	csr.move( y++, X1 );
        csr.printf("--------------------------------------------------------");

	y += 3; // XY_JUMP
	int y_0 = y - 1;
	int x_0 = X1 - 1;
	csr.move( y++, X1 );
        csr.printf("Hello World!");
	csr.get_yx();
	int y_2 = csr.y_was;
	int x_2 = csr.x_was + 1;

	SHAPE_ONE shape(csr) ; // XY not YX 
	shape.set_X_Y_W_H( 7, 15, 25, 5 );
	shape.draw();


	int nk=0;
	while(nk++<10) {
         refresh();
         int k = csr.get_ch();
		csr.move( 5, 4 );
		char * kstr = keybound( k, 1 );
		csr.printf("k=='%s' keybound is useless", kstr);
		free(kstr);
	 switch( k ) {
	  case KEY_RESIZE:
		csr.move( 6, 4 );
		csr.printf("k==KEY_RESIZE");
	  break;
	  case 'Q':
	  case 'q':
	  case 'k':
	  case KEY_BACKSPACE:
//	  case KEY_ESC:
	  case 33:
	  	nk+=1000;
	  break;
	 }
	}

        return true;
  }
