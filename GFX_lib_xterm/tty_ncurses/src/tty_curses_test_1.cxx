#include <ncurses.h>
#include "tty_curses.h"
#include "tty_curses_CSR.h"
#include "dgb.h"
#include "e_print.h"
#include "stdio.h"
#include <locale.h>

extern "C" void exit(int);

// namespace TTY_CURSES {
//	INTENTIONALLY NOT IN namespace, as if from api callers view
//	DITTO Makefile can have users invokation + _include _link _load _call
// }; // namespace


  bool main_test1()
  {       
	TTY_CURSES::tty_curses tty;
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
