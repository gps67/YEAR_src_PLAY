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
#include "argv_decoder.h"

extern "C" void exit(int);
extern "C" void sleep(int);

// namespace TTY_CURSES {
//	INTENTIONALLY NOT IN namespace, as if from api callers view
//	DITTO Makefile can have users invokation + _include _link _load _call
// }; // namespace

	using namespace TTY_CURSES;


  extern 
  bool main_test_3(argv_decoder & ARGS);
  bool main_test_3(argv_decoder & ARGS)
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


	int X1 = 4;  // then some INDENT system tables of numbers varnames

	// CSR_t csr // local tool to attach variables to
	// FIELD csr.FieldName // GEN adds it to the derived-from-CSR_t
	// DIALECT guesses_then_pretends_it_always_knew_by_adding_item_to_list
	// DIALECT csr.fieldname // fields just spring to life on mention
	// BENCH retro-fits DECLARATIONS of PREBUILT for SCRIPT api

	int A1 = A_BOLD;
	int A2 = A_UNDERLINE;

        csr.move_status_line();
	csr.fgbg_on( A1 );
	csr.fgbg_on( A2 );
	
	buffer1 buf;
	const char * PROG_NAME = ARGS.get_prog_name(buf);
        csr.printf("THIRD %s", PROG_NAME);

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

	// 


	XYWH_t XYWH;
	if(!csr.get_WH( XYWH.WH )) // assigning into parameter vars
	 return FAIL_FAILED();

#if 0
	XYWH.XY.X ++;
	XYWH.XY.Y ++;

	XYWH.XY.X ++;
	XYWH.XY.Y ++;
#endif

	XYWH.WH.W -= XYWH.XY.X;
	XYWH.WH.H -= XYWH.XY.Y;

//	XYWH.WH.W --;
//	XYWH.WH.H --;

	XY_t XY_2;

	SHAPE_ONE shape(csr, PROG_NAME) ; // 
	shape.set_XYWH( XYWH );

	shape.draw(csr);
//	 INFO("sleep ()");
 //        refresh();
//	sleep(20);


	int nk=0;
	while(nk++<10) {
	 csr.move( XY_2 );
	 // rewrite as follows:
	 // csr.set_blinking_csr(XY_POS_2, "EXPLAINER"); // when_as_CSR explain
	 // csr.enable_blinking
	 INFO("refresh ()");
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
