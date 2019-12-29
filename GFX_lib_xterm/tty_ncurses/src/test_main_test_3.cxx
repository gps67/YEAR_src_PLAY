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
	tty.setup_mouse();

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
	
	// XYWH == (0,0) ( W,H ) of parent // screen

#if 0
	// as much left gap as required // NONE
	XYWH.XY.X ++;
	XYWH.XY.Y ++;

	XYWH.XY.X ++;
	XYWH.XY.Y ++;
#endif

	// SUBZONE drawable
	XYWH.WH.W -= XYWH.XY.X;
	XYWH.WH.H -= XYWH.XY.Y;

//	XYWH.WH.W --;
//	XYWH.WH.H --;
	// as much right gap as required // NONE

	XY_t XY_2;

	SHAPE_ONE shape(csr, "absent_MUX") ; // 
	// SHAPE_ONE shape(csr, PROG_NAME) ; // 

	// TODO
	// shape.set_stretch_gap_pos( 0.3 ) // (full_width - used_width) * (0.3 0.7)
	/*
	   HERE
	   micro script macro expr, or var in compond expr FSM
	   mini-zone of 256 items perhaps 10
	   Layout does it's own OPCODE computations, eg

	   	TOKEN "XPOS_3" {
			XPOS_t from SUBLEX_of_XPOS
			// float or PIXEL_POS or fraction_dpi
			IDX = TOKEN_idx
		}
		TOKEN W_title_gaps_box_expr == 
			STO_MACHINE
			obtain str
			foreach char in str
				W ++ W(char) // actual code TBS by pixel upgrade
		STMT
		 W += char_width // extra_padding = 0; W += 1;
		STMT
		 add XPOS_3 to XPOS[idx] as _3
		 // regen using %s_%d XPOS / *XPOS_3* / idx = N ++
	*/

	shape.set_XYWH( XYWH );	// set frame size

	shape.draw(csr); // draw frame 
//	 INFO("sleep ()");
 //        refresh();
//	sleep(20);


	int nk=0;
	while(nk++<6000) {
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
	  case KEY_MOUSE: {
	  	MEVENT event;
		if( getmouse(&event) != OK) {
			WARN("getmouse(event) not OK");
			continue;
		}
		XY_t pos_mouse;
		pos_mouse.X = event.x;
		pos_mouse.Y = event.y;
		long bstate = event.bstate;
		csr.move( XY_t( 4, 7 ));
		csr.printf("k==KEY_MOUSE");
		csr.move( pos_mouse );
		csr.printf("Mouse at row=%d, column=%d bstate=0x%08lx",
                 pos_mouse.Y, pos_mouse.X, bstate);
		csr.move( pos_mouse );
	  }
	  break;
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
	  	nk+=10000;
		return true;
		
	  break;
	  default:
		csr.printf(" k==%d", k);
	 }
	}

        return true;
  }
