#include <ncurses.h>
#include "tty_curses.h"
#include "dgb.h"
#include "e_print.h"
#include "stdio.h"
#include <locale.h>

extern "C" void exit(int);


  bool main_test1()
  {       
	TTY_CURSES::tty_curses tty;
	tty.setup();

	int y_line = 2;
	int x_column = 4;

	int &y = y_line;
	int &x = x_column;

	move( y++, x );
        printw("Hello World!");
        printw("TODO GIT Branch 01_Hello_World");
	move( y++, x );
        printw("--------------------------------------------------------");
	move( y++, x );
        printw("TODO GIT Branch 01_Hello_World");
        printw("");
        printw("--------------------------------------------------------");
	int nk=0;
	while(nk++<10) {
         refresh();
         int k = getch();
		move( 5, 4 );
		char * kstr = keybound( k, 1 );
		printw("k=='%s' keybound is useless", kstr);
		free(kstr);
	 switch( k ) {
	  case KEY_RESIZE:
		move( 6, 4 );
		printw("k==KEY_RESIZE");
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

#include <signal.h>
  static void handle_SIGINT(int sig) {
  	endwin();
	WARN("CTRL-C INTR");
	e_print("===============\n");
	exit(sig);
  }

	static
	void handle_SIGWINCH( int signal ) {
		// best to not handle it, let curses handle it, get KEY_RESIZE
		endwin();
		refresh(); // picks WH from xterm's ENV somehow // LINES COLS
	        INFO("LINES = %d COLS = %d", LINES, COLS );
		// now compute new layout and draw it
		FAIL("unwritten");
	}

namespace TTY_CURSES {

 // ncurses is C and used loads of global names
 // but we could pretend that didn't matter
 // and keep ours in a class in a namespace

  tty_curses:: ~tty_curses () {
	INFO("END TTY // _ncurses");
	def_prog_mode(); // before shell out
	// def_shell_mode(); also exists
        endwin(); 
	delscreen( SCREEN_HERE );
	SCREEN_HERE = NULL;
	// hope next refresh sets itself as global ?
	// hope other fallback is stdscr
  }

  tty_curses:: tty_curses () {
   SCREEN_HERE = NULL;
  }

  bool tty_curses:: set_term_global () {
   INFO("TTY global");
   if( SCREEN_HERE ) {
    SCREEN * prev = set_term( SCREEN_HERE ); // all ncurses funcs to this
    INFO("LINES = %d COLS = %d", LINES, COLS );
    //NOR // COLS = 20;
    return true;
   } else {
    return FAIL( "curses SCREEN_HERE NULL");
    return false;
   }
  }
/* PANEL
  tty_curses:: go_xy ( int x, int y ) {
  	go_x( x );
  	go_y( y );
  }
  tty_curses:: go_y ( int x ) {
  	go_x( x );
  	go_y( y );
  }
*/
 

  bool tty_curses:: setup () {
  	(void) signal(SIGINT, handle_SIGINT); // TTY is bigger than curses
// best to not handle it, let curses handle it, get KEY_RESIZE
//  	(void) signal(SIGWINCH, handle_SIGWINCH ); // TTY is bigger than curses

	setlocale(LC_ALL, "");
	setlocale(LC_ALL, "en_GB");

	// alternate to initscr();

	SCREEN_HERE = newterm( NULL, stdin, stdout );
	set_term_global();

        // initscr(); // curses
	cbreak();		// char break not line buffered
	noecho();		// TTY echo off

	// // raw();			// TTY line buffering mode
	nonl();			// TTY NL -> CR LF // OFF
	intrflush(stdscr, false );
	keypad(stdscr, true );	// get KEY events
	// tabs as left by tput init

	if(has_colors()) {
	 start_color();
	 // colour pair(0, not available
	 init_pair( 1, COLOR_GREEN, COLOR_BLACK );
	 // app start on default from setup
	 attrset( COLOR_PAIR(1) );
	}
	return true;
  }

}; // namespace
