// SHAPE_ // #include <ncurses.h>
// SHAPE_ // #include "dgb.h"
// SHAPE_ // #include "e_print.h"
// SHAPE_ // #include <stdio.h>
// SHAPE_ // #include <locale.h>
// SHAPE_ // #include "str1.h"

#include "test_SHAPE_ONE.h"
#include "test_SHAPE_TWO.h" 

#include "tty_curses.h"
#include "tty_curses_CSR.h"

extern "C" void exit(int);

// namespace TTY_CURSES {
//	INTENTIONALLY NOT IN namespace, as if from api callers view
//	DITTO Makefile can have users invokation + _include _link _load _call
// }; // namespace

	using namespace TTY_CURSES;


  bool main_test_3()
  {       
	// one per TTY
	tty_curses tty;
	tty.setup();

	// one per APP // was supposed to be the blinking CSR // TODO
	TTY_CURSES::tty_curses_CSR csr( & tty );
	csr.set_stdscr(); // tell CSR it is the top level window
	// csr.move( Y X )
	// csr.printf("fmt", ARGS );
	// csr.get_panel()


	// WHEN THIS is derived from _CSR
	// TTY_CURSES::tty_curses_CSR & csr = *this;

	// _CSR ?
	int X1 = 4;  // then some INDENT system tables of numbers varnames
	XY_t XY;
	XY.Y = 2;
	XY.X = 4;

	// CSR_t csr // local tool to attach variables to
	// FIELD csr.FieldName // GEN adds it to the derived-from-CSR_t
	// DIALECT guesses_then_pretends_it_always_knew_by_adding_item_to_list
	// DIALECT csr.fieldname // fields just spring to life on mention
	// BENCH retro-fits DECLARATIONS of PREBUILT for SCRIPT api

	int A1 = A_BOLD;
	int A2 = A_UNDERLINE;

	csr.move( XY );
	XY.Y++;
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
	int X2 = csr.XY_was.X;
	csr.move( XY ); 
      //  csr.printf("--------------------------------------------------------");
	XY.Y--;
	csr.move( XY ); // X2 tho
	XY.Y++;
        csr.printf(".. . .  .   .    .     .        .           .             .");

	csr.move( XY );
	XY.Y++;
        csr.printf("--------------------------------------------------------");

	// 
        csr.move_status_line();
	csr.move( XY );
	csr.print( "TODO GIT Branch 01_");
	csr.fgbg_on( A2 );
	csr.print( "_Hello_World");
	csr.fgbg_off( A2 );
	// NOTE the screen cursor has been all over
	// but XY.Y is where we want it to be

	XY.Y += 2; // Two Blank Lines // OMITTED_not_EMITTED // XY_JUMP
	csr.move( XY );
	XY.Y++;
        csr.printf("DODO GIT Branch 01_Hello_World");
        csr.printf("");
	csr.move( XY );
	XY.Y++;
        csr.printf("--------------------------------------------------------");

	XY.Y += 3; // XY_JUMP
	int y_0 = XY.Y - 1;
	int x_0 = X1 - 1;
	csr.move( XY );
        csr.printf("Hello World!");
	XY.Y += 1;
	csr.get_yx();
	XY_t XY_2 = csr.XY_was;

	// TODO leave blinking CSR at ! 
	// TODO highlighting CSR

	// LINES COLS
	int SCN_W = 80;
	int SCN_H = 25; // bad day, 44 is OK
	if(!csr.get_W_H( SCN_W, SCN_H )) // assigning into parameter vars
	 return FAIL_FAILED();

	XYWH_t XYWH;
	XYWH.XY.X = 2;
	XYWH.XY.Y = XY.Y+3;
	XYWH.WH.W = 13;
	XYWH.WH.H = 5;

	SHAPE_ONE shape(csr) ; // XY not YX 
	shape.set_XYWH( XYWH );
	XY.Y += XYWH.WH.H;

	XY.Y += 7; // the unknown margin

	XYWH.XY.X = ( XYWH.XY.X + XYWH.WH.W ); // the old _X==7 from left
	XYWH.WH.W = SCN_W - XYWH.XY.X - 6; // includes MAIN box edge +1
	XYWH.XY.Y = XY.Y;
	XYWH.WH.H = SCN_H - XYWH.XY.Y - 6; // includes MAIN box edge +1

	SHAPE_TWO ladder(csr) ; // XY not YX 
	ladder.set_XYWH( XYWH );
	XY.Y += XYWH.WH.H; // could draw a lot later, after Layout completed, remotely

if(1)	shape.draw( csr );
	ladder.draw(csr);


	int nk=0;
	while(nk++<10) {
	 csr.move( XY_2 );
	 // rewrite as follows:
	 // csr.set_blinking_csr(XY_POS_2, "EXPLAINER"); // when_as_CSR explain
	 // csr.enable_blinking
         refresh();
         int k = csr.get_ch();
		csr.move( XY_t( 4,  5 ));
		char * kstr = keybound( k, 1 );
		csr.printf("k=='%s' keybound is useless", kstr);
		free(kstr);
	 switch( k ) {
	  case KEY_RESIZE:
		csr.move( XY_t( 4, 6 ));
		csr.printf("k==KEY_RESIZE");
	  break;
	  case 'Q':
	  case 'q':
	  case 'k':
	  case KEY_BACKSPACE:
//	  case KEY_ESC:
	  case 33:
		INFO("1000 steps");
	  	nk+=1000;
	  break;
	 }
	}

        return true;
  }
